#version 430 core
layout (local_size_x = 1024) in;

#define PI 3.1415926535 
#define _STATE_INIT_	0
#define _STATE_ACTIVE_	1

float random(in vec2 _st) {
	return fract(sin(dot(_st.xy,
		vec2(12.9898, 78.233))) *
		43758.5453123);
}

float noise(in vec2 _st) {
	vec2 i = floor(_st);
	vec2 f = fract(_st);

	// Four corners in 2D of a tile
	float a = random(i);
	float b = random(i + vec2(1.0, 0.0));
	float c = random(i + vec2(0.0, 1.0));
	float d = random(i + vec2(1.0, 1.0));

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(a, b, u.x) +
		(c - a) * u.y * (1.0 - u.x) +
		(d - b) * u.x * u.y;
}

float fbm(in vec2 _st) {
	int numOctaves = int(min(10.0, log2(800.0))) - 3;

	float v = 0.0;
	float a = 0.5;
	vec2 shift = vec2(100.0);
	// Rotate to reduce axial bias
	mat2 rot = mat2(cos(0.5), sin(0.5),
		-sin(0.5), cos(0.50));

	for (int i = 0; i <= numOctaves; i++)
	{
		v += a * noise(_st);
		_st = rot * _st * 2.0 + shift;
		a *= 0.5;
	}

	return v;
}

float rand11(float n)
{
	n += 0.1415926;
	n *= 12.9898 * 78.233;
	return fract(sin(n) * 43758.5453);
}

vec3 rand13(float n)
{
	float v1 = rand11(n * 7.543654);
	float v2 = rand11(n * 5.3214123);
	float v3 = rand11(n * 2.32436);

	return vec3(v1, v2, v3);
}

float StToUv(float st) 
{
	return (st - 0.5) * 2.0;
}

vec3 StToUv(vec3 st) 
{
	return (st - vec3(0.5)) * 2.0;
}

struct Particle
{
	vec3	position;
	vec3	velocity;
	vec3	acceleration;
	vec3	color;
	float	age;
	float	life;
	float	size;
	float	angle;
	uint	state;
};

uniform float curTick;
uniform float preTick;
uniform int particleCount;

layout (binding = 0) buffer particlesBlock 
{
	Particle particles[];
}particlesBuffer;

layout (binding = 1) uniform atomic_uint counter;

Particle InitParticle(float seed)
{
	Particle particle;

	particle.position = rand13(seed);
	particle.velocity = StToUv(rand13(seed + 0.96214));
	particle.acceleration = rand13(seed + 0.6244);
	particle.color = rand13(seed + 0.4567);
	particle.age = 0.0;
	particle.life = 10.0;
	particle.size = 1.0;
	particle.angle = 2.0 * PI * rand11(seed + 1.135454);
	particle.state = _STATE_ACTIVE_;
	
	return particle;
}

Particle ClearParticle() 
{
	Particle particle;

	particle.position = vec3(0.0);
	particle.velocity = vec3(0.0);
	particle.acceleration = vec3(0.0);
	particle.color = vec3(0.0);
	particle.age = 0.0;
	particle.life = 0.0;
	particle.size = 0.0;
	particle.angle = 0.0; 
	particle.state = _STATE_INIT_;

	return particle;
}


vec2 CalculateVelocity(float angle, float vr, float ar) 
{
	return vec2(vr * cos(angle * ar * PI), vr * sin(angle * ar));
}

void main() 
{
	uint index = gl_GlobalInvocationID.x;
	if (index >= particleCount)
	{
		return;
	}

	Particle particle = particlesBuffer.particles[index];
	float tickInterval = curTick - preTick;

	if(particle.state == _STATE_INIT_)
	{
		particle = InitParticle(index + curTick);
	}
	else 
	{
		vec2 uv = vec2(particle.position.x, particle.position.y);
		vec2 seed = vec2(uv);

		particle.angle += StToUv(fbm(seed));
		vec2 velocity = CalculateVelocity(particle.angle, 0.5, 0.1) * 1.2;
		particle.position += vec3(velocity, 0.0) * tickInterval;

		if (abs(particle.position.x) > 1.0 || abs(particle.position.y) > 1.0)
		{
			particle = ClearParticle();
		}
	}

	particlesBuffer.particles[index] = particle;
}

