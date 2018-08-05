#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <iostream>
#include <string>
#include "Vec3D.h"
#include "color.h"

class Particle {
private:
	Vec3D position;
	Vec3D velocity;
	double mass;
	double charge;
	std::string name;
	
public:
	Particle(void);
	Particle(std::string);
	Vec3D & Position(void);
	Vec3D & Velocity(void);
	double & Mass(void);
	double & Charge(void);
	std::string Name(void);
};

std::ostream& operator<<(std::ostream& os, Particle& p);

#endif
