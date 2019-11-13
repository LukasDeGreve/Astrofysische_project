#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

//Program units: G=1

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

    string out() {return to_string(x()) + ' ' + to_string(y()) + ' ' + to_string(z());}

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

// Vector-vector and vector-scalar operations
Vec operator+(Vec a, Vec b) { return a += b; }
Vec operator-(Vec a, Vec b) { return a -= b; }

Vec operator*(Vec a, double s) { return a *= s; }
Vec operator*(double s, Vec b) { return b *= s; }
Vec operator/(Vec a, double s) { return a /= s; }
Vec operator/(double s, Vec b) { return b /= s; }


void print(Vec a) {
	cout << a.x() << " " << a.y() << " " << a.z() << endl;
}

Vec acc(const vector<Vec>& pos, const vector<double>& m, int i, int N, double e) {
	Vec sum(0.,0.,0.);
	for (int j = 0; j < N; ++j) {
		if (i != j) {
		sum += m[j] * (pos[j] - pos[i])/pow(e*e + (pos[i]-pos[j]).norm2(), 1.5);
		}
	}
	return sum;
}

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

int main() {

	cout << "Writing to outputfile..." << endl;

	ofstream outfile("test.txt");
	outfile << setprecision(12);

	ofstream outfile_E("test_energy.txt");
	outfile_E << setprecision(12);

	//Declaration/initialisation
	const int N = 3;
	const double h = 0.001;
	const double e = 0.5;
	double controlE = 0.;

	vector<Vec> pos;
	vector<Vec> pos_half;
	vector<Vec> pos_half_next;
	Vec pos_temp;
	
	vector<Vec> vel;
	vector<Vec> vel_next;
	Vec vel_temp;

	vector<double> m;
	
	vel.push_back(Vec(0.,0.,0.));
	vel.push_back(Vec(0.,0.,0.));
	vel.push_back(Vec(0.,0.,0.));
	pos.push_back(Vec(1.,3.,0.));
	pos.push_back(Vec(-2.,-1,0.));
	pos.push_back(Vec(1.,-1,0.));
	
	m.push_back(3);
	m.push_back(4);
	m.push_back(5);

	//initialising positions and velocities
	for(int i = 0; i < N; ++i) {
		//pos_temp = Vec(i*1.,i*1.);
		//vel_temp = Vec(i/10.0,i/10.0);
		//pos.push_back(pos_temp);
		//vel.push_back(vel_temp);
		//outfile << i << ' ' <<  pos[i].x() << ' ' << pos[i].y() << "\n";
		pos_half_next.push_back(Vec(0.,0.,0.));
		vel_next.push_back(Vec(0.,0.,0.));
	}

	controlE = E(pos, vel, m, N, e);
	outfile_E << 0. << ' ' << 0.  << "\n";
	outfile << "\n";

	//Vec vcom = (snela + snelb) * 0.5;
	//posa -= vcom;
	//posb -= vcom;

	//Determining position half step	
	for (int i = 0; i < N; ++i) {
		pos_half.push_back(pos[i] + 0.5 * h * vel[i] + (h * h / 8)*acc(pos, m, i , N, e));
		outfile << i << ' ' << pos_half[i].x() << ' ' << pos_half[i].y() << ' ' << pos_half[i].z() << "\n";
	}

	outfile << "\n";
	//calculating rest of positions and velocities
	for (int n = 0; n < 50000; ++n) {
		for (int i = 0; i < N; ++i) {
			vel_next[i] = vel[i] + h * acc(pos_half, m, i, N, e);
		}

		for (int i = 0; i < N; ++i) {
			pos_half_next[i] = pos_half[i] + h * vel_next[i];
			pos_half[i] = pos_half_next[i];
			vel[i] = vel_next[i];
			pos[i] = pos_half[i] - h / 2 * vel[i];
		outfile << i << ' ' <<  pos_half[i].x() << ' ' << pos_half[i].y() << ' ' << pos_half[i].z() << "\n";
		}

		outfile_E << n*h << ' ' << abs((E(pos, vel, m, N, e) - controlE)/controlE) << "\n";
		outfile << "\n";
	}
	cout << "Done writing" << endl;
	outfile.close();
	outfile_E.close();
}
