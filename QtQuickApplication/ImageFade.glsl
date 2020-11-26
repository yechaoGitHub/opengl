#version 430 core
layout(local_size_x = 32, local_size_y = 32) in;

#define _STATE_INIT_	0
#define _STATE_ACTIVE_	1

float random(in vec2 _st) {
	return fract(sin(dot(_st.xy,
		vec2(12.9898, 78.233))) *
		43758.5453123);
}

vec2 random22(in vec2 st)
{
	return vec2(fract(sin(st.x * 12.9898) * 43758.5453123),
				fract(cos(st.y * 78.233) * 61315.3544253));
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

layout(binding = 0) buffer particlesBlock
{
	Particle particles[];
}particlesBuffer;

uniform float curTick;
uniform float preTick;
uniform int particleCount;
float tickInterval = curTick - preTick;

uniform int width;
uniform int height;

Particle InitializeParticle(vec2 location) 
{
	vec2 imageSize(width, height);
	vec2 stlocation = location / imageSize;

	Particle particle;
	particle.position = vec3(stlocation, 1.0);
	particle.velocity =  vec3(0.1 * random22(stlocation), 1.0);
	particle.acceleration = vec3(0.0);
	particle.color = vec3(0.0);
	particle.age = 0.0;
	particle.life = 0.0;
	particle.size = 0.0;
	particle.angle = 0.0;
	particle.state = _STATE_ACTIVE_;

	particle.velocity.y = abs(particle.velocity.y);

	return particle;
}

vec2 CalculateVelocity(float angle, float vr, float ar)
{
	return vec2(vr * cos(angle * ar), vr * clamp(0.0, 1.0, sin(angle * ar)));
}

void main()
{
	int x = gl_GlobalInvocationID.x;
	int y = gl_GlobalInvocationID.y;

	if (x >= width || y >= height) return;

	int index = x + y * height;
	Particle par = particlesBuffer.particles[index];

	if (par.state == _STATE_INIT_) 
	{
		par = InitializeParticle(vec2(x, y));
	}
	else 
	{
		par.angle += random(vec2(x,y) + curTick);
		par.velocity = CalculateVelocity(par.angle, 0.5, 0.1);
		par.position += vec3(par.velocity, 0.0) * tickInterval;
	}

	particlesBuffer.particles[index] = par;
}
