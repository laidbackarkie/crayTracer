/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef POINT_H
#define POINT_H

//get stl headers
#include <vector>


using namespace std;

class Point{
public:
  //constructors
  Point()
  : xyz(vector<double>(3))
  {}

  //specific constructor
  Point(double x, double y, double z);

  //specific constructor for vector
  Point(const vector<double>& vals);

  //move point (vector trans)
  void movePoint(double x, double y, double z);


  //getters
  inline double getX(){return xyz[0];}
  inline double getY(){return xyz[1];}
  inline double getZ(){return xyz[2];}
  //get point
  inline const vector<double>& getCoords() const {return xyz;}

  //setters
  inline void setX(double x){xyz[0] = x;}
  inline void setY(double y){xyz[1] = y;}
  inline void setZ(double z){xyz[2] = z;}
  inline void setCoords(const vector<double>& newCoords){xyz = newCoords;}
  // inline void setCoords(vector<double>& newCoords){xyz = newCoords;}
  //destructor
  ~Point(){
    //all data primitive and on stack
  }

private:
  vector<double> xyz;

};//end Point class

#endif//close guard and Point header
