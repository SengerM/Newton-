#include "main.h"

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
	
	// Particles ----------------------------------------
	cerr << "Creating and configuring particles... " << endl; 
	vector<Particle> particles_vec; // This vector will contain all the particles.
	Vec3D center_of_mass_position;
	Vec3D center_of_mass_velocity; // Here I will compute the center of mass velocity.
	double total_mass = 0;
	srand(time(NULL)); // This is to initialize the random seed.
	for (i=0; i<N_PARTICLES; i++) {
		particles_vec.push_back(Particle()); // Creates the particles, the method «push_back» creates copies inside the vector.
		(particles_vec[i]).Mass() = double(random_uniform(1, 100))/100.0;
		(particles_vec[i]).Position() = Vec3D(random_uniform(-50,50), random_uniform(-50,50), random_uniform(-50,50));
		center_of_mass_position = center_of_mass_position + (particles_vec[i]).Position()*((particles_vec[i]).Mass());
		(particles_vec[i]).Velocity() = Vec3D(random_uniform(-50,50), random_uniform(-50,50), random_uniform(-50,50));
		center_of_mass_velocity = center_of_mass_velocity + (particles_vec[i]).Velocity()*((particles_vec[i]).Mass());
		total_mass += (particles_vec[i]).Mass();
	}
	center_of_mass_position = center_of_mass_position/total_mass;
	center_of_mass_velocity = center_of_mass_velocity/total_mass;
	for (i=0; i<N_PARTICLES; i++) {
		(particles_vec[i]).Position() = (particles_vec[i]).Position() - center_of_mass_position;
		(particles_vec[i]).Velocity() = (particles_vec[i]).Velocity() - center_of_mass_velocity;
	}
	// ----------------------------------------------------
	
	// FORCES ----------------------------------------- //
	cerr << "Creating and configuring forces... " << endl;
	double 	damping_coef = 10,
			repulsion_coef = 1,
			colission_radius = 0.2,
			gravitational_constant = 1,
			gravitational_radius = 0.2,
			k = 1,
			l = 0;
	Force	colission("colission force", colission_force, 3, false);
	colission.AddParameter(&colission_radius);
	colission.AddParameter(&damping_coef);
	colission.AddParameter(&repulsion_coef);
	Force	gravity("force of gravity", force_of_gravity, 2, false);
	gravity.AddParameter(&gravitational_constant);
	gravity.AddParameter(&gravitational_radius);
	Force	damping("classical damping force", damping_force, 1, false);
	damping.AddParameter(&damping_coef);
	Force	elastic("elastic force", elastic_force, 2, false);
	elastic.AddParameter(&k);
	elastic.AddParameter(&l);
	
	if (!gravity.Check() || !colission.Check() || !damping.Check() || !elastic.Check())
		cerr << ERROR_MSG << "Some forces are not ready to use." << endl;
	// ------------------------------------------------ //
	
	// System of particles ----------------------------- //
	cerr << "Creating and configuring system of particles..." << endl;
	ParticleSystem sys;
	for (i=0; i<N_PARTICLES; i++) {
		sys.AddParticle(particles_vec[i]);
	}
	for (i=0; i<N_PARTICLES; i++) {
		for (j=0; j<N_PARTICLES; j++) {
			if (j == i)
				continue;
			sys.AddInteraction(particles_vec[i], particles_vec[j], elastic);
			sys.AddInteraction(particles_vec[i], particles_vec[j], damping);
		}
	}
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
	ofile << "N_PARTICLES" << '\t' << N_PARTICLES << endl;
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


void print_percentage(size_t i, size_t I) {
	cerr << '\b' << '\b' << '\b' << '\b';
	cerr << setfill('0') << setw(2) << (i*100)/I << " %";
}

int random_uniform(int low, int high) {
    if (low > high) 
		return low + (std::rand() % (low - high + 1));
    return low + (std::rand() % (high - low + 1));
}
