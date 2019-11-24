//
//  probeersel2.cpp
//  
//
//  Created by Sébastien Chevalier on 24/11/2019.
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





//knoopstructuur maken
struct node {
    double w;
    double r;
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
node* nieuweknoop(double ww, double rr) {
    node *knoop = new struct node();
    knoop->w = ww;
    knoop->r = rr;
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

void preorder(node *root) {
    if (root == NULL) return;
    cout << " w = " << root->w << " en r = " << root->r << endl;
    preorder(root->I);
    preorder(root->II);
    preorder(root->III);
    preorder(root->IV);
    preorder(root->V);
    preorder(root->VI);
    preorder(root->VII);
    preorder(root->VIII);
}


double delta = 0.2;

//functie die totaal aantal deeltjes telt door de boom te doorlopen
void preorder2(node *root) {
    if (root == NULL) return;
    cout << " w = " << root->w << " en r = " << root->r << endl;
    if ((root->w)/(root->r) < delta){
        cout << "klein genoeg" << endl;
        return;
    }
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
    struct node *root = nieuweknoop(1.0,2.0);
    root->I = nieuweknoop(0.5,1.0);
    (root->I)->I = nieuweknoop(0.25, 0.7);
    (root->I)->II = nieuweknoop(0.25, 0.8);
    (root->I)->III = nieuweknoop(0.25, 1.1);
    (root->I)->IV = nieuweknoop(0.25, 1.4);
    (root->I)->V = nieuweknoop(0.25, 1.7);
    (root->I)->VI = nieuweknoop(0.25, 1.8);
    (root->I)->VII = nieuweknoop(0.25, 0.6);
    (root->I)->VIII = nieuweknoop(0.25, 0.4);
    root->II = nieuweknoop(0.5,0.8);
    root->III = nieuweknoop(0.5, 0.7);
    root->IV = nieuweknoop(0.5, 4.0);
    root->V = nieuweknoop(0.5, 7.0);
    root->VI = nieuweknoop(1.0, 4.0);
    root->VII = nieuweknoop(1.0, 1.2);
    root->VIII = nieuweknoop(0.25, 0.75);
    (root->V)->I = nieuweknoop(0.25, 0.4);
    (root->V)->II = nieuweknoop(0.25, 0.7);
    (root->V)->III = nieuweknoop(0.25, 0.99);
    (root->V)->IV = nieuweknoop(0.25, 2);
    (root->V)->V = nieuweknoop(0.21, 4);
    (root->V)->VI = nieuweknoop(0.31, 0.7);
    (root->V)->VII = nieuweknoop(0.21, 0.45);
    (root->V)->VIII = nieuweknoop(0.11, 0.09);
    

    //print die boom
    preorder2(root);
}
