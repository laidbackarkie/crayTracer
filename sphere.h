/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef SPHERE_H
#define SPHERE_H

//get stl headers
#include <vector>

#include "point.h"
#include "matrix.h"

using namespace std;

class Sphere{
public:
  //constructors
  Sphere()
  : points(vector<Point*>()), sphereCenter(new Point()), calcMat(new Matrix())
  {}

  //specific constructor for a set of points
  Sphere(vector<Point*>& verts);

  //check sphere intersection
  //check if ray intersects with sphere around object
  bool checkSphereIntersect(Point* pixPoint, vector<double>& uVector);

  //setters

  //fxn that determines the radius of a sphere
  void setRadius(vector<Point*>& points);
  //fxn that determines the center of a given set of points
  //returns a vector containing the x,y,z of middle
  void setCenter(vector<Point*>& points);

  //getters
  inline Point* getCenter(){return sphereCenter;}
  inline double getRadius(){return radius;}


  //destructor
  ~Sphere(){
    //delete center
    delete sphereCenter;
    //delete vertices
    for(int i = 0; i < points.size(); i++)
      delete points[i];
    //delete calculation Matrix
    delete calcMat;

  }


private:
  vector<Point*> points;
  Point* sphereCenter;
  double radius;
  Matrix* calcMat;

};//end sphere class

#endif//close guard and sphere header
