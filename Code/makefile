CC = g++
CFLAGS = -ansi -pedantic -Wall -std=c++11
LFLAGS = -lm

all: separator main.o ParticleSystem.o Particle.o Vec3D.o Force.o Interaction.o my_forces.o
	$(CC) $(CFLAGS) -o newton++ *.o $(LFLAGS)


main.o: main.cpp main.h simulation_macros.n++ user_forces.n++ user_macros.n++ user_particles.n++ user_system.n++ user_functions.n++
	$(CC) $(CFLAGS) -c -o main.o main.cpp $(LFLAGS)
ParticleSystem.o: ParticleSystem.cpp ParticleSystem.h Particle.h Interaction.h
	$(CC) $(CFLAGS) -c -o ParticleSystem.o ParticleSystem.cpp $(LFLAGS)
Particle.o: Particle.cpp Particle.h Vec3D.h color.h
	$(CC) $(CFLAGS) -c -o Particle.o Particle.cpp $(LFLAGS)
Vec3D.o: Vec3D.cpp Vec3D.h
	$(CC) $(CFLAGS) -c -o Vec3D.o Vec3D.cpp $(LFLAGS)
Force.o: Force.cpp Force.h Vec3D.h Particle.h color.h
	$(CC) $(CFLAGS) -c -o Force.o Force.cpp $(LFLAGS)
Interaction.o: Interaction.cpp Interaction.h Force.h
	$(CC) $(CFLAGS) -c -o Interaction.o Interaction.cpp $(LFLAGS)
my_forces.o: my_forces.cpp my_forces.h
	$(CC) $(CFLAGS) -c -o my_forces.o my_forces.cpp $(LFLAGS)


test_Vec3D: separator test_Vec3D.o Vec3D.o
	$(CC) $(CFLAGS) -o test_Vec3D *.o $(LFLAGS)
test_Vec3D.o: test_Vec3D.cpp Vec3D.o
	$(CC) $(CFLAGS) -c -o test_Vec3D.o test_Vec3D.cpp $(LFLAGS)




separator:
	#----------------------------------------------------------------------------------------------------#
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                               SEPARADOR                                            #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#                                                                                                    #
	#----------------------------------------------------------------------------------------------------#

clean:
	rm *.o
