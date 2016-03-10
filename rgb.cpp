/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "rgb.h"

using namespace std;

/*TODO
unit test
*/

  //specific constructor for set rgb values
  RGB::RGB(int r, int g, int b){
    this->color.push_back(r);
    this->color.push_back(g);
    this->color.push_back(b);
  }

  //specific constructor for vector of rgb vals
  RGB::RGB(vector<int>& vals){
    this->color.push_back(vals[0]);
    this->color.push_back(vals[1]);
    this->color.push_back(vals[2]);
  }
