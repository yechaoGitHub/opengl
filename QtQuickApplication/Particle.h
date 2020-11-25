#pragma once
#include "GlUtil.h"
#include "glm/vec3.hpp"

class Particle
{
public:
	Particle();
	~Particle();

	__declspec(align(16)) glm::vec3 position;
	__declspec(align(16)) glm::vec3 velocity;
	__declspec(align(16)) glm::vec3 acceleration;
	__declspec(align(16)) glm::vec3 color;
	float							age;
	float							life;
	float							size;
	uint32_t						state;
};

