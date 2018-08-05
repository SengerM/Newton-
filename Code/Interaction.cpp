#include "Interaction.h"

using namespace std;

Interaction::Interaction(Force & f, size_t p) {
	force = &f;
	with = p;
}

Force & Interaction::GetForce(void) {
	return *force;
}

size_t Interaction::GetParticleNumber(void) {
	return with;
}

Vec3D Interaction::GetValue(void) {
	return value;
}

bool Interaction::WithField(void) {
	return force->IsField();
}

std::string Interaction::GetForceName(void) {
	return force->GetName();
}

void Interaction::SetValue(Vec3D & vec) {
	value = vec;
}
