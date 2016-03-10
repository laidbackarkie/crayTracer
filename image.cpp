/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "image.h"

#include <iostream>

using namespace std;

/*TODO

*/

//scaling fxn that rescales pixels of given image
void Image::imageRescale(){
  //if min equals max then there would be division by 0, error
  //calc min Color val and max Color val
  double min = calcMinC();
  double max = calcMaxC();
  //cout << min << ", " << max <<endl;
  if(min == max){
    for(int j = 0; j < colors.size(); j++){
      int red = round(255*(double(colors[j][0])));
      int green = round(255*(double(colors[j][1])));
      int blue = round(255*(double(colors[j][2])));
      //cout << red << " "<< green << " "<< blue<< endl;//debugging
      //combine scaled vals into an rgb and push back
      this->pixels.push_back(new RGB(red, green, blue));
    }
  }
  else{
    for(int j = 0; j < colors.size(); j++){
      int red = (round((255*(double(colors[j][0]) - double(min))) / (double(max) - double(min))));
      int green = (round((255*(double(colors[j][1]) - double(min))) / (double(max) - double(min))));
      int blue = (round((255*(double(colors[j][2]) - double(min))) / (double(max) - double(min))));
      //cout << red << " "<< green << " "<< blue<< endl;//debugging
      //combine scaled vals into an rgb and push back
      this->pixels.push_back(new RGB(red, green, blue));
    }
  }
}//end image pixel rescaler

//auxillary methods for scaling that get min and max color values
double Image::calcMinC(){
  double min = colors[0][0];
  for(int i = 0; i < this->colors.size(); i++){
    if(colors[i][0] < min) min = colors[i][0];
    if(colors[i][1] < min) min = colors[i][1];
    if(colors[i][2] < min) min = colors[i][2];
  }
  return min;
}//end min calc
double Image::calcMaxC(){
  double max = 0;
  for(int i = 0; i < this->colors.size(); i++){
    if(colors[i][0] > max) max = colors[i][0];
    if(colors[i][1] > max) max = colors[i][1];
    if(colors[i][2] > max) max = colors[i][2];
  }
  return max;
}//end max calc



//-------------------------output color image for pa3
void Image::printColorPPM(ofstream& outStr){
  //variables
	int w = this->getWidth();
	int h = this->getHeight();
  //check for the right amount of pixels
  if(this->pixels.size() != ((w)*(h))){
    outStr << "error: pixel amount incorrect" << endl;
    outStr << "expected count of " << ((w)*(h));//debugging error print
    outStr << " recieved count of " << this->pixels.size() << endl;//debugging error print
    outStr.close();
    return;
  }

	outStr << "P3" << endl; //print P3 and end line 1
  outStr << "#Image Created by Adrion Q Arkenberg CS410 CSU" << endl;
	outStr << w << " ";	//print width
	outStr << h << " ";	//print height
	outStr << 255 << endl;	//print mxpx and end 2nd line

	//print pixels values
	for(int k = 0; k < this->pixels.size(); k++){
		if(k%w == w-1){
      //case that at end of line width wise, add endl
      outStr << this->pixels[k]->getR() << " ";
      outStr << this->pixels[k]->getG() << " ";
      outStr << this->pixels[k]->getB() << endl;
    }
    else{
      //case that pixel is somewhere within width, just leave a space after
      outStr << this->pixels[k]->getR() << " ";
      outStr << this->pixels[k]->getG() << " ";
      outStr << this->pixels[k]->getB() << " ";
    }
	}//end loop through pixels
  //close file now that printing is complete
	outStr.close();
}//end pa3 color printer

//---------------------------output image printer for pa2
//pix array is 0s and 1s:
//  case 0: fill pix black
//  case 1: fill pix white
//DOESNT WORK CURRENTLY
void Image::printSilhouette(ofstream& outStr){
  if(this->pixels.size() != ((this->width+1)*(this->height+1))){
    outStr << "error: pixel amount incorrect" << endl;
    outStr << "expected count of " << ((this->width+1)*(this->height+1));//debugging error print
    outStr << " recieved count of " << this->pixels.size() << endl;//debugging error print
    outStr.close();
    return;
  }
  //print PPM header
  outStr << "P3"<< endl;
  //print width, height, and maxPixVal (always 255)
  outStr << (this->width+1) << " " << (this->height+1) << " " << "255" << endl;
  //begin printing pixels
  for(int i = 0; i < pixels.size(); i++){
    if(pixels[i]->getR() == 1){
      outStr << "255" << " " << "255" << " " << "255" << endl;
    }
    else{
      outStr << "0" << " " << "0" << " " << "0" << endl;
    }
  }//end pixel printing
  outStr.close();
}//end output image printer
