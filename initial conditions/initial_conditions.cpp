#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include "../Vector.cpp"
using namespace std;


int main()
{   
// declare variables
Vec pos;
Vec avg_pos;
Vec vel;
Vec avg_vel;
Vec diff;
vector<Vec> pos_list;
vector<Vec> vel_list;
double E_kin;
double E_pot;
int particles; 
string filename;
mt19937_64 rng;

// aantal deeltjes in de simulatie
cout << "How many particles do you want to generate" << endl;
cin >> particles;

// check speed
chrono::steady_clock::time_point begin = chrono::steady_clock::now();

// sets file for output
filename = "initial_conditions_" + to_string(particles) + "particles.txt";
ofstream outfile (filename);
outfile << setprecision(8);

// initialize the random number generator with time-dependent seed
uint64_t timeSeed = chrono::high_resolution_clock::now().time_since_epoch().count();
seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
rng.seed(ss);
// initialize a uniform distribution between 0 and 1
uniform_real_distribution<double> unif(0.0, 1.0);
auto random_number  = bind(unif, rng);


for (int i = 0; i < particles ; i++)
{
    // positions according to a plumber model
    double rand1 = random_number();
    double rand2 = random_number();
    double rand3 = random_number();

    double theta = acos(1 - 2 * rand1);
    double phi = 2 * M_PI * rand2;
    double radius = 3 * M_PI / (16 * sqrt(pow(rand3, -2.0/3.0) - 1.0));
    
    double x = radius * sin(theta) * cos(phi);
    double y = radius * sin(theta) * sin(phi);
    double z = radius * cos(theta);

    Vec pos(x, y, z);
    avg_pos += pos;
    pos_list.push_back(pos);

    // velocities according to an isotropic plumber model
    bool not_generated = true;

    do
    {
        double rand4 = random_number();           // q
        double rand5 = random_number();

        double p_max = 50176.0 * sqrt(2) / (19683.0 * M_PI) * rand5;
        double p = 512 / (7 * M_PI) * pow(1 - rand4 * rand4, 7.0/2.0) * rand4 * rand4;

        if ( p_max < p )
        {
            not_generated = false;
            double rand6 = random_number();
            double rand7 = random_number();

            double theta = acos(1 - 2 * rand6);
            double phi = 2 * M_PI * rand7;

            double v_esc = sqrt(2 / sqrt(pos.norm2() + 9 * M_PI / 256));
            double q_esc = v_esc * rand4;

            double v_x = q_esc * sin(theta) * cos(phi);
            double v_y = q_esc * sin(theta) * sin(phi);
            double v_z = q_esc * cos(theta);

            Vec vel(v_x, v_y, v_z);
            avg_vel += vel;
            vel_list.push_back(vel);
        }

    }
    while ( not_generated );
}

// COM-position and velocity
avg_vel /= particles;
avg_pos /= particles;

// centering: we read the list and subtract the COM-velocity and position from every point
for (size_t i=0; i!=pos_list.size(); i++)
{
    pos_list[i] -= avg_pos;
    vel_list[i] -= avg_vel;
}   

// scaling: we rescale the coordinates so that the virial theorem is true
// the total potential energy must be -1/2 and the total kinetic energy 1/4 
for (size_t i=0; i!=pos_list.size(); i++)
{
    E_kin += vel_list[i].norm2();
    if (i % 100 == 0)
    {
        cout << i << endl;
    }
    

    for (size_t j=0; j!=pos_list.size(); j++)
    {  
        if ( i != j)
        {
            diff = pos_list[i] - pos_list[j];
            E_pot += 1/(diff.norm());
        }
        
    }
}

E_kin /= 2 * particles;
E_pot /= 2 * particles * particles;

for (size_t i=0; i!=pos_list.size(); i++)
{
    pos_list[i] *= -1*2*E_pot;
    vel_list[i] /= 2*sqrt(E_kin);
}


for (size_t i=0; i!=pos_list.size(); i++)
{
    pos = pos_list[i];
    vel = vel_list[i];
    outfile << pos.out() << " " << vel.out() << endl;
}
// more speed checks
chrono::steady_clock::time_point end = chrono::steady_clock::now();
cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

return (0);
}
