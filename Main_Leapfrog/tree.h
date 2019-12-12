#ifndef TREE_H
#define TREE_H

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
#include "Vector2.h"
using namespace std;

// maak een klasse die ruimte bezit met alle deeltjes die in de ruimte zitten

class Leaf{
	
	array<double,6> _space; // space = {{-x, x, -y, y, -z, z}}
	vector<Vec> _deeltjes; // lijst van alle posities van elk deeltje

public:
	Leaf() {_space={{-1, 1, -1, 1, -1, 1}}; _deeltjes={Vec()};}
	Leaf(array<double,6> space, vector<Vec> deeltjes)
		{_space = space; _deeltjes = deeltjes;}

	array<double,6> space() const {return _space;}
	vector<Vec> deeltjes() {return _deeltjes;}

	double width() const {double breedte = _space[1] - _space[0]; return breedte;}

	// center is het geometrisch centrum van het ruimteblokje

	vector<double> center() const {
		double xc = (_space[0] + _space[1]) /2;
		double yc = (_space[2] + _space[3]) /2;
		double zc = (_space[4] + _space[5]) /2;
		vector<double> cen = {xc, yc, zc};
		return cen;
		}

	// com is het center of mass van de ruimte

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

	// deze functie split, split de ruimte op in octanten en bekijkt welke deeltjes verplaatst moeten worden naar elk octant

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

		// stopde kinderen van de ruimte in de array

		array<Leaf,8> kinderen = {{I, II, III, IV, V, VI, VII, VIII}};

		// kijken welk deeltje naar welke ruimte moet.

		double first;
		double second;
		double thirth;
		array<double,6> ruimte;
		vector<Leaf> kids;
		
		for (Leaf kind : kinderen){ // overlopen van de ruimten
			ruimte = kind.space();
			v = kind.deeltjes();

			for (Vec deeltje : _deeltjes) { // overlopen van de deeltjes
				first = deeltje.x();
				second = deeltje.y();
				thirth = deeltje.z();
				
				//  
				
				if (ruimte[0] <= first && ruimte[2] <= second  && ruimte[4] <= thirth && first < ruimte[1] && second < ruimte[3] && thirth < ruimte[5]){v.push_back(deeltje);}
				
			}
			Leaf nieuwleaf(ruimte, v);
			kids.push_back(nieuwleaf);
		}
		
		
		return kids;
	}

};

//declaration for new tree node: onthou de COM, geometrisch center de width en het aantal deeltjes.
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

//allocates new node. maakt een nieuwe node met de informatie en initialiseert de 8 subnodes
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

// functie die bij het splitsen van een tak de bladeren in de juiste volgorde zet. Dit gebeurt op basis van de posities van de geometrische centrums van de ruimten.
// dit is een uitbreiding op de binary tree 

node* Insert(node* root,Leaf data) {
	if(root == NULL) { // als de node leeg is, dan initialiseer je een nieuwe node. 
		if (data.deeltjes().size() == 0) {} // als er geen deeltjes in de ruimte zitten, moet er niets gebeuren.
		else {
			tuple<Vec, vector<double>, double, double> tup;
			tup = std::make_tuple(data.com(), data.center(), data.width(), data.deeltjes().size());
			root = newNode(tup);
		}
	}
	// choose where ot put the new node, based on the positiion of the center to the center of the parent
	else {	vector<double> deel = data.center(); // deel is het geometrisch centrum van het kind
		vector<double> og = get<1>(root->data); // og is het geometrisch centrum van de ouder

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

// deze functie maaktde hierarchische boom voor een bepaalde tijdsstap. 'num' is een waarde groter dan de verste positie van alle deeltjes

node* maketree(vector<Vec> deeltjes, double num) {
	
	// maak de initiele ruimte waar alle deeltjes in zitten
	
	Leaf hoofdruimte({{-num, num, -num, num, -num, num}}, deeltjes);
	tuple<Vec, vector<double>, double, double> tup;
	tup = std::make_tuple(hoofdruimte.com(), hoofdruimte.center(), hoofdruimte.width(), hoofdruimte.deeltjes().size()); // deze tuple is de node van de tree

// begin met het maken van de structure. 

	struct node *root = newNode(tup);

	// kinderen is een lijst met ruimtes, waar nog meer dan 1 deeltje in aanwezig zijn. Als deze lijst leeg is, dan moet er volledig gestopt worden met splitsen.

	vector<Leaf> kinderen = hoofdruimte.split();

	while(kinderen.size()>1) {

		vector<Leaf> kids = kinderen;
		kinderen = {};

		for( Leaf kind : kids) {
	// een link tussen ouder en kind leggen
			root = Insert(root, kind);

	// als er in het kind meer dan 1 deeltje zit, dan moet er opnieuw gesplitst worden. hiervoor moet deze ruimte terug in de lijst 'kinderen worden gestopt.
			if (kind.deeltjes().size() > 1) {
				vector<Leaf> newkids = kind.split();
				for (Leaf newkid : newkids) {kinderen.push_back(newkid);}
			}
		}
	}
	return root;
}

// functie om de tree eens te overlopen en te vernietigen, zodat de memory niet vol zit.


node* overloop(node* tree) {
	if (tree == NULL) {return tree;}

	else {if (tree->I != NULL) tree->I = overloop(tree->I);

	if (tree->II != NULL) tree->II = overloop(tree->II);

	if (tree->III != NULL) tree->III = overloop(tree->III);

	if (tree->IV != NULL) tree->IV = overloop(tree->IV);

	if (tree->V != NULL) tree->V = overloop(tree->V);

	if (tree->VI != NULL) tree->VI = overloop(tree->VI);

	if (tree->VII != NULL) tree->VII = overloop(tree->VII);

	if (tree->VIII != NULL) tree->VIII = overloop(tree->VIII);
	
	if (tree->I == NULL && tree->II == NULL && tree->III == NULL && tree->VI == NULL && tree->V == NULL && tree->IV == NULL && tree->VII == NULL && tree->VIII == NULL){
	delete tree;
	tree = NULL;
	return tree;
	}}

}


// functie om de kracht op een deeltje te berekenen. deze geeft een som terug van alle krachten die op het bepaalde deeltje inwerken.
// deeltje = het deeltje, waarop we de inwerkende kracht willen berekenen. root is de tree. N is het totaal aantal deeltje. delta is het criterium voor de width/afstand en soft is de waarde voor de softening

Vec force(node *root, Vec deeltje, double N, double delta, double soft) {
	Vec kracht(0, 0, 0);
	
	if (root == NULL) {}
	else {
		Vec afst = get<0>(root->data) - deeltje; // get<0>(root->data) is de center of mass
		if (afst.x() == 0 && afst.y() == 0 && afst.z() == 0) {kracht = Vec(0, 0, 0);} // zodat het deeltje zichzelf niet meeneemt
		else {
			if (get<2>(root->data) / afst.norm() > delta && get<3>(root->data) != 1) {

				kracht += force(root->I, deeltje, N, delta, soft);
				kracht += force(root->II, deeltje, N, delta, soft);
				kracht += force(root->III, deeltje, N, delta, soft);
				kracht += force(root->IV, deeltje, N, delta, soft);
				kracht += force(root->V, deeltje, N, delta, soft);
				kracht += force(root->VI, deeltje, N, delta, soft);
				kracht += force(root->VII, deeltje, N, delta, soft);
				kracht += force(root->VIII, deeltje, N, delta, soft);
			}
			else {
				kracht = get<3>(root->data)/N * (1 / pow(afst.norm2() + soft, 3/2)) * afst; // get<3>(root->data) is totaal aantal deeltjes
				//cout << kracht.x() << " " << kracht.y() << " " << kracht.z() << endl;
			}
		}
	}
	return kracht;
}

#endif
