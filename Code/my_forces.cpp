#include "my_forces.h"

using namespace std;

Vec3D elastic_force(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 2 (first «k» and second «length»).
	* bool is_f: false (this is a classicla force, not a force due to a field).
	*/
	Vec3D vec;
	double 	k = *((double*)params[0]), // Elastic constant of the spring.
			length = *((double*)params[1]); // Natural length of the spring.
	
	vec = (b.Position() - a.Position());
	vec = vec*(k*(1-length/vec.Abs()));
	
	return vec;
}

Vec3D force_of_gravity(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 1 (recieves no additional parameters).
	* 	1) param[0] is the gravitational constant (the value you like).
	* 	2) param[1] is the minimum radius allowed for non-zero force (for avoiding numerical problems).
	* bool is_f: false (this is the classical newtonian force of gravity between two masses).
	*/
	Vec3D vec = (b.Position() - a.Position());
	double abs = vec.Abs();
	double minimum_radius = *((double*)params[1]);
	if (abs < minimum_radius) // No interaction.
		return Vec3D(0,0,0);
	double 	constant = *((double*)params[0]);
	return vec/(abs/abs/abs*constant*a.Mass()*b.Mass());
}

Vec3D colission_force(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 4
	* 	1) Colission radius.
	* 	2) Damping coefficient.
	* 	3) Repulsion coefficient.
	*/
	Vec3D r = (b.Position()-a.Position());
	double distance = r.Abs();
	double 	colission_radius = *((double*)params[0]);
	if (distance > colission_radius) // There is no interaction.
		return Vec3D(0,0,0);
	
	double 	damping_coef = *((double*)params[1]);
	double 	repulsion_coef = *((double*)params[2]);
	Vec3D damping, repulsion;
	damping = (r/distance)*( (b.Velocity()-a.Velocity()).Dot(r/distance) )*damping_coef;
	repulsion = r/distance/distance*repulsion_coef;
	return damping - repulsion;
}

Vec3D damping_force(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 1 (damping constant, the force is of the form F = damping_constant*(vb-va)).
	* bool is_f: false (this is a classicla force, not a force due to a field).
	*/
	Vec3D r=(b.Position()-a.Position());
	double abs_r=r.Abs();
	return r*(( (b.Velocity()-a.Velocity()).Dot(r) )*(*((double*)params[0]))/(abs_r*abs_r));
}

Vec3D damping_force_with_distance(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* 1) Damping constant.
	*/
	Vec3D r=(b.Position()-a.Position());
	double abs_r=r.Abs();
	return r*(( (b.Velocity()-a.Velocity()).Dot(r) )*(*((double*)params[0]))/(abs_r*abs_r*abs_r));
}

Vec3D armonic_3D_field(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 3 (first «k», second «length» and third «center»).
	* bool is_f: true (this is a force due to a field).
	*/
	Vec3D vec;
	double 	k=*((double*)params[0]), // Elastic constant of the spring.
			length=*((double*)params[1]); // Natural length of the spring.
	Vec3D 	center=*((Vec3D*)params[2]); // Position in space of the center of the field.
	
	vec = center - a.Position();
	vec = vec*k*(vec.Abs()-length);
	return vec;
}

Vec3D viscous_field(Particle &a, Particle &b, std::vector<void*> & params) {
	Vec3D vec;
	double 	damping = *((double*)params[0]); // Elastic constant of the spring.
	return a.Velocity()*(-damping);
}

Vec3D gravitational_field(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 2 (first the mass generating the filed and second the position of that mass).
	* bool is_f: true (this is a force due to a field).
	*/
	
	Vec3D vec = (*((Vec3D*)params[1])) - a.Position();
	double abs = vec.Abs();
	return vec/abs/abs/abs*GRAVITATIONAL_CONSTANT*a.Mass()*(*((double*)params[0]));
}

Vec3D uniform_gravitational_field(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 1
	* bool is_f: true (this is a force due to a field).
	*/
	Vec3D g = *((Vec3D*)params[0]); // Gravitational acceleration.
	return g*(a.Mass());
}

Vec3D uniform_magnetic_field(Particle &a, Particle &b, std::vector<void*> & params) {
	/*
	Information nedeed to construct the «Force» object:
	* size_t param_number: 1
	* bool is_f: true (this is a force due to a field).
	*/
	Vec3D B = *((Vec3D*)params[0]); // Magnetic field B.
	return (a.Velocity()*B)*a.Charge();
}

