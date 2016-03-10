/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4 - main
date: 11.07.15
email: laidbackarkie@yahoo.com
*/
/*TODO:

      ------
      FIND WHERE ROUNDING ERROR IS OCCURING!!
      ------
      consider turning return vals to void and simply changing vectors sent as args
      CLEAN UP MEMORY! VALGRIND (find leak that is "still reachable")
*/

//include stl files
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>
using namespace std;

//include local files
#include "matrix.h"
#include "model.h"
#include "camera.h"
#include "image.h"
#include "rgb.h"
#include "sphere.h"
#include "ray.h"
#include "light.h"
#include "material.h"
#include "point.h"
#include "polygon.h"
#include "world.h"

/*
//MATERIAL GETTER FXN--------------------------------------------------
//method that takes in the material file
//extracts and creates materials in file
  after 'M':
    all numbers are positive
    first 3 are ints
    next 4 are doubles s.t.:
      0<= first 4 #s <= 1
    next # is int and can be > 1
    last # is 0<=double<=1 for translucency
//applies materials to model's polygons
//returns true if no problem processing
*/
bool getMaterials(ifstream& matFile, vector<Model*>& models){
  //vars
  string currLine;
  char firstLetter;
  int modNum, startPoly, endPoly, exponent;
  double diffX, diffY, diffZ, shine, translucency;
  //begin processing material file
  getline(matFile,currLine);
  while(!matFile.eof()){
    //cout << currLine << endl;//debugging
    if(currLine.at(0) == 'M' || currLine.at(0) == 'm'){
      stringstream lineHolder;
      lineHolder.str(string());
      lineHolder.str(currLine);

      lineHolder >> firstLetter;
      lineHolder >> modNum;
      if(lineHolder.fail()) return false;
      //check if model is within bounds
      if(modNum >= models.size() || modNum < 0) return false;

      lineHolder >> startPoly;
      if(lineHolder.fail()) return false;
      //check if startpolygon is within bounds
      if(startPoly >= models[modNum]->getFaces().size() || startPoly < 0) return false;
      lineHolder >> endPoly;
      if(lineHolder.fail()) return false;

      //check if endPoly is within bounds
      if(endPoly >= models[modNum]->getFaces().size() || endPoly < 0) return false;
      lineHolder >> diffX;

      //get last 6 values for material--
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(diffX > 1 || diffX < 0) return false;
      lineHolder >> diffY;
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(diffY > 1 || diffY < 0) return false;
      lineHolder >> diffZ;
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(diffZ > 1 || diffZ < 0) return false;
      lineHolder >> shine;
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(shine > 1 || shine < 0) return false;
      lineHolder >> exponent;
      if(lineHolder.fail()) return false;
      if(exponent < 0) return false;
      lineHolder >> translucency;
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(translucency > 1 || translucency < 0) return false;


      //apply material to model
      models[modNum]->setMaterial(startPoly, endPoly, diffX, diffY, diffZ, shine, exponent, translucency);
    }//end case that line was a material
    //get next line and loop
    getline(matFile,currLine);
  }//end of file
  matFile.close();
  //no problems have occurred return true
  return true;
}//end material getter------------------------------------------
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
//code adapted from "Maxpms" code on stack overflow
//random number generator for double 'x' s.t. -.5<=x<=.5
double AArandom(){
  return (double(rand()) / double(RAND_MAX)) - 0.5 ;
}//end random number generator for Anti-Aliasing range---------

////////////////////////////////////////////////////////////////
//PROGRAM ENTERS EXECUTION
int main(int argc, char const *argv[]) {
  //variable declarations
  vector<Model*> modelList;

  //CHECK # OF ARGS (SHOULD BE >= 5: 4 PLUS EXECUTABLE)
  if(argc < 5){
    cout << "error: number of args should be 4 or greater" << endl;
    cout << "USAGE:: [camera] [lights/materials] [model1 ... modeln] [outputfile]" << endl;
    return -1;
  }

  //OPEN AND CHECK THE CAMERA FILE
  ifstream cameraFile;
  cameraFile.open(argv[1], ios::in);
  if(cameraFile.fail()){
    cout << "error: failed opening camera file" << endl;
    return -1;
  }
  //process camera
  Camera* sceneCam = new Camera(cameraFile);
  bool validCam = sceneCam->getValidity();
  if(validCam == false){
    cout << "error: invalid camera" << endl;
    return -1;
  }//end camera processing

  //OPEN AND CREATE MODELS FROM MODEL FILES
  //model files are argv[3] - argv[size-2]
  for(int i = 3; i < (argc-1); i++){
    ifstream currFile;
    currFile.open(argv[i], ios::in);
    if(currFile.fail()){
      cout << "error: failed opening model file:"<< (i-2) << endl;
      return -1;
    }
    modelList.push_back(new Model(currFile));
    //cout << "processed model:" << (i-2) << endl; //debugging
    currFile.close();
  }//end model creation
  //SINCE MODELS ARE GATHERED, NOW PROCESS MATERIAL&LIGHT FILE
  //process lights and materials and apply to camera and models
  ifstream materialFile;
  materialFile.open(argv[2], ios::in);
  if(materialFile.fail()){
    cout << "error: failed to open material file" << endl;
    return -1;
  }
  //create materials and apply to models
  bool matCheck = getMaterials(materialFile, modelList);
  if(matCheck == false){
    cout << "error: processing material file" << endl;
    return -1;
  }
  //process lights and apply to camera
  //assumes that material file was closed in 'getMaterials'
  ifstream lightFile;
  lightFile.open(argv[2], ios::in);
  if(lightFile.fail()){
    cout << "error: failed to open light file" << endl;
    return -1;
  }
  //create materials and apply to models
  bool lightCheck = sceneCam->getLights(lightFile);
  if(lightCheck == false){
    cout << "error: processing light file" << endl;
    return -1;
  }
  //end full material&light processing

  //OPEN AND CHECK OUTPUT FILE
  ofstream outfile;
  outfile.open(argv[argc-1], ios::out);
  if(outfile.fail()){
    cout << "error: failed opening output file" << endl;
    return -1;
  }

  //PRINT BOUNDINGBOX FOR EACH MODEL DEBUGGING
  for(int i = 0; i < modelList.size(); i++){
    cout<< "model" << i;
    modelList[i]->printBBox();
  }

  //CREATE WORLD WITH CAMERA AND MODELS NOT THAT AQUIRED
  World* scene = new World(modelList, sceneCam);

  Matrix* holderMat = new Matrix(); //just a matrix used for calling member fxns of matrix (NO DATA)
  srand(time(NULL));

  /*
  begin tracing rays from FP to each pixel
    pixels located at fp+dn+au+bv (fp + dn are constant)
  */
  //vars
  vector<double> au;
  vector<double> bv;
  vector<double> auPlusbv;
  vector<double> Uvec;
  vector<double> uVecUnit;
  //create vectors to hold list of tVals, and corresponding mod# & poly# for intersections
  vector<double> tVals;
  vector<int> modNumbers;
  vector<int> polygonNumbers;
  vector<Point*> intersections;
  vector<double> pixPtCoords;
  //declare progress bar stringstream
  stringstream progress;
  int progressCount = -1;
  //calc vector to Image = fp+dn [CONSTANT FOR ALL PIXELS]
  vector<double> dn;
  holderMat->scalarMult(sceneCam->getD(), sceneCam->getN(), dn);
  vector<double> vToImg;
  holderMat->vectorAdd(sceneCam->getFP()->getCoords(),dn, vToImg);
  Point* pixelPoint = new Point();
  for(int b = sceneCam->getMaxU(); b>= sceneCam->getMinU(); b--){
  //   //progress meter for debugging
  //   progressCount++;
  //   if(b%int(round(sceneCam->getW()*.1)) == 0){
  //     progress << "#";
  //     if(progress.str().size() == 11){
  //       cout << '\r' << "[" << progress.str() << "] " << (double(progressCount)/double(sceneCam->getW())*100) << '%';
  //       cout.flush();
  //       cout << endl;
  //     }
  //     else{
  //       cout << '\r' << "[" << progress.str() << "] " << (double(progressCount)/double(sceneCam->getW())*100) << '%';
  //       cout.flush();
  //     }
  //   }

    for(int a = sceneCam->getMaxV(); a >= sceneCam->getMinV(); a--){
      //cout << "a, b: " << a << "," << b << endl;//debugging
      //clear vectors for intersections
      au.clear();
      bv.clear();
      auPlusbv.clear();
      Uvec.clear();
      uVecUnit.clear();
      pixPtCoords.clear();

      // vector<vector<double> > pixelAA;
      // //ANTI-ALIASING
      // //send 5 rays through every pixel to smooth out curves
      // for(int rayNum = 0; rayNum < 5; rayNum++){
      //     //get random offsets for ray sampling
      //     double randU = AArandom();
      //     double randV = AArandom();
          ////clear vectors for color
          tVals.clear();
          modNumbers.clear();
          polygonNumbers.clear();
          intersections.clear();

          //calc L = pixelPoint = fp+dn+au+bv
          holderMat->scalarMult((a), sceneCam->getU(), au); //USE WHEN NOT ANTI-ALIASING
          holderMat->scalarMult((b), sceneCam->getV(), bv); //USE WHEN NOT ANTI-ALIASING

          // holderMat->scalarMult((a+randU), sceneCam->getU(), au); //USE WHEN ANTI-ALIASING IS ON
          // holderMat->scalarMult((b+randV), sceneCam->getV(), bv); //USE WHEN ANTI-ALIASING IS ON
          holderMat->vectorAdd(au, bv, auPlusbv);
          holderMat->vectorAdd(vToImg, auPlusbv, pixPtCoords);
          pixelPoint->setCoords(pixPtCoords);
          //end pixelPoint calc
          //calc Uvector = (L-E) / ||(L-E)||
          holderMat->vectorSub(sceneCam->getFP()->getCoords(), pixelPoint->getCoords(), Uvec);
          holderMat->makeUnit(Uvec, uVecUnit);
          //for each model
          for(int modNum = 0; modNum < modelList.size(); modNum++){
            if(modelList[modNum]->getSphere()->checkSphereIntersect(pixelPoint, uVecUnit) == true){
              //for each polygon in model
              for(int polNum = 0; polNum < modelList[modNum]->getFaces().size(); polNum++){
                //check sphere intersection for each polygon
                if(modelList[modNum]->getFaces()[polNum]->getSphere()->checkSphereIntersect(pixelPoint, uVecUnit) == true){
                  double currTVal;
                  //check plane intersection-- sets the t val that corresponds to intersection point with plane
                  if(modelList[modNum]->getFaces()[polNum]->checkPlaneIntersect(pixelPoint, uVecUnit, currTVal) == true){
                      //plug t into eq to get actual intersection point
                      vector<double> tU;
                      holderMat->scalarMult(currTVal, uVecUnit, tU);
                      vector<double> LplustU;
                      holderMat->vectorAdd(pixelPoint->getCoords(), tU, LplustU);
                      Point* POI = new Point(LplustU);
                      //check polygon intersection
                      if(modelList[modNum]->getFaces()[polNum]->checkPolyIntersect(pixelPoint, uVecUnit, POI) == true){
                        //ray intersected with polygon, store t,polNum,& modNum
                        tVals.push_back(currTVal);
                        modNumbers.push_back(modNum);
                        polygonNumbers.push_back(polNum);
                        intersections.push_back(POI);
                      }// end case of polygon intersection
                      else delete POI; //case that point was outside poly on plane, clean up memory
                  }// end case of plane intersection
                }// end case of sphere intersection with poly
              }// end polygon loop
            }//end case of sphere intersection with model
          }// end modelLoop
          //CALCULATE COLOR OF THIS PIXEL USING INTERSECTIONS FROM ABOVE

          //done with modelLoop for this pixel, all tVals aquired, find smallest tVal > 0;
          if(tVals.size() > 0){
            int index;
            double smallest = tVals[0];
            for(int i = 0; i < tVals.size(); i++){
              if(tVals[i] <= smallest){
                smallest = tVals[i];
                index = i;
              }
            }//found smallest tVal for pixel
              Point* rayPOI = new Point();
              int modelNum, pNum;
              if(scene->intersectRay(pixelPoint, uVecUnit, rayPOI, modelNum, pNum) == true){


                cout << "raypoi coords: " << rayPOI->getCoords()[0] << " "<< rayPOI->getCoords()[1] << " "<< rayPOI->getCoords()[2] << endl; //debugging
                cout << "poi coords: " << intersections[index]->getCoords()[0] << " "<< intersections[index]->getCoords()[1] << " "<< intersections[index]->getCoords()[2] << endl;//debugging

          // vector<double> pixColor = sceneCam->tracePixelColor(intersections[index], modelList[modNumbers[index]]->getFaces()[polygonNumbers[index]], uVecUnit,
          //                                                         scene->getModelList(), modNumbers[index], polygonNumbers[index], modelList[modNumbers[index]]->getFaces()[polygonNumbers[index]]->getMaterial()->getKs(),
          //                                                         modelList[modNumbers[index]]->getFaces()[polygonNumbers[index]]->getMaterial()->getKt(), 0);

              //calculate color vector of this pixel using smallest tVal's intersection, and polygon intersection is with
              vector<double> pixColor = sceneCam->tracePixelColor(rayPOI, modelList[modelNum]->getFaces()[pNum], uVecUnit,
                                                                      scene->getModelList(), modelNum, pNum, modelList[modelNum]->getFaces()[pNum]->getMaterial()->getKs(),
                                                                      modelList[modelNum]->getFaces()[pNum]->getMaterial()->getKt(), 0);
              
              // set this pixels colorVal in image
              sceneCam->getImage()->setNextColor(pixColor); //USE WHEN NOT ANTI-ALIASING
              //pixelAA.push_back(pixColor); //USE WHEN ANTI-ALIASING IN ON
              //clean up memory
              //delete rayPOI;
            }
          }
          else{
            //ray didn't intersect any polygons, pixel is black
            vector<double> pixColor(3,0.0);
            //set this pixels colorVal to 0 and push back
            sceneCam->getImage()->setNextColor(pixColor); //USE WHEN NOT ANTI-ALIASING
            //pixelAA.push_back(pixColor); //USE WHEN ANTI-ALIASING IS ON
          }

          //FOR ANTI-ALIASING------
          //on last iteration, avg the returned values & set the pixel color in image
          // if(rayNum == 4){
          //   vector<double> avgColor(3,0.0);
          //   vector<double> interim(3);
          //   for(int i = 0; i < 5; i++){ //sum all returned color vals
          //     holderMat->vectorAdd(avgColor, pixelAA[i], interim);
          //     avgColor = interim;
          //   }
          //   //divide color val sum by 5
          //   for(int i = 0; i < 3; i++) avgColor[i] /= 5.0;
          //   sceneCam->getImage()->setNextColor(avgColor);
          // }

          //clean up memory DELETE INTERSECTIONS
          for(int i = 0; i < intersections.size(); i++){
            delete intersections[i];
          }
        //}//end ANTI-ALIASING loop
        //END PIXEL COLOR CALULATING--------------------------------------
      }// end loop maxV->minV
    }// end loop maxU->minU


  //ALL PIXELS' COLORS HAVE BEEN CALCULATED
  //SCALE COLORS INTO RGB VALS FOR Image
  sceneCam->getImage()->imageRescale();
  /*
  after tracing rays from FP, and determining pixel values:
  print pixels to output file in PPM format (P3)
  */
  sceneCam->getImage()->printColorPPM(outfile);

  //CLEAN UP MEMORY AND EXIT SUCCESSFULLY
  // ->(set each obj to NULL so any reference after will cause error)
  delete scene;//delete scene
  world = NULL;
  delete holderMat;//delete calculation matrix
  holderMat = NULL;

  delete pixelPoint;//delete pixPointonImage
  pixelPoint = NULL;


  return 0;
}//end execution
