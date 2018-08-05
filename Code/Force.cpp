#include "Force.h"

using namespace std;

/*
	Force object constructor.
	Params are:
		* string str: A string specifying the name of the force. For exmaple "elastic force 1" or "my magnetic field".
		* ForceFormula_t function: This is a function whose prototype is of the form specified by «ForceFormula_t» that defines the way the force is obtained in terms of the interacting particles and other parameters.
		* size_t param_numer: This number specifies the nuber of params the force function «function» is waiting for. For example if the force is an elastic force of the form «F=k/2*(x-l)^2 then it uses two params «k» and «l». This number is used internally when checking if the force object is ready to use.
		* bool is_f: If «true» then the force object is configured as being a force due to a field, that means that the force acts over ONE particle. If «false» then the object is configured as being a force between TWO particles.
*/
Force::Force(std::string str, ForceFormula_t function, size_t param_number, bool is_a_field) { // Constructor.
	F_formula = function;
	n_params = param_number;
	name = str;
	is_field = is_a_field;
	is_ready = false;
}

void Force::AddParameter(void * param) {
	parameters.push_back(param);
}

bool Force::Check(void) {
	if (parameters.size() != n_params) {
		is_ready = false;
		cerr << WARNING_MSG << "Force object named «" << name << "» is not ready to use." << endl;
		return false;
	}
	cerr << "Force object named " << BOLD << UNDERLINE << name << RST << " is ready to use." << endl;
	is_ready = true;
	return true;
}

Vec3D Force::operator()(Particle &a, Particle &b) { // Interaction within two particles.
	if (!is_ready) {
		if (!(this->Check()))
			cerr << ERROR_MSG << "Trying to use force object " << BOLD << UNDERLINE << name << RST << " not properly configured. Param number needed is different from param number setted. " << endl;
	}
	if (is_field)
		cerr << WARNING_MSG << "The object " << BOLD << UNDERLINE << name << RST << " was created as being a field and it was used as being a force (that is acting over two particles)." << endl;
	return F_formula(a, b, parameters);
}

Vec3D Force::operator()(Particle &a) { // Interaction with a field.
	if (!is_ready) {
		if (!(this->Check()))
			cerr << ERROR_MSG << "Trying to use force object " << BOLD << UNDERLINE << name << RST << " not properly configured. Param number needed is different from param number setted. " << endl;
	}
	if (!is_field)
		cerr << WARNING_MSG << "The object " << BOLD << UNDERLINE << name << RST << " was created as being a force between two particles and it has been called as being a field that acts over only one particle." << endl;
	return F_formula(a, a, parameters);
}

bool Force::IsField() { // Tells whether the force is a field or not.
	return is_field;
}

string Force::GetName() {
	return name;
}
