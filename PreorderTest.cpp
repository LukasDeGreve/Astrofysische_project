//
//  probeersel.cpp
//  
//
//  Created by Sébastien Chevalier on 23/11/2019.
//

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <array>
#include <vector>
#include <cstddef>
#include <chrono>
using namespace std;




class Leaf {
    
    array<double,6> _space;
    vector<vector<double>> _deeltjes;

public:
    Leaf() {_space={{-1, 1, -1, 1, -1, 1}}; _deeltjes={{0, 0, 0}};}
    Leaf(array<double,6> space, vector<vector<double>> deeltjes)
        {_space = space; _deeltjes = deeltjes;}

    array<double,6> space() const {return _space;}
    vector<vector<double>> deeltjes() {return _deeltjes;}

    double width() const {double breedte = _space[1] - _space[0]; return breedte;}

    vector<double> center() const {
        double xc = (_space[0] + _space[1]) /2;
        double yc = (_space[2] + _space[3]) /2;
        double zc = (_space[4] + _space[5]) /2;
        vector<double> cen = {xc, yc, zc};
        return cen;
        }
};

double afstand(vector<double> a, vector<double> b){
    double r = sqrt( pow(a[0]-b[0],2.0) + pow(a[1]-b[1],2.0) + pow(a[2]-b[2],2.0));
    return r;
}


//knoopstructuur maken
struct node {
    Leaf data;
    struct node *I;
    struct node *II;
    struct node *III;
    struct node *IV;
    struct node *V;
    struct node *VI;
    struct node *VII;
    struct node *VIII;
};

//maak een nieuwe knoop
node* nieuweknoop(Leaf meegegevendata) {
    node *knoop = new struct node();
    knoop->data = meegegevendata;
    knoop->I = NULL;
    knoop->II = NULL;
    knoop->III = NULL;
    knoop->IV = NULL;
    knoop->V = NULL;
    knoop->VI = NULL;
    knoop->VII = NULL;
    knoop->VIII = NULL;
    return knoop;
}

void printspace(array<double,6> v) {
    cout << "<";
    for (double el : v){
        cout << el << ' ';
    }
    cout << ">" << endl;
}

void preorder(node *root) {
    if (root == NULL) return;
    printspace((root->data).space());
    preorder(root->I);
    preorder(root->II);
    preorder(root->III);
    preorder(root->IV);
    preorder(root->V);
    preorder(root->VI);
    preorder(root->VII);
    preorder(root->VIII);
}

double totaledeeltjes = 0.0;
vector<double> oorsprong = {0.0, 0.0, 0.0};
double deltaterm = 0.2;

//functie die totaal aantal deeltjes telt door de boom te doorlopen
void preorder2(node *root) {
    if (root == NULL) return;
    double r = afstand((root->data).center(),oorsprong);
    double w = (root->data).width();
    cout << "w : " << w << " en r : " << r << endl;
    if (w/r < deltaterm){
        cout << "klein genoeg" << endl;
        totaledeeltjes = totaledeeltjes + ((root->data).deeltjes()).size() ;
        return;
    }
    totaledeeltjes = totaledeeltjes + ((root->data).deeltjes()).size();
    preorder2(root->I);
    preorder2(root->II);
    preorder2(root->III);
    preorder2(root->IV);
    preorder2(root->V);
    preorder2(root->VI);
    preorder2(root->VII);
    preorder2(root->VIII);
}

//maak een boom
int main()
{
    //voorbeeld1 van randen van een knoop
    array<double,6> a = {{-2.0, -1.0, -2.0, -1.0, -2.0, -3.0}};
    //voorbeeld2 van randen van een knoop
    array<double,6> c = {{3.0, 4.2, -2.0, -1.0, 7.2, 13.0}};
    //voorbeeld3 van randen van een knoop
    array<double,6> e = {{-100.0, 100.0, -100.0, 100.0, -100.0, 100.0}};
    //voorbeeld1 van een deeltje
    vector<vector<double>> b = {{-1.5, -1.5, -2.5},{-1.2, -1.2, -2.9}};
    //voorbeeld2 van een deeltje
    vector<vector<double>> d = {{3.2, -1.8, 9.3},{3.1, -1.1, 9.1}};
    
    Leaf rootdata(a, b);
    Leaf rootdata2(c, d);
    Leaf rootdata3(e, b);
    struct node *root = nieuweknoop(rootdata3);
    root->I = nieuweknoop(rootdata2);
    root->II = nieuweknoop(rootdata);
    root->III = nieuweknoop(rootdata2);
    root->IV = nieuweknoop(rootdata2);
    root->V = nieuweknoop(rootdata2);
    root->VI = nieuweknoop(rootdata2);
    root->VII = nieuweknoop(rootdata);
    root->VIII = nieuweknoop(rootdata2);
    (root->V)->I = nieuweknoop(rootdata2);
    (root->V)->II = nieuweknoop(rootdata);
    (root->V)->III = nieuweknoop(rootdata2);
    (root->V)->IV = nieuweknoop(rootdata2);
    (root->V)->V = nieuweknoop(rootdata2);
    (root->V)->VI = nieuweknoop(rootdata2);
    (root->V)->VII = nieuweknoop(rootdata);
    (root->V)->VIII = nieuweknoop(rootdata2);
    

    //print die boom
    //preorder(root);
    preorder2(root);
    cout << totaledeeltjes << endl;
}



