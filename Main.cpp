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
#include "Vector.cpp"
#include "tree.cpp"
using namespace std;

int main() { 
vector<Vec> positions;
vector<Vec> velocities;
int i = 0;
int j = 0;
int integration_time_steps = 25000;
double time_step = 0.0001;

chrono::steady_clock::time_point begin = chrono::steady_clock::now();

ifstream inFile;
inFile.open("/home/lukasdegreve/Documents/School/Astrofysische simulaties/Astrofysische_project/initial conditions/initial_conditions_100particles.txt",std::ios::in);
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
for (int i = 0; i <= integration_time_steps; i++)
{
    // create tree and return the root node
    node* tree = maketree(positions, num);
    if (i % 100 == 0){cout << i << endl;}

    for (int j = 0; j < positions.size(); j++)
    {
        Vec acc = force(tree, positions[j]);

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
            // putting the if statement here gives you the velocities at a different time step than the positions
            positions[j] += time_step * velocities[j];
        }
    }

}
cout << "position 1" << positions[0].out() << endl;
chrono::steady_clock::time_point end = chrono::steady_clock::now();
cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

return 0; 
}