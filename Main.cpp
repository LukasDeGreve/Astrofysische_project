#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <array>
#include <vector>
#include <cstddef>
#include <chrono>
#include <tuple>
#include "tree.cpp"
#include "ezETAProgressBar.hpp"
using namespace std;

int main() { 
vector<Vec> positions;
vector<Vec> velocities;
int i = 0;              // for loop variables
int j = 0;
int k = 0;
int l = 0;
int integration_time_steps = 250;
double time_step = 0.0005;
int N;                  // number of particles in the simulation
double delta = 0.8;     // Tree clustering variable
double epsilon = 0.5;       // softening parameter
vector<Vec> E_pos_list;
vector<Vec> E_vel_list;
double E_kin;
double E_pot;
double E_tot;
double E_rel;
Vec diff;


ez::ezETAProgressBar eta_time(integration_time_steps);

chrono::steady_clock::time_point begin = chrono::steady_clock::now();

ifstream inFile;
inFile.open("/home/lukasdegreve/Documents/School/Astrofysische simulaties/Astrofysische_project/initial conditions/initial_conditions_1000particles.txt",std::ios::in);
double posx,posy,posz,velx,vely,velz;
double num = 0.;

while(inFile >> posx >> posy >> posz >> velx >> vely >> velz) 
{
    //Calculate the maximum width for the total space
    if (abs(posx) > num) {num = abs(posx) + 1;}
    if (abs(posy) > num) {num = abs(posy) + 1;}
    if (abs(posz) > num) {num = abs(posz) + 1;}

    //Initialising pos and vectors
    positions.push_back(Vec(posx,posy,posz));
    velocities.push_back(Vec(velx,vely,velz));
}
N = positions.size();

// output file for plots and energy
ofstream outfile("Tree_data.txt");
outfile << setprecision(8);
ofstream outfile2("energy.txt");
outfile2 << setprecision(8);

eta_time.start();
for (int i = 0; i <= integration_time_steps; i++, ++eta_time)
{
    // create tree and return the root node
    node* tree = maketree(positions, num);
    E_vel_list.clear();
    E_pos_list.clear();

    for (j = 0; j < N; j++)
    {
        Vec acc = force(tree, positions[j], N, delta, epsilon);
        // A different integration scheme is needed for the first time step in comparison to the rest and the last
        if (i == 0)
        {
            velocities[j] += 0.5 * acc * time_step;
            positions[j] += time_step * velocities[j];
            E_pos_list.push_back(positions[j]);
            E_vel_list.push_back(velocities[j]);
        }

        else if (i == integration_time_steps)
        {
            velocities[j] += 0.5 * acc * time_step;
            E_pos_list.push_back(positions[j]);
            E_vel_list.push_back(velocities[j]);
        }
        else
        {
            velocities[j] += 0.5 * acc * time_step;
            // When calculating energy, calculate it here and not in the next step
            E_pos_list.push_back(positions[j]);
            E_vel_list.push_back(velocities[j]);

            velocities[j] += 0.5 * acc * time_step;
            positions[j] += time_step * velocities[j];
        }
        if (i % 10 == 0)
        {
            outfile << j << ' ' << positions[j].out() << endl;
            if (j == N - 1) {outfile << endl;}

        }

    }
    
    // loops for calculating the energy
    E_kin = 0;
    E_pot = 0;
    for (k=0; k != N; k++)
    {
        E_kin += E_vel_list[k].norm2();

        for (l=0; l!=N; l++)
        {  
            if ( k != l)
            {
                diff = E_pos_list[k] - E_pos_list[l];
                E_pot += 1/(diff.norm());
            }
            
        }
    }
    E_kin /= 2 * N;
    E_pot /= -2 * N * N;
    E_tot = E_pot + E_kin;
    E_rel = (E_tot + 0.25) / (-0.25) * 100;
    outfile2 << E_rel << " " << E_tot << " " << E_pot << " " << E_kin << endl;


}
chrono::steady_clock::time_point end = chrono::steady_clock::now();
cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

return 0; 

}