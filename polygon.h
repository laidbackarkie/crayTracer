/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef POLYGON_H
#define POLYGON_H

//get stl headers
#include "material.h"
#include "point.h"
#include "sphere.h"

using namespace std;

class Polygon{
public:
  //constructors
  Polygon()
  : vertices(vector<Point*>()), surfMat(new Material()), calcMat(new Matrix()),
            surfNormal(vector<double>(3,0.0)), polySphere(new Sphere())
  {}

  //specific constructor for set of points and a mat
  Polygon(vector<Point*>& vSet, Material* mat);

  //specific constructor for set of points without a material
  Polygon(vector<Point*>& vSet);

  //polygon intersection
  //check if ray intersects with the infinite plane containing the polygon
  bool checkPlaneIntersect(Point* pixPoint, vector<double>& uVector, double& t);
  //plane containing polygon intersection
  //check if ray interection is contained by edges of polygon
  bool checkPolyIntersect(Point* pixPoint, vector<double>& uVector, Point* intersectP);

  //set surface normal to Polygon
  void setSurfNorm();

  //material getter
  inline Material* getSurface(){return surfMat;}
  //material setter
  void setSurfaceMaterial(double x, double y, double z, double s, int a, double t);

  //getters
  inline Point* getVert(int Vnum){return vertices[Vnum];} //assumes that user isn't going out of bounds
  inline const vector<Point*>& getAllVert() const {return vertices;}
  inline int getNumVerts(){return vertices.size();}
  inline Material* getMaterial(){return surfMat;}
  inline Sphere* getSphere(){return polySphere;}
  inline const vector<double>& getSurfNorm() const {return surfNormal;}




  //destructor
  ~Polygon(){
    //delete surrounding sphere
    delete polySphere;
    //delete Material
    delete surfMat;
    //delete all verts in Polygon
    //  B/C OF WAY THEY'RE CREATED, THE POINTS ARE DELETED BY MODEL
    //delete calculation Matrix
    delete calcMat;

  }


private:
  vector<Point*> vertices;
  Material* surfMat;
  Matrix* calcMat;
  Sphere* polySphere;
  vector<double> surfNormal;  //add a surfaceNormal field??


};//end Polygon class

#endif//close guard and Polygon header
