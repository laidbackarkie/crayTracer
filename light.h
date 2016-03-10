/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef LIGHT_H
#define LIGHT_H

//get stl headers
#include "point.h"
using namespace std;

class Light{
public:
  //constructors
  //default constructor (set type to 1 so no issues arise in deleting)
  Light()
  : ambient(false), r(0), g(0), b(0), location(new Point())
  {}
  //specific constructor creates a point light
  Light(double r, double g, double b, Point* loc);
  //specific constructor creates an ambient light
  Light(double r, double g, double b);

  //getters
  inline Point* locate(){return location;}
  inline double getR(){return r;}
  inline double getG(){return g;}
  inline double getB(){return b;}
  inline bool getAmbient(){return ambient;}



  //destructor
  ~Light(){
    //delete location if light isn't ambient
    //ambient has no position so location would == NULL
    if(ambient == false)
      delete location;
  }


private:
  //ambient:true = ambient ambient: false = pointLight
  bool ambient;
  //all 3 should be <= 1 && >=0
  double r, g, b;
  //if ambient, location sets to null
  Point* location;

};//end Light class

#endif//close guard and Light header
