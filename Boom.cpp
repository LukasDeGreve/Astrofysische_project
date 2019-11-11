//
//  Boom.cpp
//  
//
//  Created by Sébastien Chevalier on 11/11/2019.
//

#include "Boom.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
using namespace std;


//printen van een vector
void print(const vector<double>& v){
    for (double element : v) cout << element << ' ';
}


//initieel volume normaliseren
double V0 = 1.0;


//definieren van de Knoop-klasse
class Knoop {
    
    
    //generatie en geometrisch middelpunt
    int _g;
    vector<double> _gmp;
    
    
public:
    
    
    //default constructor
    Knoop(){
        _g = 0;
        _gmp = {0, 0, 0};
    }
    
    
    //geparametriseerde constructor
    Knoop(double generatie, vector<double> middelpunt){
        _g = generatie;
        _gmp = middelpunt;
    }
    
    
    //getters (get generatie en get geometrisch middelpunt)
    double get_g() const {return _g;}
    vector<double> get_gmp() const {return _gmp;}
    
    
    //volume knoop geven
    double volumeknoop() const {return pow(1.0/8.0,_g);}
    
    
    //lengte knoop geven
    double lengteknoop() const {return pow(pow(1.0/8.0,_g),1.0/3.0);}
};


//informatie over knoop kunnen printen
//1e getal = generatie
//2e, 3e, 4e getal geeft geometrisch middelpunt weer
//5e getal geeft volumeweer (maar die informatie zit reeds vervat in de generatie)
void printknoop(Knoop a){
    cout << a.get_g() << ' ';
    print(a.get_gmp());
    cout << a.volumeknoop() << endl;
}


//knoop kunnen splitsen in 8 knopen mogelijk maken
//a = ouderknoop
//b = vector van kinderknopen
vector<Knoop> knoopsplit(Knoop a){
    int g = a.get_g();
    vector<double> gmp = a.get_gmp();
    double l = (1.0/4.0)*a.lengteknoop();
    double x0 = gmp[0];
    double y0 = gmp[1];
    double z0 = gmp[2];
    // of vector<Knoop> b(8);
    vector<Knoop> b;
    b.resize(8);
    vector<double> coo1 = {x0 - l, y0 + l, z0 + l};
    vector<double> coo2 = {x0 + l, y0 + l, z0 + l};
    vector<double> coo3 = {x0 + l, y0 - l, z0 + l};
    vector<double> coo4 = {x0 - l, y0 - l, z0 + l};
    vector<double> coo5 = {x0 - l, y0 + l, z0 - l};
    vector<double> coo6 = {x0 + l, y0 + l, z0 - l};
    vector<double> coo7 = {x0 + l, y0 - l, z0 - l};
    vector<double> coo8 = {x0 - l, y0 - l, z0 - l};
    b[0] = Knoop(g + 1, coo1);
    b[1] = Knoop(g + 1, coo2);
    b[2] = Knoop(g + 1, coo3);
    b[3] = Knoop(g + 1, coo4);
    b[4] = Knoop(g + 1, coo5);
    b[5] = Knoop(g + 1, coo6);
    b[6] = Knoop(g + 1, coo7);
    b[7] = Knoop(g + 1, coo8);
    return b;
}


int main(){
    //maak eens drie knopen aan en print ze uit
    Knoop a(2, {1.0,2.0,3.0});
    Knoop b;
    Knoop c(1, {4.0, 2.4, 5.0});
    printknoop(a);
    printknoop(b);
    printknoop(c);
    
    //split knoop c in 8 en print de kinderen uit
    vector<Knoop> d = knoopsplit(c);
    for (int i = 0; i!=8; ++i){
        printknoop(d[i]);
    }
}
