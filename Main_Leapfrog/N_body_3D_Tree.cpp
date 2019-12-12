#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
//#include "Vector2.h"
#include "tree.h"
#include "ezETAProgressBar.hpp"
using namespace std;

//Program units: G=1
//Define functions
//Calculating acceleration
Vec acc(const vector<Vec>& pos, const vector<double>& m, int i, int N, double e) {
	Vec sum(0.,0.,0.);
	for (int j = 0; j < N; ++j) {
		if (i != j) {
		sum += m[j] * (pos[j] - pos[i])/pow(e*e + (pos[i]-pos[j]).norm2(), 1.5);
		}
	}
	return sum;
}

//Calculating total energy
double E(const vector<Vec>& pos, const vector<Vec>& vel, int N, double e) {
	double Kin = 0.;
	double Pot = 0.;
	for(int i = 0; i < N; ++i) {
	Kin += (0.5*vel[i].norm2())/N;
		for (int j = 0; j < N; ++j) {
			if (i != j) {
				Pot -= 0.5/(pow(e*e + (pos[i] - pos[j]).norm2(),0.5)*N*N);
			}
		}
	}
	return Kin + Pot;
}

//Main program
int main() {

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	cout << "Writing to outputfile..." << endl;

	//creating output files; one for the positions, one for the total energy
	ofstream outfile("Positions100.txt");
	outfile << setprecision(12);

	ofstream outfile_E("Energy_error100.txt");
	outfile_E << setprecision(12);

	//Declaration/initialisation
	int N;				//#particles
	const double h = 0.001;		//time step
	const double e = 0.5;		//softening
	const double delta = 0.25;
	const double time_steps = 25000;	//amount of time steps
	double controlE = 0.;		//initial energy (initialised to zero for now)
	const int write_step = 20;	//the position of the particles will be written to a file every 10 steps

	//Declaration of position, velocity and mass variables
	vector<Vec> pos;		//position at integer time steps
	vector<Vec> pos_half;		//position at half-integer time steps
	vector<Vec> pos_half_next;	//the next position at half-integer time steps
	Vec pos_temp;			//temporary variable
		
	vector<Vec> vel;		//velocity at integer time steps
	vector<Vec> vel_next;		//the next velocity at half-integer time steps
	Vec vel_temp;			//temporary variable

	ez::ezETAProgressBar eta_time(time_steps);

	//The block of code below serves to test small N-body systems with specific initial conditions.
	//
/*
	vel.push_back(Vec(0.4662036850,0.4323657300,0.));
	vel.push_back(Vec(-0.93240737,-0.86473146,0.));
	vel.push_back(Vec(0.4662036850,0.4323657300,0.));
	pos.push_back(Vec(-.97000436,0.24308753,0.));
	pos.push_back(Vec(0.,0.,0.));
	pos.push_back(Vec(0.97000436,-0.24308753,0.));
	
	m.push_back(1.);
	m.push_back(1.);
	m.push_back(1.);
	
	for(int i = 0; i < N; ++i) {
		outfile << i << ' ' <<  pos[i].x() << ' ' << pos[i].y() << ' ' << pos[i].z() << "\n";
		pos_half_next.push_back(Vec(0.,0.,0.));
		vel_next.push_back(Vec(0.,0.,0.));
	}
	//
*/	


	//Below, the initial conditions are loaded into the program
	ifstream inFile;
	inFile.open("/home/wout/Documents/Astro_sim/Project/N_body_3D_Tree/initial_conditions_100particles.txt",std::ios::in);
	double posx,posy,posz,velx,vely,velz;
	int i = 0;
	double num = 0.;

	while(inFile >> posx >> posy >> posz >> velx >> vely >> velz) {
		//Calculate the maximum width for the total space
		if (abs(posx) > num) {num = abs(posx) + 1;}
		if (abs(posy) > num) {num = abs(posy) + 1;}
		if (abs(posz) > num) {num = abs(posz) + 1;}

		//Initialising pos and vectors
		pos.push_back(Vec(posx,posy,posz));
		vel.push_back(Vec(velx,vely,velz));

		//Writing positions to file
		outfile << i << ' ' <<  posx << ' ' << posy << ' ' << posz << "\n";

		//Initialising pos_half and vel_next
		pos_half_next.push_back(Vec(0.,0.,0.));
		vel_next.push_back(Vec(0.,0.,0.));
		++i;
	}

	//Automatically determine the amount of particles from the initial conditions file
	N = i;

	//Writing intial energy to text file
	controlE = E(pos, vel, N, e);
	outfile_E << 0. << ' ' << 0.  << "\n";
	outfile << "\n";


	//Determining position at first half step	
	//For loop over all the particles

	//Make tree
	node* tree = maketree(pos, num);
	for (int i = 0; i < N; ++i) {
		pos_half.push_back(pos[i] + 0.5 * h * vel[i] + (h * h / 8)*force(tree,pos[i],N,delta,e));
		outfile << i << ' ' << pos_half[i].x() << ' ' << pos_half[i].y() << ' ' << pos_half[i].z() << "\n";
	}
	outfile << "\n";


	//Calculating the rest of the positions and velocities using the leapfrog integrator
	//For loop over all timesteps 
	eta_time.start();
	for (int n = 0; n < time_steps; ++n, ++eta_time) {
		//Make tree
		node* tree = maketree(pos_half, num);
		//if (n % write_step == 0) {
		//	cout << "time step  " << n << "/" << to_string(time_steps) << endl;
		//}
		//Calculating velocity (Acceleration calculation needs to be in a seperate for loop)
		//For loop over all particles
		for (int i = 0; i < N; ++i) {
			vel_next[i] = vel[i] + h * force(tree,pos_half[i], N, delta, e);
		}

		//Calculating postion
		//For loop over all particles
		for (int i = 0; i < N; ++i) {
			pos_half_next[i] = pos_half[i] + h * vel_next[i];
			pos_half[i] = pos_half_next[i];
			vel[i] = vel_next[i];
			pos[i] = pos_half[i] - h / 2 * vel[i];

			//To minimize output file size and ensure that the program does not slow down due to writing too much data,
			//the following if statement is used to write the position of every particle every x amount of steps
			if (n % write_step == 0) { 
				outfile << i << ' ' <<  pos_half[i].x() << ' ' << pos_half[i].y() << ' ' << pos_half[i].z() << "\n";
			}
		}
		
		if (n % write_step == 0) { 
			outfile << "\n";
		}

		//Writing the time and relative energy error to another file
		if (n % write_step == 0) { 
			outfile_E << n*h << ' ' << abs((E(pos, vel, N, e) - controlE)/controlE) << "\n";
		}
	}

	cout << "Done writing" << endl;

	//Close files
	outfile.close();
	outfile_E.close();

	chrono::steady_clock::time_point end = chrono::steady_clock::now();
    	cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() * 0.001 << "s" << endl;
}
