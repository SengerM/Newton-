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
	vector<Particle> 	layer1,
						layer2,
						layer3;
	for (i=0; i<N_PARTICLES_PER_LAYER; i++) {
		layer1.push_back(Particle("I am in layer 1")); // Creates the particles, the method «push_back» creates copies inside the vector.
		(layer1[i]).Mass() = 1;
		(layer1[i]).Position() = Vec3D(0, double(i)*double(OBJECTIVE_PARTICLES_RADIUS)-double(N_PARTICLES_PER_LAYER-1)/2.0, 0);
		
		layer2.push_back(Particle("I am in layer 2"));
		(layer2[i]).Mass() = 1;
		(layer2[i]).Position() = Vec3D(OBJECTIVE_PARTICLES_RADIUS*sqrt(3)/2, double(i)*double(OBJECTIVE_PARTICLES_RADIUS)-double(N_PARTICLES_PER_LAYER-1)/2.0+double(OBJECTIVE_PARTICLES_RADIUS)/2.0, 0);
		
		layer3.push_back(Particle("I am in layer 3"));
		(layer3[i]).Mass() = 1;
		(layer3[i]).Position() = Vec3D(OBJECTIVE_PARTICLES_RADIUS*sqrt(3), double(i)*double(OBJECTIVE_PARTICLES_RADIUS)-double(N_PARTICLES_PER_LAYER-1)/2.0, 0);
	}
	Particle projectile("projectile"); // This is the particle that is going to be shooted.
	projectile.Mass() = 0.1;
	srand(time(NULL)); // This is to initialize the random seed.
	projectile.Position() = Vec3D(-OBJECTIVE_PARTICLES_RADIUS*4 ,random_uniform(-OBJECTIVE_PARTICLES_RADIUS*100,OBJECTIVE_PARTICLES_RADIUS*100)/100.0, 0);
	projectile.Velocity() = Vec3D(200, 0, 0);
	// ----------------------------------------------------
	
	// FORCES ----------------------------------------- //
	cerr << "Creating and configuring forces... " << endl;
	
	Force	colission("colission force between projectile and objective", colission_force, 3, false);
	double	repulsion_coef = 1000, // Projectile
			dissipation_coef = 0, // Projectile
			colission_radius = double(OBJECTIVE_PARTICLES_RADIUS)/1; // Projectile
	colission.AddParameter(&colission_radius);
	colission.AddParameter(&dissipation_coef);
	colission.AddParameter(&repulsion_coef);
	
	Force	damping("damping force of objective particles", damping_force, 1, false);
	double 	damping_coef = 0; // Objective
	damping.AddParameter(&damping_coef);
	
	Force	bonding("bonding force of objective particles", elastic_force, 2, false);
	double 	k = 10000, // Objective
			l = OBJECTIVE_PARTICLES_RADIUS; // Objective
	bonding.AddParameter(&k);
	bonding.AddParameter(&l);
	
	Force	fixing1("objective fixing force at y = 0", armonic_3D_field, 3, true);
	double	k_fixing = 1e5,
			l_fixing = 0;
	Vec3D	fixing_point_1(layer1[0].Position());
	fixing1.AddParameter(&k_fixing);
	fixing1.AddParameter(&l_fixing);
	fixing1.AddParameter(&fixing_point_1);
	
	Force	fixing2("objective fixing force at y = N_PARTICLES", armonic_3D_field, 3, true);
	Vec3D	fixing_point_2(layer1[N_PARTICLES_PER_LAYER-1].Position());
	fixing2.AddParameter(&k_fixing);
	fixing2.AddParameter(&l_fixing);
	fixing2.AddParameter(&fixing_point_2);
	
	Force	viscous("dissipating force in the fixings of the objective", viscous_field, 1, true);
	double	viscous_fixing_constant = 0;
	viscous.AddParameter(&viscous_fixing_constant);
	// ------------------------------------------------ //
	
	// System of particles ----------------------------- //
	cerr << "Creating and configuring system of particles..." << endl;
	ParticleSystem sys;
	for (i=0; i<N_PARTICLES_PER_LAYER; i++) {
		sys.AddParticle(layer1[i]);
		sys.AddParticle(layer2[i]);
		sys.AddParticle(layer3[i]);
	}
	sys.AddParticle(projectile);
	for (i=0; i<N_PARTICLES_PER_LAYER; i++) {
		sys.AddInteraction(projectile, layer1[i], colission);
		sys.AddInteraction(layer1[i], projectile, colission);
		
		sys.AddInteraction(projectile, layer2[i], colission);
		sys.AddInteraction(layer2[i], projectile, colission);
		
		sys.AddInteraction(projectile, layer3[i], colission);
		sys.AddInteraction(layer3[i], projectile, colission);
		
		if (i != N_PARTICLES_PER_LAYER-1) { // Interactions within each layer.
			sys.AddInteraction(layer1[i], layer1[i+1], bonding);
			sys.AddInteraction(layer1[i+1], layer1[i], bonding);
			sys.AddInteraction(layer1[i], layer1[i+1], damping);
			sys.AddInteraction(layer1[i+1], layer1[i], damping);
			
			sys.AddInteraction(layer2[i], layer2[i+1], bonding);
			sys.AddInteraction(layer2[i+1], layer2[i], bonding);
			sys.AddInteraction(layer2[i], layer2[i+1], damping);
			sys.AddInteraction(layer2[i+1], layer2[i], damping);
			
			sys.AddInteraction(layer3[i], layer3[i+1], bonding);
			sys.AddInteraction(layer3[i+1], layer3[i], bonding);
			sys.AddInteraction(layer3[i], layer3[i+1], damping);
			sys.AddInteraction(layer3[i+1], layer3[i], damping);
		}
		if (i != N_PARTICLES_PER_LAYER - 1) {
			sys.AddInteraction(layer1[i], layer2[i], bonding);
			sys.AddInteraction(layer2[i], layer1[i], bonding);
			sys.AddInteraction(layer1[i], layer2[i], damping);
			sys.AddInteraction(layer2[i], layer1[i], damping);
			
			sys.AddInteraction(layer2[i], layer3[i], bonding);
			sys.AddInteraction(layer3[i], layer2[i], bonding);
			sys.AddInteraction(layer2[i], layer3[i], damping);
			sys.AddInteraction(layer3[i], layer2[i], damping);
			
			sys.AddInteraction(layer1[i+1], layer2[i], bonding);
			sys.AddInteraction(layer2[i], layer1[i+1], bonding);
			sys.AddInteraction(layer1[i+1], layer2[i], damping);
			sys.AddInteraction(layer2[i], layer1[i+1], damping);
			
			sys.AddInteraction(layer2[i], layer3[i+1], bonding);
			sys.AddInteraction(layer3[i+1], layer2[i], bonding);
			sys.AddInteraction(layer2[i], layer3[i+1], damping);
			sys.AddInteraction(layer3[i+1], layer2[i], damping);
		}
	}
	sys.AddInteraction(layer1[0], fixing1);
	sys.AddInteraction(layer1[0], viscous);
	sys.AddInteraction(layer1[N_PARTICLES_PER_LAYER-1], fixing2);
	sys.AddInteraction(layer1[N_PARTICLES_PER_LAYER-1], viscous);
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
