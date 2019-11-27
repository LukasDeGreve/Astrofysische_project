//
//  tree.cpp
//  
//
//  Created by Robin
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <array>
#include <vector>
#include <cstddef>
#include <chrono>
using namespace std;

double afstand(vector<double> a, vector<double> b){
    double r = sqrt( pow(a[0]-b[0],2.0) + pow(a[1]-b[1],2.0) + pow(a[2]-b[2],2.0));
    return r;
}


class Leaf{
    
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

    vector<Leaf> split() {
        double x0 = _space[0];
        double x1 = _space[1];
        double y0 = _space[2];
        double y1 = _space[3];
        double z0 = _space[4];
        double z1 = _space[5];

        double xmid = (x0 + x1)/2;
        double ymid = (y0 + y1)/2;
        double zmid = (z0 + z1)/2;

        vector<vector<double>> v;

        Leaf I({{xmid, x1, ymid, y1, zmid, z1}}, v);
        Leaf II({{xmid, x1, y0, ymid, zmid, z1}}, v);
        Leaf III({{x0, xmid, y0, ymid, zmid, z1}}, v);
        Leaf IV({{x0, xmid, ymid, y1, zmid, z1}}, v);
        Leaf V({{xmid, x1, ymid, y1, z0, zmid}}, v);
        Leaf VI({{xmid, x1, y0, ymid, z0, zmid}}, v);
        Leaf VII({{x0, xmid, y0, ymid, z0, zmid}}, v);
        Leaf VIII({{x0, xmid, ymid, y1, z0, zmid}}, v);



        array<Leaf,8> kinderen = {{I, II, III, IV, V, VI, VII, VIII}};


        double first;
        double second;
        double thirth;
        array<double,6> ruimte;
        vector<Leaf> kids;
        
        for (Leaf kind : kinderen){
            ruimte = kind.space();
            v = kind.deeltjes();

            for (vector<double> deeltje : _deeltjes) {
                first = deeltje[0];
                second = deeltje[1];
                thirth = deeltje[2];

                if (ruimte[0] <= first && ruimte[2] <= second  && ruimte[4] <= thirth && first < ruimte[1] && second < ruimte[3] && thirth < ruimte[5]){v.push_back(deeltje);}
                
            }
            Leaf nieuwleaf(ruimte, v);
            kids.push_back(nieuwleaf);
        }
        
        
        return kids;
    }

};

void printspace(array<double,6> v) {
    cout << "<";
    for (double el : v){
        cout << el << ' ';
    }
    cout << ">" << endl;
}

//declaration for new tree node
struct node  {
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

//allocates new node
node* newNode(Leaf data) {
  // declare and allocate new node
    node* newnode = new struct node();
  
    newnode->data = data;    // Assign data to this node
  
  // Initialize left and right children as NULL
    newnode->I = NULL;
    newnode->II = NULL;
    newnode->III = NULL;
    newnode->IV = NULL;
    newnode->V = NULL;
    newnode->VI = NULL;
    newnode->VII = NULL;
    newnode->VIII = NULL;
    return newnode;
}

double totaledeeltjes = 0.0;
double deltaterm = 0.2;

//functie die totaal aantal deeltjes telt door de boom te doorlopen
void preorder2(node *root, vector<double> posdeeltje) {
    if (root == NULL) return;
    double r = afstand((root->data).center(),posdeeltje);
    double w = (root->data).width();
    cout << "w : " << w << " en r : " << r << endl;
    if (w/r < deltaterm){
        cout << "klein genoeg" << endl;
        totaledeeltjes = totaledeeltjes + ((root->data).deeltjes()).size();
        return;
    }
    if (((root->data).deeltjes()).size() == 1){
        totaledeeltjes = totaledeeltjes + ((root->data).deeltjes()).size();
    }
    preorder2(root->I, posdeeltje);
    preorder2(root->II, posdeeltje);
    preorder2(root->III, posdeeltje);
    preorder2(root->IV, posdeeltje);
    preorder2(root->V, posdeeltje);
    preorder2(root->VI, posdeeltje);
    preorder2(root->VII, posdeeltje);
    preorder2(root->VIII, posdeeltje);
}

// functie die bij het splitsen van een tak de bladeren in de juiste volgorde zet.

node* Insert(node* root,Leaf data) {
    if(root == NULL) { // empty tree
        root = newNode(data);
    }
    // choose where ot put the new node, based on the positiion of the center to the center of the parent
    else {    vector<double> deel = data.center();
        vector<double> og = root->data.center();

        if (deel[0] <= og[0]) {
            if (deel[1] <= og[1]) {
                if (deel[2] <= og[2]) {root->I = Insert(root->I,data);}
                else {root->II = Insert(root->II,data);}
            }
            else {
                if (deel[2] <= og[2]) {root->III = Insert(root->III,data);}
                else {root->IV = Insert(root->IV,data);}
            }
        }
        // else, insert in right subtree.
        else {
            if (deel[1] <= og[1]) {
                if (deel[2] <= og[2]) {root->V = Insert(root->V,data);}
                else {root->VI = Insert(root->VI,data);}
            }
            else {
                if (deel[2] <= og[2]) {root->VII = Insert(root->VII,data);}
                else {root->VIII = Insert(root->VIII,data);}
            }
        }
    }
    return root;
}


int main() {

// testdeeltjes
//vector<double> d1 = {0.5,0.2, 0.6};
//vector<double> d2 = {0.23, 0.52, 0.91};
//vector<double> d3 = { 0.57, 0.67, 0.37};

chrono::steady_clock::time_point begin = chrono::steady_clock::now();

ifstream inFile;
inFile.open("initial_conditions.txt", std::ios::in);
vector<vector<double>> deeltjes;
vector<double> deeltje;
double getal = 0.0;

while (inFile >> getal) {
    deeltje.push_back(getal);
    if (deeltje.size() == 6) {deeltjes.push_back(deeltje); deeltje = {};}
}


 /*create root node*/
double num = 100.0;

Leaf hoofdruimte({{-num, num, -num, num, -num, num}}, deeltjes);

// begin met het maken van de structure.

struct node *root = newNode(hoofdruimte);

// kinderen is een lijst met ruimtes, waar nog meer dan 1 deeltje in aanwezig zijn. Als deze lijst leeg is, dan moet er volledig gestopt worden met splitsen.

vector<Leaf> kinderen = hoofdruimte.split();
vector<Leaf> endlist;

while(kinderen.size()>1) {
// vector kinderen bevat de nog potentieel te moeten splitsen kinderleafs
    vector<Leaf> kids = kinderen;
    kinderen = {};

    for( Leaf kind : kids) {
// een link tussen ouder en kind leggen, voeg informatie aan object waarnaar rootpointers verwijst toe
        root = Insert(root, kind);
// als kindleaf slechts één deeltje meer bevat mag de splitsing stoppen en is het
// een échte leaf
        if (kind.deeltjes().size() == 1) {endlist.push_back(kind);}
// is het kind nog steeds groter dan 2, dan moet er opnieuw gesplitst worden,
// de gesplitsten worden toegevoegd aan de te onderzoeken kinderen
        if (kind.deeltjes().size() > 1) {
            vector<Leaf> newkids = kind.split();
            for (Leaf newkid : newkids) {kinderen.push_back(newkid);}
        }
    }
}

// check de controlelijst (=endlist) of er correct gesplitst is.

for (Leaf kind : endlist) {
    cout << "x: [" <<kind.space()[0] << ", " << kind.space()[1] << "] y: ["<< kind.space()[2] << ", " << kind.space()[3] << "] z: [" << kind.space()[4] << ", " << kind.space()[5] << "]" << '\t' << kind.deeltjes().size() << endl;}

cout << endlist.size() << endl;

chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

    vector<double> vbdeeltje = (root->data).deeltjes()[0];
    vector<double> startpos = {vbdeeltje[0], vbdeeltje[1], vbdeeltje[2]};
    cout << "voorbeelddeeltje : " ;
    for (double el : startpos){
        cout << " " << el;
    }
    cout << endl;
    preorder2(root, startpos);
    cout << totaledeeltjes << endl;

return 0;
}
