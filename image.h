/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef IMAGE_H
#define IMAGE_H

//get stl headers
#include <fstream>
#include <vector>
#include <cmath>
//get project headers
#include "rgb.h"

using namespace std;

class Image{
public:
  //constructors
  Image()
  : pixels(vector<RGB*>()), maxPval(0), minPval(0), width(0), height(0), colors(vector<vector<double> >())
  {}

  //rgb print ppm
  void printColorPPM(ofstream& outStr);

  //silhouette print ppm
  void printSilhouette(ofstream& outStr);

  //scaling fxn that rescales pixels of given image
  //takes in min and max and scales the values stored in "colors"
  //scaled "colors" become RGBs in "pixels" vector
	void imageRescale();

  //auxillary methods for scaling that get min and max color values
  double calcMinC();
  double calcMaxC();


  //setters
  inline void setNextPix(RGB* color){pixels.push_back(color);}
  inline void setNextColor(vector<double>& curr){colors.push_back(curr);}
  inline void setPix(int pixNum, RGB* color){pixels[pixNum] = color;}
  inline void setMaxP(int a){maxPval = a;}
  inline void setMinP(int a){minPval = a;}
  inline void setMaxC(double a){maxCval = a;}
  inline void setMinC(double a){minCval = a;}
  inline void setWidth(int w){width = w;}
  inline void setHeight(int h){height = h;}

  //getters
  inline RGB* getPixAt(int i){return pixels[i];}//assumes not going out of bounds
  inline vector<RGB*>& getAllPix(){return pixels;}
  inline vector<vector<double> >& getAllColors(){return colors;}
  inline int getMaxP(){return maxPval;}
  inline int getMinP(){return minPval;}
  inline double getMaxC(){return maxCval;}
  inline double getMinC(){return minCval;}
  inline int getWidth(){return width;}
  inline int getHeight(){return height;}


  //destructor
  ~Image(){
    //delete image's pixels
    for(int i = 0; i < pixels.size(); i++)
      delete pixels[i];
  }


private:
  vector<RGB*> pixels;
  vector<vector<double> > colors;
  int maxPval, minPval, width, height;
  double maxCval, minCval;


};//end Image class

#endif//close guard and Image header
