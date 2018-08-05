#ifndef _MY_FORCES_H_
#define _MY_FORCES_H_

/*
	* This file (and the «.cpp») contains the user-writed functions that 
	* define the forces to be used in configuring the «Force» objects
	* in the «main()». This forces must satisfy the prototype that is 
	* defined in «Force.h», wich is:
	* 
	*	 typedef Vec3D(*ForceFormula_t)(Particle & a, Particle & b, std::vector<void*> & params); // Data type that defines a pointer to a function that calculates the force.
	* 
	* The parameters of this functions are:
	* 	- Particle & a: Reference to the particle to wich the force is applied.
	* 	- Particle & b: Reference to the particle that imparts the force to «a».
	* 	- std::vector<void*> & params: Additional parameters that need to be
	* 				passed to the force like constants or whatever.
	* 
	* If the force is due to an interaction between two particles then
	* data is taken from the two particles (like position or velocities).
	* If the force is due to a field (so there are not two but only one 
	* particle involved) the particle «b» is not used. 
*/

#define GRAVITATIONAL_CONSTANT 6.674e-11 // N*m^22/kg^2

#include <vector>
#include "Particle.h"
#include "Vec3D.h"

Vec3D elastic_force(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D force_of_gravity(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D damping_force(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D colission_force(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D damping_force_with_distance(Particle &a, Particle &b, std::vector<void*> & params);

Vec3D armonic_3D_field(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D gravitational_field(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D uniform_gravitational_field(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D uniform_magnetic_field(Particle &a, Particle &b, std::vector<void*> & params);
Vec3D viscous_field(Particle &a, Particle &b, std::vector<void*> & params);

#endif
