#version 430 core
layout(local_size_x = 32, local_size_y = 32) in;

#define PI 3.1415926535898

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
	vec3	coordinate;
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

layout(binding = 1) buffer indexBlock
{
	uint indices[];
}indicesBuffer;

layout(binding = 2) uniform atomic_uint atomicCounter;

uniform float startTick;
uniform float curTick;
uniform float preTick;

uniform uint particleCount;

uniform uint width;
uniform uint height;

Particle InitializeParticle(uint x , uint y) 
{
	float fWidth = float(width);
	float fHeight = float(height);
	float fX = float(x);
	float fY = float(y);

	vec2 stlocation = vec2(fX / fWidth, fY / fHeight);

	Particle particle;
	particle.position = vec3(stlocation, 1.0);
	particle.coordinate = vec3(stlocation, 1.0);
	particle.velocity =  vec3(0.1, 0.5, 0.0);
	particle.acceleration = vec3(0.0);
	particle.color = vec3(random22(stlocation * 13.765435), 1.0);;
	particle.age = 0.0;
	particle.life = 0.0;
	particle.size = 0.0;
	particle.angle = 0.0;
	particle.state = _STATE_ACTIVE_;

	return particle;
}

vec2 CalculateVelocity(float angle, float vr, float ar)
{
	float x = vr * cos(angle * ar);
	float y = vr * ((sin(angle * ar) + 1.0) / 2.0);

	return vec2(x, y);
}

#define curParticle particlesBuffer.particles[index]

float tickInterval = curTick - preTick;
float passTick = curTick - startTick;
float totalTime = 3.0;

void main()
{
	uint x = gl_GlobalInvocationID.x;
	uint y = gl_GlobalInvocationID.y;

	if (x >= width || y >= height) return;

	float fHeight = float(height);
	float fX = float(x);
	float fY = float(y);
	float scanfThreshold = float(y);
	float curScanfValue = (height / 10.0) * passTick;


	uint index = x + y * height;
	if (curParticle.state == _STATE_INIT_)
	{
		curParticle = InitializeParticle(x, y);
	}
	else 
	{
		curParticle.angle += (random(vec2(fX,fY) + curTick) - 0.5)  * 4.0 * PI;
		curParticle.velocity = vec3(CalculateVelocity(curParticle.angle, 0.5, 0.1), 0.0);
		curParticle.position += (curParticle.velocity * 0.01) * step(scanfThreshold, curScanfValue);
	}

	if (curParticle.position.x < 1.0 && 
		curParticle.position.y < 1.0) 
	{
		uint particleIndex = atomicCounterIncrement(atomicCounter);
		indicesBuffer.indices[particleIndex] = index;
	}
}
