#include "Particle.h"

using namespace std;

Particle::Particle(void) {
	mass = 1;
	charge = 0;
}

Particle::Particle(std::string str) {
	name = str;
	mass = 1;
	charge = 0;
}

Vec3D & Particle::Position(void) {
	return position;
}

Vec3D & Particle::Velocity(void) {
	return velocity;
}

double & Particle::Mass(void) {
	return mass;
}

double & Particle::Charge(void) {
	return charge;
}

std::string Particle::Name(void) { // Returns the name of the string.
	return name;
}

std::ostream& operator<<(std::ostream& os, Particle& p) {
	Vec3D vec;
	if ((p.Name()).length())
		os << UNDERLINE << BOLD << p.Name() << RST << endl;
	vec = p.Position();
	os << "Position: " << vec << endl;
	vec = p.Velocity();
	os << "Velocity: " << vec << endl;
	os << "Mass: " << p.Mass() << endl;
	os << "Charge: " << p.Charge();
	return os;
}
