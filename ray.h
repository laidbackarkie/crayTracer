/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef RAY_H
#define RAY_H

//get stl headers
#include <vector>

#include "point.h"

using namespace std;

class Ray{
public:
  //constructors
  Ray()
  : rayPoint(new Point()), rayVec(vector<double>())
  {}

  //specific constructor
  Ray(Point* src, vector<double>& direction);

  //getRay (vector add)
  vector<double> getRay();

  //get vec
  inline vector<double>& getVector(){return rayVec;}
  //get point
  inline Point* getPoint(){return rayPoint;}


  //destructor
  ~Ray(){
    //delete point
    delete rayPoint;
  }


private:
  Point* rayPoint;
  vector<double> rayVec;

};//end ray class

#endif//close guard and ray header
