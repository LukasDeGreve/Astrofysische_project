#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Vector.cpp"
using namespace std;

Vec acc(Vec pos1, Vec pos2)
{
    Vec diff = pos2 - pos1;
    return diff / (diff.norm3());
}

Vec step1(Vec vel1, Vec acc, double h)     // eq 5.10a in syllabus, h denotes the time step
{
    return vel1 + 0.5 * h * acc;
}

Vec step2(Vec pos1, Vec vel1, double h)    // eq 5.10b in syllabus, h denotes the time step
{
    return pos1 + h * vel1;
}

Vec step3(Vec vel1, Vec acc, double h)     // eq 5.10c in syllabus, h denotes the time step
{
    return vel1 + 0.5 * h * acc;
}
