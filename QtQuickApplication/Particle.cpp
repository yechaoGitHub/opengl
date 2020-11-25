#include "Particle.h"

Particle::Particle() :
	position{},
	velocity{},
	acceleration{},
	color{},
	age(0.0),
	life(0.0),
	size(0.0),
	state(0)
{
}

Particle::~Particle()
{
}
