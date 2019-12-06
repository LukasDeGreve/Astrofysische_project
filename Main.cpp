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
int integration_time_steps = 250;
double time_step = 0.0005;
int N;                  // number of particles in the simulation
double delta = 0.8;     // Tree clustering variable
double upsilon = 0.5;       // softening parameter

ez::ezETAProgressBar eta_time(integration_time_steps);

chrono::steady_clock::time_point begin = chrono::steady_clock::now();

ifstream inFile;
inFile.open("/home/lukasdegreve/Documents/School/Astrofysische simulaties/Astrofysische_project/initial conditions/initial_conditions_collide_100particles.txt",std::ios::in);
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

// output file for plots
ofstream outfile("Tree_data.txt");
outfile << setprecision(8);

eta_time.start();
for (int i = 0; i <= integration_time_steps; i++, ++eta_time)
{
    // create tree and return the root node
    node* tree = maketree(positions, num);
    // if (i % 100 == 0){cout << i << endl;}

    //for (int j = 0; j < positions.size(); j++)

    {
        Vec acc = force(tree, positions[j], N, delta, upsilon);

        // A different integration scheme is needed for the first time step in comparison to the rest and the last
        if (i == 0)
        {
            velocities[j] += 0.5 * acc * time_step;
            positions[j] += time_step * velocities[j];
        }

        else if (i == integration_time_steps)
        {
            velocities[j] += 0.5 * acc * time_step;
        }
        else
        {
            velocities[j] += 0.5 * acc * time_step;
            // When calculating energy, put an if statement here and print the positions and velocities

            velocities[j] += 0.5 * acc * time_step;
            // putting the if statement here gives you the velocities at a different time step than the positions --> DONT DO THIS
            positions[j] += time_step * velocities[j];
        }
        if (i % 10 == 0)
        {
            outfile << j << ' ' << positions[j].out() << endl;
            if (j == positions.size() - 1) {outfile << endl;}

        }

    }

}
chrono::steady_clock::time_point end = chrono::steady_clock::now();
cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

return 0; 

}