/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "sphere.h"

using namespace std;

/*TODO

*/

//specific constructor for a set of points
Sphere::Sphere(vector<Point*>& verts){
  this->calcMat = new Matrix();
  //get and set center
  this->setCenter(verts);
  //get and set radius
  //calling this method assumes that center has been set
  this->setRadius(verts);
}

//check sphere intersection
//check if ray intersects with sphere around object
bool Sphere::checkSphereIntersect(Point* pixPoint, vector<double>& uVector){
  /*PROCESS
  calc sphere center
  calc sphere radius
  calc Tvector = center - pixPoint
  calc s1 and s2 = (U dot T) +- sqrt((U dot T)^2 - T^2 - r^2)
    (choosing the smallest non-negative of the two)
    if none found: return false
  */
    vector<double> tVector;
    vector<double> tVecUnit;
    calcMat->vectorSub(pixPoint->getCoords(),sphereCenter->getCoords(), tVector);
    calcMat->makeUnit(tVector, tVecUnit);
    double s1, s2;
    s1 = calcMat->sphereQuadraticSolve(true, uVector, tVecUnit, radius);
    s2 = calcMat->sphereQuadraticSolve(false, uVector, tVecUnit, radius);
    //check if there is a non-negative root
    //if so return false, intersection is behind camera/on lens
    if(s1 < -.00001 && s2 < -.00001){
      //cout << "polygon behind camera" << endl; //debugging
      return false;
    }
    //if reached here w/out returning false, then the ray from the
    //pixel intersects the sphere sorrounding the polygon: return true
    return true;
}//end sphere intersection check

//fxn that determines the center of a given set of points
//returns a vector containing the x,y,z of middle
void Sphere::setCenter(vector<Point*>& points){
  vector<double> center(3);
  double avgX = 0.0;
  double avgY = 0.0;
  double avgZ = 0.0;
  for(int i = 0; i < points.size(); i++){
    avgX += points[i]->getCoords()[0];
    avgY += points[i]->getCoords()[1];
    avgZ += points[i]->getCoords()[2];
  }
  avgX /= double(points.size()); //divide by numVertices
  avgY /= double(points.size()); //divide by numVertices
  avgZ /= double(points.size()); //divide by numVertices
  center[0] = (avgX);
  center[1] = (avgY);
  center[2] = (avgZ);
  //set center
  this->sphereCenter = new Point(center);
  //this->sphereCenter->setCoords();
}

//fxn that determines the radius of a sphere
//takes in a center point, and a set of points
//determines which point P is furthest away
//returns the magnitude of P minus center
void Sphere::setRadius(vector<Point*>& points){
  double r = 0.0;
  vector<double> curr;
  for(int i = 0; i < points.size(); i++){
    curr.clear();
    calcMat->vectorSub(sphereCenter->getCoords(), points[i]->getCoords(), curr);
    double currMag = calcMat->magnitude(curr);
    if(currMag > r) r = currMag;
  }
  this->radius = r;
}
