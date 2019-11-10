#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

class Vec {

	double _x;
	double _y;

public:
	Vec() { _x = 0; _y = 0; }
	Vec(double x, double y) {
		_x = x; _y = y;
		}

	double x() const { return _x; }
	double y() const { return _y; }

	double norm() const {
		return sqrt(_x * _x + _y * _y);
	}

	double norm2() const {
		return _x * _x + _y * _y;
	}

	double norm3() const {
		double r = sqrt(_x * _x + _y * _y); return r * r * r;
	}

	Vec& operator+=(Vec v) {
		_x += v._x;
		_y += v._y;
		return *this;
	}
	
	Vec& operator-=(Vec v) {
		_x -= v._x;
		_y -= v._y;
		return *this;
	}


	Vec& operator*=(double s) {
		_x *= s;
		_y *= s;
		return *this;
	}

	Vec& operator/=(double s) {
		_x /= s;
		_y /= s;
		return *this;
	}

};

Vec operator+(Vec a, Vec b) { return a += b; }
Vec operator-(Vec a, Vec b) { return a -= b; }

Vec operator*(Vec a, double s) { return a *= s; }
Vec operator*(double s, Vec b) { return b *= s; }
Vec operator/(double s, Vec b) { return b /= s; }
Vec operator/(Vec a, double s) { return a /= s; }


void print(Vec a) {
	cout << a.x() << " " << a.y() << endl;
}

Vec acc(const vector<Vec>& pos, int i, int N) {
	Vec sum(0.,0.);
	for (int j = 0; j < N; ++j) {
		if (i != j) {
		sum += 0.01 * (pos[j] - pos[i])/((pos[i]-pos[j]).norm3());
		}
	}
	return sum;
}

double E(const vector<Vec>& pos, const vector<Vec>& vel, int N) {
	double Kin = 0.;
	double Pot = 0.;
	for(int i = 0; i < N; ++i) {
	Kin += 0.5*vel[i].norm2();
		for (int j = 0; j < N; ++j) {
			if (i != j) {
				Pot -= (0.5 * 0.01)/((pos[i] - pos[j]).norm());
			}
		}
	}
	return Kin + Pot;
}

int main() {

	cout << "Writing to outputfile..." << endl;

	ofstream outfile("test.txt");
	outfile << setprecision(12);

	ofstream outfile_E("test_energy.txt");
	outfile_E << setprecision(12);

	//Declaration/initialisation
	const int N = 2;
	const double h = 0.001;
	double controlE = 0.;

	vector<Vec> pos;
	vector<Vec> pos_half;
	vector<Vec> pos_half_next;
	Vec pos_temp;
	
	vector<Vec> vel;
	vector<Vec> vel_next;
	Vec vel_temp;
	
	vel.push_back(Vec(0.,0.));
	vel.push_back(Vec(-0.04,0.06));
	pos.push_back(Vec(0.,0.));
	pos.push_back(Vec(1.,0));

	//initialising positions and velocities
	for(int i = 0; i < N; ++i) {
		//pos_temp = Vec(i*1.,i*1.);
		//vel_temp = Vec(i/10.0,i/10.0);
		//pos.push_back(pos_temp);
		//vel.push_back(vel_temp);
		//outfile << i << ' ' <<  pos[i].x() << ' ' << pos[i].y() << "\n";
		pos_half_next.push_back(Vec(0.,0.));
		vel_next.push_back(Vec(0.,0.));
	}

	controlE = E(pos, vel, N);
	outfile_E << 0. << ' ' << 0.  << "\n";
	outfile << "\n";

	//Vec vcom = (snela + snelb) * 0.5;
	//posa -= vcom;
	//posb -= vcom;

	//double controlE = snelb.norm2() / 2 - mu/(posb-posa).norm() + snela.norm2() / 2;
		
	//Determining position half step	
	for (int i = 0; i < N; ++i) {
		pos_half.push_back(pos[i] + 0.5 * h * vel[i] + (h * h / 8)*acc(pos, i , N));
		outfile << i << ' ' << pos_half[i].x() << ' ' << pos_half[i].y() << "\n";
	}

	outfile << "\n";
	//calculating rest of positions and velocities
	for (int n = 0; n < 100000; ++n) {
		for (int i = 0; i < N; ++i) {
			vel_next[i] = vel[i] + h * acc(pos_half, i, N);
		}

		for (int i = 0; i < N; ++i) {
			pos_half_next[i] = pos_half[i] + h * vel_next[i];
			pos_half[i] = pos_half_next[i];
			vel[i] = vel_next[i];
			pos[i] = pos_half[i] - h / 2 * vel[i];
		outfile << i << ' ' <<  pos_half[i].x() << ' ' << pos_half[i].y() << "\n";
		}

		//double energy = snelb.norm2() / 2 - mu / (posb-posa).norm() + snela.norm2() / 2;
		//outfile << posbhalf.x() << " " << posbhalf.y() << " " << i/100 << " " << abs((energy - controlE)/controlE) << " " << posahalf.x() << " " << posahalf.y() << "\n";
		outfile_E << n*h << ' ' << abs((E(pos, vel, N) - controlE)/controlE) << "\n";
		outfile << "\n";
	}
	cout << "Done writing" << endl;
	outfile.close();
	outfile_E.close();
}
