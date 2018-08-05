#ifndef _FORCE_H_
#define _FORCE_H_

#include "Vec3D.h"
#include "Particle.h"
#include "color.h"
#include <vector>

struct Parameter {
	double val;
	Vec3D vec;
};

typedef Vec3D(*ForceFormula_t)(Particle &, Particle &, std::vector<void*> &); // Data type that defines a pointer to a function that calculates the force.

class Force {
private:
	ForceFormula_t F_formula; // Here it will be stored a function that knows how to calculate the force.
	std::string name; // Name of the force object so it can be identified.
	std::vector<void*> parameters; // Parameters that characterize the force such as constants and parameters.
	size_t n_params; // This is setted to the number of params that «F_formula» is waiting for, so that it can be checked whether or not the object was configured properly.
	bool is_field; // This flag indicates whether the object is a field that acts over one particle or a force between two particles.
	bool is_ready; // This flag indicates whether the object is ready to use or not.
	
	Force(); // The constructor without arguments is private so it cannot be created an empty force.
	
public:
	Vec3D operator()(Particle &, Particle &); // Interaction within two particles.
	Vec3D operator()(Particle &); // Interaction with a field.
	
	Force(std::string str, ForceFormula_t function, size_t param_number, bool is_a_field); // Constructor.
	void AddParameter(void *);
	bool Check(void); // Checks whether the object is ready to use or not.
	bool IsField(); // Tells whether the force is a field or not.
	std::string GetName();
};

#endif
