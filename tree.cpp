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
using namespace std;


class Leaf{
	
	array<double,6> _space;
	vector<Vec> _deeltjes;

public:
	Leaf() {_space={{-1, 1, -1, 1, -1, 1}}; _deeltjes={Vec()};}
	Leaf(array<double,6> space, vector<Vec> deeltjes)
		{_space = space; _deeltjes = deeltjes;}

	array<double,6> space() const {return _space;}
	vector<Vec> deeltjes() {return _deeltjes;}

	double width() const {double breedte = _space[1] - _space[0]; return breedte;}

	vector<double> center() const {
		double xc = (_space[0] + _space[1]) /2;
		double yc = (_space[2] + _space[3]) /2;
		double zc = (_space[4] + _space[5]) /2;
		vector<double> cen = {xc, yc, zc};
		return cen;
		}

	Vec com() const {
		double x = 0;
		double y = 0;
		double z = 0;
		for (Vec deeltje : _deeltjes) { x += deeltje.x(); y += deeltje.y(); z += deeltje.z();}
		double len = _deeltjes.size();
		x /= len;
		y /= len;
		z /= len;
		Vec c(x, y, z);
		return c;
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

		vector<Vec> v;  //vecor van posities

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

			for (Vec deeltje : _deeltjes) {
				first = deeltje.x();
				second = deeltje.y();
				thirth = deeltje.z();

				if (ruimte[0] <= first && ruimte[2] <= second  && ruimte[4] <= thirth && first < ruimte[1] && second < ruimte[3] && thirth < ruimte[5]){v.push_back(deeltje);}
				
			}
			Leaf nieuwleaf(ruimte, v);
			kids.push_back(nieuwleaf);
		}
		
		
		return kids;
	}

};

//declaration for new tree node: onthou de COM en de width.
struct node  { 
tuple<Vec, vector<double>, double, double> data; 
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
node* newNode(tuple<Vec, vector<double>, double, double> data) { 
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

// functie die bij het splitsen van een tak de bladeren in de juiste volgorde zet.

node* Insert(node* root,Leaf data) {
	if(root == NULL) { // empty tree
		if (data.deeltjes().size() == 0) {}
		else {
			tuple<Vec, vector<double>, double, double> tup;
			tup = std::make_tuple(data.com(), data.center(), data.width(), data.deeltjes().size());
			root = newNode(tup);
		}
	}
	// choose where ot put the new node, based on the positiion of the center to the center of the parent
	else {	vector<double> deel = data.center();
		vector<double> og = get<1>(root->data);

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

node* maketree(vector<Vec> deeltjes, double num) {
	
	Leaf hoofdruimte({{-num, num, -num, num, -num, num}}, deeltjes);
	tuple<Vec, vector<double>, double, double> tup;
	tup = std::make_tuple(hoofdruimte.com(), hoofdruimte.center(), hoofdruimte.width(), hoofdruimte.deeltjes().size()); // deze tuple is de node van de tree

// begin met het maken van de structure. 

	struct node *root = newNode(tup);

	// kinderen is een lijst met ruimtes, waar nog meer dan 1 deeltje in aanwezig zijn. Als deze lijst leeg is, dan moet er volledig gestopt worden met splitsen.

	vector<Leaf> kinderen = hoofdruimte.split();
	//vector<Leaf> endlist;

	while(kinderen.size()>1) {

		vector<Leaf> kids = kinderen;
		kinderen = {};

		for( Leaf kind : kids) {
	// een link tussen ouder en kind leggen
			root = Insert(root, kind);

	// deze lijn is om te kijken of het programma correct stopt met splitsen.
			//if (kind.deeltjes().size() == 1) {endlist.push_back(kind);}

	// is het kind nog steeds groter dan 2, dan moet er opnieuw gesplitst worden, dus terug naar de lijst kinderen.
			if (kind.deeltjes().size() > 1) {
				vector<Leaf> newkids = kind.split();
				for (Leaf newkid : newkids) {kinderen.push_back(newkid);}
			}
		}
	}
	return root;
}

// functie om de tree eens te overlopen ter controle deze print de centers van alle kubusjes.


node* overloop(node* tree) {
	vector<double> cen = get<1>(tree->data);
	cout << get<2>(tree->data) << " " << cen[0] << " " << cen[1] << " " << cen[2] << " " << endl;
	if (tree->I != NULL){
		overloop(tree->I);
	}
	if (tree->II != NULL){
		overloop(tree->II);
	}
	if (tree->III != NULL){
		overloop(tree->III);
	}
	if (tree->IV != NULL){
		overloop(tree->IV);
	}
	if (tree->V != NULL){
		overloop(tree->V);
	}
	if (tree->VI != NULL){
		overloop(tree->VI);
	}
	if (tree->VII != NULL){
		overloop(tree->VII);
	}
	if (tree->VIII != NULL){
		overloop(tree->VIII);
	}
	return tree;
}


// functie om de kracht op een deeltje te berekenen. deze geeft een som terug van alle krachten die op het bepaalde deeltje inwerken.

Vec force(node *root, Vec deeltje) {
	Vec kracht(0, 0, 0);
	
	if (root == NULL) {kracht = Vec(0, 0, 0);}
	else {
		Vec afst = get<0>(root->data) - deeltje;
		if (afst.x() == 0 && afst.y() == 0 && afst.z() == 0) {kracht = Vec(0, 0, 0);}
		else {
			double delta = 1;
			if (get<2>(root->data) / afst.norm() < delta) {

				kracht += force(root->I, deeltje);
				kracht += force(root->II, deeltje);
				kracht += force(root->III, deeltje);
				kracht += force(root->IV, deeltje);
				kracht += force(root->V, deeltje);
				kracht += force(root->VI, deeltje);
				kracht += force(root->VII, deeltje);
				kracht += force(root->VIII, deeltje);
			}
			else {
				kracht = get<3>(root->data) * (1 / afst.norm3()) * afst; // get<3>(root->data) is totaal aantal deeltjes
			}
		}
	}
	return kracht;
}


int main() { 



chrono::steady_clock::time_point begin = chrono::steady_clock::now();

ifstream inFile;
inFile.open("initial_conditions.txt", std::ios::in);

// initialiseer variabelen die nodig zijn voor txt in te lezen

vector<Vec> deeltjes;
Vec deeltje(0, 0, 0);
double getal = 0.0;
double num = 0.0;
int teller = 0;
vector<double> testvector = {};

// inlezen van txt-file, classificeren van de posities in Vec's en bepalen hoe groot de initiele doos moet zijn.

while (inFile >> getal) {
	if (teller % 6 <= 2) {
		if (abs(getal) > num) {num = abs(getal) + 1;}

		testvector.push_back(getal);
	}
	
	if (testvector.size() == 3) {
		deeltje = Vec(testvector[0], testvector[1], testvector[2]);
		deeltjes.push_back(deeltje);
		testvector = {};
		
	}
	teller += 1;
}


//create tree and return the root node


node* tree = maketree(deeltjes, num);

// kies een deeltje en bereken de kracht hierop

deeltje = deeltjes[227];
Vec kracht = force(tree, deeltje);

cout << kracht.x() << " " << kracht.y() << " " << kracht.z() << " " << endl;






// check de controlelijst (=endlist) of er correct gesplitst is. 

/*
for (Leaf kind : endlist) {
	cout << "x: [" <<kind.space()[0] << ", " << kind.space()[1] << "] y: ["<< kind.space()[2] << ", " << kind.space()[3] << "] z: [" << kind.space()[4] << ", " << kind.space()[5] << "]" << '\t' << kind.deeltjes().size() << endl;}
*/
chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

return 0; 
}
