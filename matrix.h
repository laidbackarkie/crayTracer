/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4
date: 11.07.15
email: laidbackarkie@yahoo.com
*/
#ifndef MATRIX_H
#define MATRIX_H

//include necessary stl headers
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

class Matrix{
public:
  //default constructor
  //used for accessing member fxns primarily
  Matrix()
    :elements(vector<vector<double> >())
    {}


  //matrix multiply fxn, multiplies this by m
  //returns a matrix pointer
  vector<vector<double> > multiply(vector<vector<double> >& elements, int countFace);
  //matrix multiply that works for transformation matrices only
  vector<vector<double> > transMultiply(vector<vector<double> >& elements);


  //vector magnitude fxn: squares all indices, sums, and sqroots
  //returns a scalar that is the magnitude of this
  double magnitude(vector<double>& v);

  //boolean fxn to determine if given vector a is 0-vector
  //returns true if all elements are 0
  //returns false else
  bool checkZeroVec(vector<double>& a);

  //multiplies vector a by the scalar s
  //assumes that the out vector is new
  void scalarMult(double s, vector<double>& a, vector<double>& out);

  //adds the vectors a and b
  //out is a vector that is the sum between the two
  //assumes that the out vector is new
  void vectorAdd(const vector<double>& a, const vector<double>& b, vector<double>& out);

  //subtracts the vectors b from a
  //out is a vector that is the difference between the two
  //assumes that the two vectors are of equal length
  //assumes that the out vector is new
  void vectorSub(const vector<double>& a, const vector<double>& b, vector<double>& out);

  //vector fxn that makes the vector into a unit vector
  //divides this by it's magnitude, out is a vector of unit length
  //assumes that the out vector is new
  void makeUnit(vector<double>& v, vector<double>& out);

  //vector dot product fxn: mults all indices
  //returns a double that is the dot product of a & b
  double dotProd(vector<double>& a, const vector<double>& b);

  //vector cross-product method.
  //takes 2 matrices (1 arg and this) and performs x-product EQ
  //x-product EQ: ((v1y*v2z - v2y*v1z),(v1z*v2x - v2z*v1x),(v1x*v2y - v2x*v1y))
  //out is a vector that is [a] X [b]
  //assumes that the out vector is new
  void crossProd(const vector<double>& a, const vector<double>& b, vector<double>& out);

  //matrix transpose method
  //takes 'this'.elements and returns transpose of them
  //assumes that the matrix is square
  vector<vector<double> > transpose();

  //quadratic equation fxn used for sphere intersection
  //if 'plus' == true then add sqrt
  //else subtract sqrt
  double sphereQuadraticSolve(bool plus, vector<double>& uVec, vector<double>& tVec, double rad);



  //matrix scale, takes three coordinates
  Matrix* scale(double x, double y, double z);
  //matrix translate, takes three coordinates
  Matrix* translate(double x, double y, double z);
  //matrix rotate, takes three coordinates (axis of rotation), and theta (degree of rotation)
  Matrix* rotate(double x, double y, double z, double theta);
  //matrix project for cameraModels
  Matrix* camProj(double length);

  //getters

  //setters



  //destructor
  ~Matrix(){
    //all vars are primitive and vector so they'll be cleaned off of stack
  }

  vector<vector<double> > elements;
private:



};//end Matrix class

#endif//end matrix header
