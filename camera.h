/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef CAMERA_H
#define CAMERA_H

//get stl headers
#include <vector>
#include <cmath>

#include "point.h"
#include "image.h"
#include "light.h"
#include "polygon.h"
#include "model.h"
using namespace std;

class Camera{
public:
  //constructors
  Camera()
  : FP(new Point()), VUP(new Point()), lookAt(new Point()), scene(new Image()), lights(vector<Light*>()),
      Umin(0), Umax(0), Vmin(0), Vmax(0), focalLength(0), validCam(false), calcMat(new Matrix())
  {}
  //readCam from main as specific constructor
  Camera(ifstream& camFile);
  //create cameraProjection method (from main)
  void createCamProj();

  //method that takes in the material file
  //extracts and creates lights in file
  bool getLights(ifstream& lightsFile);

  //method that does pa3's pixel color calculation
  //takes in a polygon and the point that the ray hit that polygon
  vector<double> tracePixelColor(Point* intersectP, Polygon* hitPoly, vector<double>& Uvector,
                                  vector<Model*>& models, int modNum, int polNum, double currKs, double currKt, int recCount);


  //getters
  inline Point* getFP() const {return FP;}
  inline Point* getVUP() const {return VUP;}
  inline Point* getLAP() const {return lookAt;}
  inline Image* getImage() const {return scene;}
  inline const vector<Light*>& getLights() const{return lights;}
  inline vector<double>& getN(){return nUnit;}
  inline vector<double>& getU(){return uUnit;}
  inline vector<double>& getV(){return vUnit;}
  inline int getMinU() const {return Umin;}
  inline int getMinV() const {return Vmin;}
  inline int getMaxU() const {return Umax;}
  inline int getMaxV() const {return Vmax;}
  inline int getW() const {return Umax-Umin;}
  inline int getH() const {return Vmax-Vmin;}
  inline double getD() const {return focalLength;}
  inline bool getValidity() const {return validCam;}

  // //setters
  // inline void setFP(Point){return FP;}
  // inline void setVUP(){return VUP;}
  // inline void setLAP(){return lookAt;}
  // inline void setImage(){return scene;}
  // inline void setLights(){return lights;}
  // inline void setMinU(){return Umin;}
  // inline void setMinV(){return Vmin;}
  // inline void setMaxU(){return Umax;}
  // inline void setMaxV(){return Vmax;}
  // inline void setD(){return focalLength;}
  // inline void setValidity(){return validCam;}


  //destructor
  ~Camera(){
    //delete points
    delete FP;
    delete VUP;
    delete lookAt;
    //delete image
    delete scene;
    //delete all lights
    for(int i = 0; i < lights.size(); i++)
      delete lights[i];
    //delete calculation matrix
    delete calcMat;
  }



private:
  Point* FP;
  Point* VUP;
  Point* lookAt;
  Image* scene;
  Matrix* calcMat;
  vector<Light*> lights;
  int Umin, Umax, Vmin, Vmax;
  double focalLength;
  vector<double> nUnit;
  vector<double> uUnit;
  vector<double> vUnit;
  bool validCam;

};

#endif//close guard and camera header
