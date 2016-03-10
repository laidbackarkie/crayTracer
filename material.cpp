/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "material.h"

//using namespace std;

/*TODO

*/

//specific constructor
Material::Material(double r, double g, double b, double shiny, int aVal, double transVal){
  this->Kr = r;
  this->Kg = g;
  this->Kb = b;
  this->Ks = shiny;
  this->alpha = aVal;
  this->Kt = transVal;
}//end specific constructor
