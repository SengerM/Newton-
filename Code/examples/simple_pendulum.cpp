#include "main.h"

#define LENGTH 1
#define THETA0 150

using namespace std;

int main() {
	size_t i; // General use counter.
	ofstream ofile; // Output file stream.
	string gpstr; // General purpose string.
	string path_str;
	// System of particles ----------------------------- //
	ParticleSystem sys;
	// Particles -------------------------------------- //
	Particle 	pendulum("pendulum");
	// FORCES ----------------------------------------- //
	Force	K_field("K_field", armonic_3D_field, 3, true),
			G_field("G_field", uniform_gravitational_field, 1, true);
	Vec3D center(0,0,0), g(0,-9.8,0);
	double k=10000, l=LENGTH;
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
	pendulum.Mass() = 1;
	pendulum.Position() = Vec3D(LENGTH*sin(THETA0*PI/180), -LENGTH*cos(THETA0*PI/180), 0);
	// ----------------------------------------------------
	// FORCES ----------------------------------------- //
	cerr << "Configuring forces... " << endl;
	K_field.AddParameter(&k);
	K_field.AddParameter(&l);
	K_field.AddParameter(&center);
	K_field.Check();
	
	G_field.AddParameter(&g);
	G_field.Check();
	// ------------------------------------------------ //
	// System of particles ----------------------------- //
	cerr << "Configuring system of particles..." << endl;
	sys.AddParticle(pendulum);
	
	sys.AddInteraction(pendulum, K_field);
	sys.AddInteraction(pendulum, G_field);
	
	sys.CalcForces(); // Calculates the forces to their initial value.
	
	cerr << KGRN << "Initial conditions are: --------" << RST << endl;
	sys.Print();
	cerr << KGRN << "--------------------------------" << RST << endl;
	// ---------------------------------------------------------
	// Calculation ---------------------------------------------
	cerr << "Preparing calculation..." << endl;
	
	gpstr = path_str;
	gpstr.append("/");
	gpstr.append(OUTPUT_DATA_TEXT_FILE);
	ofile.open(gpstr.c_str()); // Opens the file for saving the simulation data.
	
	cerr << "Calculating... " << endl;
	cerr << "Progress: 00 %";
	sys.PrintTXT(ofile); // Print initial conditions.
	i=0;
	while ( sys.GetTime() <= SIMULATION_TIME ) {
		i++;
		sys.StepEuler(TIME_STEP); 
		if ( i%(size_t)(SIMULATION_TIME/(TIME_STEP*N_EXPORT_POINTS))-2 == 0 ) { // Prints the simulated data to output file.
			sys.PrintTXT(ofile);
		}
		if (i%(size_t)(SIMULATION_TIME/(TIME_STEP*100))-2 == 0) { // Prints progress percentage of calculation.
			cerr << '\b' << '\b' << '\b' << '\b';
			cerr << setfill('0') << setw(2) << ((int)(i/(size_t)(SIMULATION_TIME/(TIME_STEP*100)))) << " %";
		}
	}
	cerr << endl;
	cerr << "Calculation finished." << endl;
	
	ofile.close();
	// -------------------------------------------------- //
	
	cerr << "Calling Octave script «oct_quick_plot.m»." << endl;
	cerr << KBLU;
	system("octave oct_quick_plot.m");
	cerr << RST;
	
	cerr << LAST_MSG << endl;
	
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


