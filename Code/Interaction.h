#ifndef _INTERACTION_H_
#define _INTERACTION_H_

#include <string>
#include "Force.h"

class Interaction {
private:
	Force * force;
	size_t with; // This is the number of the particle wich I am interacting with.
	Vec3D value; // Here it is stored the value of the force.
	
	Interaction(); // This constructor is private so it can't be used.
public:
	Interaction(Force &, size_t);
	Force & GetForce(void);
	size_t GetParticleNumber(void);
	Vec3D GetValue(void);
	void SetValue(Vec3D &);
	bool WithField(void);
	std::string GetForceName(void);
};

#endif
