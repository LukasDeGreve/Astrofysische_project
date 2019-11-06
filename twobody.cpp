#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

class Vec {

	double _x;
	double _y;

public:
	Vec() { _x = 0; _y = 0; }
	Vec(double x, double y)
		{
			_x = x; _y = y;
		}

	double x() const { return _x; }
	double y() const { return _y; }

	double norm() const
	{
		return sqrt(_x * _x + _y * _y);
	}

	double norm2() const
	{
		return _x * _x + _y * _y;
	}

	double norm3() const
	{
		double r = sqrt(_x * _x + _y * _y); return r * r * r;
	}

	Vec& operator+=(Vec v)
	{
		_x += v._x;
		_y += v._y;
		return *this;
	}
	
	Vec& operator-=(Vec v)
	{
		_x -= v._x;
		_y -= v._y;
		return *this;
	}


	Vec& operator*=(double s)
	{
		_x *= s;
		_y *= s;
		return *this;
	}

	Vec& operator/=(double s)
	{
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

void print(Vec a)
{
	cout << a.x() << " " << a.y() << endl;
}

Vec acc(Vec r) 
{
	double mu = 0.01;
	return - (mu / r.norm3()) * r;
}

int main()
{
	cout << "Writing to outputfile..." << endl;

	ofstream outfile("banen.txt");
	outfile << setprecision(12);

	Vec posa(0., 0.);
	Vec posb(1., 0.);
	Vec snela(0., 0.);
	Vec snelb(-0.04, 0.06);
	Vec vcom = (snela + snelb) * 0.5;
	posa -= vcom;
	posb -= vcom;

	double h = 0.001;
	double mu = 0.01;
	double controlE = snelb.norm2() / 2 - mu/(posb-posa).norm() + snela.norm2() / 2;
	
	// doe halve stap
	//Vec posbhalf(0., 0.);  // gebruik als 1 lichaam beweegt
	Vec posahalf = posa + 0.5 * h * snela + (h * h / 8) * acc(posa - posb); // gebruuik als 2 lichamen bewegen
	Vec posbhalf = posb + 0.5 * h * snelb + (h * h / 8) * acc(posb - posa);
	// loop over alle stappen

	for (int i = 0; i <= 100 / h; ++i)
	{
		Vec snelanext = snela + h * acc(posahalf - posbhalf);
		Vec snelbnext = snelb + h * acc(posbhalf - posahalf);

		Vec posahalfnext = posahalf + h * snelanext;
		Vec posbhalfnext = posbhalf + h * snelbnext;

		posahalf = posahalfnext;
		posbhalf = posbhalfnext;

		snela = snelanext;
		snelb = snelbnext;

		posb = posbhalf - h / 2 * snelb;
		posa = posahalf - h / 2 * snela;
		
		double energy = snelb.norm2() / 2 - mu / (posb-posa).norm() + snela.norm2() / 2;
		
		outfile << posbhalf.x() << " " << posbhalf.y() << " " << i/100 << " " << abs((energy - controlE)/controlE) << " " << posahalf.x() << " " << posahalf.y() << "\n";

	}
	
	cout << "Done writing" << endl;
	outfile.close();
}
