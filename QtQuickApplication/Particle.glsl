#version 430 core
layout (local_size_x = 1024) in;

#define _STATE_INIT_	0
#define _STATE_ACTIVE_	1

float rand11(float n)
{
	n += 0.1415926;
	n *= 12.9898 * 78.233;
	return fract(sin(n) * 43758.5453);
}

vec3 rand13(float n)
{
	float v1 = rand11(n);
	float v2 = rand11(n * 5.3214);
	float v3 = rand11(n * 2.32436);

	return vec3(v1, v2, v3);
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
	uint	state;
};

uniform int particleCount;

layout (binding = 0) buffer particlesBlock {
	Particle particles[];
}particlesBuffer;

layout (binding = 1) uniform atomic_uint counter;

void InitParticle(out Particle particle, float seed)
{
	particle.position = rand13(seed);
	particle.velocity = rand13(seed + 0.96214);
	particle.acceleration = rand13(seed + 0.6244);
	particle.color = rand13(seed + 0.4567);
	particle.age = 0.0;
	particle.life = 10.0;
	particle.size = 1.0;
	particle.state = _STATE_ACTIVE_;
}

void MoveParticle(out Particle particle)
{
	particle.position += particle.velocity;

	if(length(particle.position) > 1.0)
	{
		particle.state = _STATE_INIT_;
	}
}

void main() 
{
	uint index = gl_GlobalInvocationID.x;
	if (index >= particleCount)
	{
		return;
	}

	if(particlesBuffer.particles[index].state == _STATE_INIT_)
	{
		InitParticle(particlesBuffer.particles[index], index);
	}
	else 
	{
		MoveParticle(particlesBuffer.particles[index]);
	}

	atomicCounterIncrement(counter);
}

