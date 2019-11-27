#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

//Program units: G=1

//Vector class
class Vec {
    double _x;
    double _y;
    double _z;

public:
    Vec() { _x=0; _y=0; _z=0; }
    Vec(double x, double y, double z) { _x = x; _y = y; _z=z; }

    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    double norm() const { return sqrt(_x*_x + _y*_y + _z*_z); }
    double norm2() const { return _x*_x + _y*_y + _z*_z; }
    double norm3() const { double r = sqrt(_x*_x + _y*_y + _z*_z); return r*r*r; }

    Vec& operator+=(Vec v) {
        _x += v._x;
        _y += v._y;
        _z += v._z;
        return *this;
    }

    Vec& operator-=(Vec v) {
        _x -= v._x;
        _y -= v._y;
        _z -= v._z;
        return *this;
    }

    Vec& operator*=(double s) {
        _x *= s;
        _y *= s;
        _z *= s;
        return *this;
    }

    Vec& operator/=(double s) {
        _x /= s;
        _y /= s;
        _z /= s;
        return *this;
    }
};

Vec operator+(Vec a, Vec b) { return a += b; }
Vec operator-(Vec a, Vec b) { return a -= b; }

Vec operator*(Vec a, double s) { return a *= s; }
Vec operator*(double s, Vec b) { return b *= s; }
Vec operator/(Vec a, double s) { return a /= s; }
Vec operator/(double s, Vec b) { return b /= s; }


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
int main() {

	cout << "Writing to outputfile..." << endl;

	//creating output files; one for the positions, one for the total energy
	ofstream outfile("Positions.txt");
	outfile << setprecision(12);

	ofstream outfile_E("Energy_error.txt");
	outfile_E << setprecision(12);

	//Declaration/initialisation
	const int N = 3;		//#particles
	const double h = 0.001;		//time step
	const double e = 0.;		//softening
	const double time_steps = 7000;	//amount of time steps
	double controlE = 0.;		//initial energy (initialised to zero for now)
	const int write_step = 10;	//the position of the particles will be written to a file every 10 steps

	//Declaration of position, velocity and mass variables
	vector<Vec> pos;		//position at integer time steps
	vector<Vec> pos_half;		//position at half-integer time steps
	vector<Vec> pos_half_next;	//the next position at half-integer time steps
	Vec pos_temp;			//temporary variable
		
	vector<Vec> vel;		//velocity at integer time steps
	vector<Vec> vel_next;		//the next velocity at half-integer time steps
	Vec vel_temp;			//temporary variable

	vector<double> m;		//mass of the particles
	double m_temp;			//temporary variable

	//The block of code below serves to test small N-body systems with specific initial conditions.
	//
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
	


	//Below, the initial conditions need to be loaded into the program
	//For loop over all the particles
/*
	for(int i = 0; i < N; ++i) {
		pos_temp = Vec(i*1.,0.,0.);
		vel_temp = Vec(0.,i*0.1,0.);
		m_temp = 1.;
		pos.push_back(pos_temp);
		vel.push_back(vel_temp);
		m.push_back(m_temp);
		outfile << i << ' ' <<  pos[i].x() << ' ' << pos[i].y() << ' ' << pos[i].z() << "\n";
		pos_half_next.push_back(Vec(0.,0.,0.));
		vel_next.push_back(Vec(0.,0.,0.));
	}
*/
	//Writing intial energy to text file
	controlE = E(pos, vel, m, N, e);
	outfile_E << 0. << ' ' << 0.  << "\n";
	outfile << "\n";


	//Determining position at first half step	
	//For loop over all the particles
	for (int i = 0; i < N; ++i) {
		pos_half.push_back(pos[i] + 0.5 * h * vel[i] + (h * h / 8)*acc(pos, m, i , N, e));
		outfile << i << ' ' << pos_half[i].x() << ' ' << pos_half[i].y() << ' ' << pos_half[i].z() << "\n";
	}
	outfile << "\n";


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

			//To minimize output file size and ensure that the program does not slow down due to writing too much data,
			//the following if statement is used to write the position of every particle every x amount of steps
			if (n % write_step == 0) { 
				outfile << i << ' ' <<  pos_half[i].x() << ' ' << pos_half[i].y() << ' ' << pos_half[i].z() << "\n";
			}
		}

		//Writing the time and relative energy error to another file
		outfile_E << n*h << ' ' << abs((E(pos, vel, m, N, e) - controlE)/controlE) << "\n";
		outfile << "\n";
	}

	cout << "Done writing" << endl;

	//Close files
	outfile.close();
	outfile_E.close();
}
