/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "light.h"

using namespace std;

/*TODO
  fix warning in header (fxn can hit end of nonvoid fxn)
*/

//specific constructor creates a point light
Light::Light(double red, double green, double blue, Point* loc){
  this->r = red;
  this->g = green;
  this->b = blue;
  this->location = new Point();
  this->location->setCoords(loc->getCoords());
  this->ambient = false;
  //because of the way i pass new points in, can just delete it here.
  delete loc;
}//end point light constructor


//specific constructor creates an ambient light
Light::Light(double red, double green, double blue){
  this->r = red;
  this->g = green;
  this->b = blue;
  this->ambient = true;
}//end ambient light constructor
