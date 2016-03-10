/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef RGB_H
#define RGB_H

//get stl headers
#include <vector>

using namespace std;

class RGB{
public:
  //constructors
  RGB()
  : color(vector<int>())
  {}

  //specific constructor for set rgb values
  RGB(int r, int g, int b);
  //specific constructor for vector of rgb values
  RGB(vector<int>& vals);

  //set r, set g, set b
  inline void setR(int r){color[0] = r;}
  inline void setG(int g){color[1] = g;}
  inline void setB(int b){color[2] = b;}

  //get r, get g, get b
  inline int getR() const {return color[0];}
  inline int getG() const {return color[1];}
  inline int getB() const {return color[2];}

  //destructor
  ~RGB(){
    //all data primitive and on stack
  }


private:
  //color[0] =r color[1] = g color[2] = b
  vector<int> color;

};//end RGB class

#endif//close guard and RGB header
