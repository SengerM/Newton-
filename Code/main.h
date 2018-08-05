#ifndef _MAIN_H_
#define _MAIN_H_

#include "ParticleSystem.h"
#include "Particle.h"
#include "Force.h"
#include "my_forces.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <string>
#include <iomanip>
#include <iomanip>
#include <limits>
#include "simulation_macros.n++"

#define PI 3.14159265359

#define OUTPUT_DIRECTORY "resultados"
#define OUTPUT_DATA_TEXT_FILE "simulation_output.txt"
#define INITIAL_CONDITIONS_DATA_TEXT_FILE "initial_conditions.txt"

#define INIT_MSG BOLD << KCYN << "Newton++ started ----------------------------" << RST // This is the message displayed when the program is opened.
#define LAST_MSG BOLD << KCYN << "Newton++ finished ---------------------------" << RST // This message is displayed when the program is closed.

std::string now(void); // Function that returns a C++ string object containing the date and time in the format "%Y_%m_%d_%H_%M_%S".
void print_percentage(size_t i, size_t I);
void asd(int); // this is just for debugging purposes.

using namespace std;

#endif
