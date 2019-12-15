#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "Vector2.h"
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
double E(const vector<Vec>& pos, const vector<Vec>& vel, const vector<double>& m, int N, double e) {
	double Kin = 0.;
	double Pot = 0.;
	for(int i = 0; i < N; ++i) {
	Kin += 0.5*m[i]*vel[i].norm2();
		for (int j = 0; j < N; ++j) {
			if (i != j) {
				Pot -= (0.5*m[i]*m[j])/pow(e*e + (pos[i] - pos[j]).norm2(),0.5);
			}
		}
	}
	return Kin + Pot;
}

//Main program
int main(int argc, char *argv[]) {

	

	//Declaration/initialisation
	int N;				//#particles
	const double h = 0.001;		//time step
	double e;		//softening
	const double time_steps = 250;	//amount of time steps
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

	vector<double> m;		//mass of the particles	

	string file = string(argv[1]);
	//Below, the initial conditions are loaded into the program
	ifstream inFile;
	inFile.open(file,std::ios::in);
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

		//Initialising pos_half and vel_next
		pos_half_next.push_back(Vec(0.,0.,0.));
		vel_next.push_back(Vec(0.,0.,0.));
		++i;
	}

	//Automatically determine the amount of particles from the initial conditions file
	N = i;
	double m_temp = 1./N;		//temporary variable
	e =  pow(N, -0.3);

	//Initialising m vector
	for (int i = 0; i < N; ++i) {
		m.push_back(m_temp);
	}

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	//Determining position at first half step	
	//For loop over all the particles
	for (int i = 0; i < N; ++i) {
		pos_half.push_back(pos[i] + 0.5 * h * vel[i] + (h * h / 8)*acc(pos, m, i , N, e));
	}


	//Calculating the rest of the positions and velocities using the leapfrog integrator
	//For loop over all timesteps 
	for (int n = 0; n < time_steps; ++n) {
		//Calculating velocity (Acceleration calculation needs to be in a seperate for loop)
		//For loop over all particles
		for (int i = 0; i < N; ++i) {
			vel_next[i] = vel[i] + h * acc(pos_half, m, i, N, e);
		}

		//Calculating postion
		//For loop over all particles
		for (int i = 0; i < N; ++i) {
			pos_half_next[i] = pos_half[i] + h * vel_next[i];
			pos_half[i] = pos_half_next[i];
			vel[i] = vel_next[i];
			pos[i] = pos_half[i] - h / 2 * vel[i];

		}


	}

	chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
}
