#pragma once
#include "GlUtil.h"
#include "glm/vec3.hpp"

class Particle
{
public:
	Particle();
	~Particle();

	ALIGN(16)	glm::vec3 position;
	ALIGN(16)	glm::vec3 coordinate;
	ALIGN(16)	glm::vec3 velocity;
	ALIGN(16)	glm::vec3 acceleration;
	ALIGN(16)	glm::vec3 color;
	float		age;
	float		life;
	float		size;
	float		angle;
	uint32_t	state;
};

