/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa1
date: 09.15.15
email: laidbackarkie@yahoo.com
*/
#ifndef MODEL_H
#define MODEL_H

//include necessary stl libraries
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

//include local headers
#include "matrix.h"
#include "sphere.h"
#include "point.h"
#include "polygon.h"

using namespace std;

class Model{
public:
  //default constructor
  Model()
  : numFaces(0), numVertices(0), xVals(vector<double>()), yVals(vector<double>()),
      zVals(vector<double>()), faces(vector<Polygon*>()), header(vector<string>()),
        modelSphere(new Sphere()), vertices(vector<Point*>())
  {}
  //specific constructor
  explicit Model(ifstream& inStr);

  //method that takes in the material vals
  //applies materials to model's polygons
  //assumes that vals were checked upon reading, so none are made
  void setMaterial(int start, int stop, double x, double y, double z, double s, int exp, double trans);

  //fxn to set model's x, y, and z vals into a single list of points
  void setVertList();

  //mean vertex getters (for bounding box)
  double getMeanVertex(vector<double>& points);

  //method that prints the bounding box of an object in large friendly letters
  void printBBox();
  //bounding box auxillary fxns
  double getMin(vector<double>& points);
  double getMax(vector<double>& points);


  //getters
  inline int getFaceCount() const {return numFaces;}
  inline int getVertCount() const {return numVertices;}
  inline bool getValidity() const {return valid;}
  inline Sphere* getSphere() const {return modelSphere;}
  inline const vector<Polygon*>& getFaces() const {return faces;}
  inline const vector<Point*>& getVerts() const {return vertices;}


  //setters
  inline void setFaceCount(int faces){this->numFaces = faces;}
  inline void setVertCount(int vert){this->numVertices = vert;}
  inline void setValidity(bool v){this->valid = v;}
  inline void setSphere(Sphere* s) {modelSphere = s;}
  inline void setFaces(vector<Polygon*>& pols) {faces = pols;}
  inline void setVerts(vector<Point*>& points) {vertices = points;}

  //printer
  void printModeltoFile(ofstream& outStr, vector<Matrix*> transforms, char const* outfileName);

  //destructor
  ~Model(){
    //delete surrounding sphere
    delete modelSphere;
    //delete all polygons in model
    for(int i = 0; i < faces.size(); i++)
      delete faces[i];
    //delete all of the points in the vertList
    for(int j = 0; j < vertices.size(); j++)
      delete vertices[j];

  }



  vector<double> xVals;
  vector<double> yVals;
  vector<double> zVals;
  // vector<vector<int> > intFaces;
  vector<string> header;

private:
  Sphere* modelSphere;
  vector<Polygon*> faces;
  vector<Point*> vertices;
  bool valid;
  int numFaces, numVertices;

};//end model class

#endif//end model header
