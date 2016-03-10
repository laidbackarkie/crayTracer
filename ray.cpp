/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "ray.h"

using namespace std;

/*TODO
-unit test
-should there be a scale vector method calling matrix's in here?
*/
//specific constructor
Ray::Ray(Point* src, vector<double>& direction){
  rayPoint = new Point(src->getCoords());
  rayVec = direction;
}

//getRay (vector add)
vector<double>  Ray::getRay(){
  vector<double> retVal;
  for(int i = 0; i < rayVec.size(); i++)
    retVal.push_back(rayVec[i] + rayPoint->getCoords()[i]);
  return retVal;
}
