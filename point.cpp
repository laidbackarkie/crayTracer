/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "point.h"
#include "matrix.h"

using namespace std;

/*TODO
unit test
*/

//specific constructor for doubles
Point::Point(double x, double y, double z){
  this->xyz.push_back(x);
  this->xyz.push_back(y);
  this->xyz.push_back(z);
}

//specific constructor for vector
Point::Point(const vector<double>& vals){
  this->xyz.push_back(vals[0]);
  this->xyz.push_back(vals[1]);
  this->xyz.push_back(vals[2]);
}

//move point (vector trans)
void Point::movePoint(double x, double y, double z){
  this->xyz[0]+=x;
  this->xyz[1]+=y;
  this->xyz[2]+=z;
}
