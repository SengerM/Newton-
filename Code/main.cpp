#include "main.h"

#include "user_functions.n++"

int main() {
	size_t i, j, I; // General use counters.
	ofstream ofile; // Output file stream.
	string gpstr; // General purpose string.
	string path_str;
	string timeString;
	// ------------------------------------------------- //

	// Begin --------------------------------------------------------
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
	
	#include "user_macros.n++"
	// Particles ----------------------------------------
	cerr << "Creating and configuring particles... " << endl;
	#include "user_particles.n++"
	// ----------------------------------------------------
	// FORCES ----------------------------------------- //
	cerr << "Creating and configuring forces... " << endl;
	#include "user_forces.n++" // In this file is where the user has to define the forces.
	// ------------------------------------------------ //
	// System of particles ----------------------------- //
	cerr << "Creating and configuring system of particles..." << endl;
	#include "user_system.n++"
	sys.CalcForces(); // Calculates the forces to their initial value.
	// ----------------------------------------------------
	// Print initial conditions and configuration information text file -----
	gpstr = path_str;
	gpstr.append("/");
	gpstr.append(INITIAL_CONDITIONS_DATA_TEXT_FILE);
	ofile.open(gpstr.c_str()); // Opens the file for saving the simulation data.
	ofile << "Initial conditions for simulation number " << timeString << endl;
	sys.Print(ofile);
	ofile << endl << "--------------------------------------------------------------------" << endl;
	ofile << "Values for the macros are:" << endl;
	ofile << "N_EXPORT_POINTS" << '\t' << N_EXPORT_POINTS << endl;
	ofile << "SIMULATION_TIME" << '\t' << SIMULATION_TIME << endl;
	ofile << "TIME_TO_START_SAVING_DATA" << '\t' << TIME_TO_START_SAVING_DATA << endl;
	ofile << "TIME_STEP" << '\t' << TIME_STEP << endl;
	ofile.close();
	// ------------------------------------------------------------------------
	
	//~ cerr << KGRN << "Initial conditions are: --------" << RST << endl;
	//~ sys.Print(cerr);
	cerr << KGRN << "--------------------------------" << RST << endl;
	// ---------------------------------------------------------
	
	// Calculation ---------------------------------------------
	cerr << "Preparing calculation..." << endl;
	gpstr = path_str;
	gpstr.append("/");
	gpstr.append(OUTPUT_DATA_TEXT_FILE);
	
	
	cerr << "Calculating... ";
	cerr << "00 %";
	I = SIMULATION_TIME/TIME_STEP; // Final value of the counter "i".
	for (i=0; i<I; i++) {
		sys.StepEuler(TIME_STEP);
		if (i > TIME_TO_START_SAVING_DATA/TIME_STEP && (i%size_t((I-TIME_TO_START_SAVING_DATA/TIME_STEP)/N_EXPORT_POINTS) == 0) ) {
			ofile.open(gpstr.c_str(), std::fstream::app); // Opens the file for saving the simulation data, in append mode.
			ofile << std::scientific << std::setprecision(RESULTS_DIGITS); //std::setprecision(std::numeric_limits<double>::digits10); // Sets the precision for printing the doubles.
			sys.PrintTXT(ofile);
			ofile.close();
		}
		if (((i*100)/I)%PERCENTAGE_PRINT_STEP == 0)
			print_percentage(i,I);
	}
	print_percentage(i,I);
	cerr << endl;
	
	
	// -------------------------------------------------- //

	cerr << "Calling Octave script «oct_quick_plot.m»..." << endl;
	cerr << KBLU;
	system("octave oct_quick_plot_scattering.m");
	cerr << RST;
	
	system("octave oct_hacer_animacion_scattering.m");
	
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


void print_percentage(size_t i, size_t I) {
	cerr << '\b' << '\b' << '\b' << '\b';
	cerr << setfill('0') << setw(2) << (i*100)/I << " %";
}

void asd(int i) { // For debugging puurposes.
	cerr << WARNING_MSG << "llegué " << i << endl;
}
