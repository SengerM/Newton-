#include "main.h"
/*
	* This «main()» implements an example simulation of some of the planets
	* of the solar system. It is a very simplified model, you will see that
	* the unique forces considered are those that the sun imparts over each
	* planet. But results are not that bad.
	* It is recomended to simulate with a TIME_STEP (macro) of about 100 and
	* a simulation time large enough to see intresting movements, like about
	* 30 years.
*/

#define N_EXPORT_POINTS 1000
#define SIMULATION_TIME (1.0*60.0*60.0*24.0*365.0*100)
#define TIME_TO_START_SAVING_DATA 0
#define TIME_STEP (1.0*60.0*60.0)
#define PERCENTAGE_PRINT_STEP 1
#define RESULTS_DIGITS 3 // Number of digits to print in the output file (scientific notation).

using namespace std;

int main() {
	size_t i, I; // General use counter.
	ofstream ofile; // Output file stream.
	string gpstr; // General purpose string.
	string path_str;
	// System of particles ----------------------------- //
	ParticleSystem solar_system;
	// Particles -------------------------------------- //
	Particle 	sun("sun"),
				venus("venus"),
				earth("earth"),
				mars("mars"),
				jupiter("jupiter"),
				saturn("saturn");
	// FORCES ----------------------------------------- //
	Force	gravity("force of gravity", force_of_gravity, 2, false);
	double gravitational_constant = 6.674e-11;
	double minimum_radius = 0;
	gravity.AddParameter(&gravitational_constant);
	gravity.AddParameter(&minimum_radius);
	// Variables for handling time and date of the current simulation ------ //
	string timeString;
	// ------------------------------------------------- //
	
	
	
	
	
	
	
	
	
	
	
	
	
	system("clear");
	cerr << INIT_MSG << endl;
	
	timeString = now(); // Function that returns a C++ string object containing the date and time in the format "%Y_%m_%d_%H_%M_%S".
	// --------------------------------------------------------------
	
	cerr << BOLD << "Simulation number: " << RST << timeString << endl;
	cerr << BOLD << '\t' << "Simulation time: " << RST << SIMULATION_TIME << endl;
	cerr << BOLD << '\t' << "Time step: " << RST << TIME_STEP << endl;
	cerr << BOLD << '\t' << "Number of data points to be exported: " << RST << N_EXPORT_POINTS << endl;
	cerr << "-------------------" << endl;
	
	path_str = string("");
	path_str.append(OUTPUT_DIRECTORY);
	path_str.append("/");
	path_str.append(timeString);
	
	gpstr = string("");
	gpstr.append("mkdir ");
	gpstr.append(path_str);
	system(gpstr.c_str()); // Creates a new directory in wich to store all the information.
	
	
	
	
	
	
	
	
	
	
	
	
	
	// Particles ----------------------------------------
	cerr << "Configuring particles..." << endl; 
	
	sun.Mass() = 1.989e30; // Data from Wikipedia.
	
	venus.Mass() = 4.869e24; // Data from Wikipedia.
	venus.Position() = Vec3D(108.2e9, 0, 0); // Data from Wikipedia.
	venus.Velocity() = Vec3D(0, 108.2e9/(224.7*24*60*60)*2*PI, 0); // Data from Wikipedia.
	
	earth.Mass() = 5.9736e24; // Data from Wikipedia.
	earth.Position() = Vec3D(149.6e9, 0, 0); // Data from Wikipedia.
	earth.Velocity() = Vec3D(0, 149.6e9/(365.26*24*60*60)*2*PI, 0); // Data from Wikipedia.
	
	mars.Mass() = 6.39e23; // Data from Wikipedia.
	mars.Position() = Vec3D(227.94e9, 0, 0); // Data from Wikipedia.
	mars.Velocity() = Vec3D(0, 227.94e9/(686.98*24*60*60)*2*PI, 0); // Data from Wikipedia.
	
	jupiter.Mass() = 1.898e27; // Data from Wikipedia.
	jupiter.Position() = Vec3D(778.33e9, 0, 0); // Data from Wikipedia.
	jupiter.Velocity() = Vec3D(0, 778.33e9/(4332.71*24*60*60)*2*PI, 0); // Data from Wikipedia.
	
	saturn.Mass() = 5.683e26; // Data from Wikipedia.
	saturn.Position() = Vec3D(1429.4e9, 0, 0); // Data from Wikipedia.
	saturn.Velocity() = Vec3D(0, 1429.4e9/(10759.5*24*60*60)*2*PI, 0); // Data from Wikipedia.
	
	// ----------------------------------------------------
	
	
	
	
	
	
	
	
	
	
	
	
	
	// FORCES ----------------------------------------- //
	
	// ------------------------------------------------ //
	
	
	
	
	
	// System of particles ----------------------------- //
	cerr << "Configuring system of particles..." << endl;
	
	solar_system.AddParticle(sun);
	solar_system.AddParticle(venus);
	solar_system.AddParticle(earth);
	solar_system.AddParticle(mars);
	solar_system.AddParticle(jupiter);
	solar_system.AddParticle(saturn);
	
	solar_system.AddInteraction(venus, sun, gravity);
	solar_system.AddInteraction(earth, sun, gravity); // Force of gravity applied to the earth due to the sun.
	solar_system.AddInteraction(mars, sun, gravity);
	solar_system.AddInteraction(jupiter, sun, gravity);
	solar_system.AddInteraction(saturn, sun, gravity);
	
	
	
	solar_system.CalcForces(); // Calculates the forces to their initial value.
	
	solar_system.CalcForces(); // Calculates the forces to their initial value.
	
	
	cerr << KGRN << "Initial conditions are: --------" << RST << endl;
	solar_system.Print(cerr);
	cerr << KGRN << "--------------------------------" << RST << endl;
	// ---------------------------------------------------------
	
	
	
	
	
	
	// Calculation ---------------------------------------------
	cerr << "Preparing calculation..." << endl;
	
	gpstr = path_str;
	gpstr.append("/");
	gpstr.append(OUTPUT_DATA_TEXT_FILE);
	ofile.open(gpstr.c_str()); // Opens the file for saving the simulation data.
	
	cerr << "Calculating... ";
	cerr << "00 %";
	I = SIMULATION_TIME/TIME_STEP; // Final value of the counter "i".
	for (i=0; i<I; i++) {
		solar_system.StepRK4(TIME_STEP);
		if (i%(I/N_EXPORT_POINTS) == 0) {
			ofile.open(gpstr.c_str(), std::fstream::app); // Opens the file for saving the simulation data, in append mode.
			ofile << std::scientific << std::setprecision(RESULTS_DIGITS); //std::setprecision(std::numeric_limits<double>::digits10); // Sets the precision for printing the doubles.
			solar_system.PrintTXT(ofile);
			ofile.close();
		}
		if (((i*100)/I)%PERCENTAGE_PRINT_STEP == 0)
			print_percentage(i,I);
	}
	print_percentage(i,I);
	cerr << endl;
	// -------------------------------------------------- //
	
}

std::string now(void) { // Function that returns a C++ string object containing the date and time in the format "%Y_%m_%d_%H_%M_%S".
	time_t current_time;
	struct tm * time_info;
	char timeString_c_like[200];
	
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString_c_like, sizeof(timeString_c_like), "%Y_%m_%d_%H_%M_%S", time_info);
	return string(timeString_c_like);
}


void print_percentage(size_t i, size_t I) {
	cerr << '\b' << '\b' << '\b' << '\b';
	cerr << setfill('0') << setw(2) << (i*100)/I << " %";
}

int random_uniform(int low, int high) {
    if (low > high) 
		return low + (std::rand() % (low - high + 1));
    return low + (std::rand() % (high - low + 1));
}
