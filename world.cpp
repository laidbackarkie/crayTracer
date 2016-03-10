/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4 - world
date: 12.06.15
email: laidbackarkie@yahoo.com
*/

//include header
#include "world.h"

using namespace std;

/*
  TODO:
*/

//specific constructor
World::World(vector<Model*>& models, Camera* cam){
  this->modelList = models;
  this->sceneCam = cam;
  this->calcMat = new Matrix();
}//end specific constructor

//intersection front-end methods
//completes all intersection code and returns true or false
//if ret:true->poi, mod#,and pol# are all set
bool World::intersectRay(Point* srcPoint, vector<double>& directionVec, Point* POI,
                        int hitModNum, int hitPolNum){
  //create vectors to hold list of tVals, and corresponding mod# & poly# for intersections
  vector<double> tVals;
  vector<int> modNumbers;
  vector<int> polygonNumbers;
  vector<Point*> intersections;
  //for each model
  for(int modNum = 0; modNum < modelList.size(); modNum++){
    if(modelList[modNum]->getSphere()->checkSphereIntersect(srcPoint, directionVec) == true){
      //for each polygon in model
      for(int polNum = 0; polNum < modelList[modNum]->getFaces().size(); polNum++){
        //check sphere intersection for each polygon
        if(modelList[modNum]->getFaces()[polNum]->getSphere()->checkSphereIntersect(srcPoint, directionVec) == true){
          double currTVal;
          //check plane intersection-- sets the t val that corresponds to intersection point with plane
          if(modelList[modNum]->getFaces()[polNum]->checkPlaneIntersect(srcPoint, directionVec, currTVal) == true){
              //plug t into eq to get actual intersection point
              vector<double> tU;
              calcMat->scalarMult(currTVal, directionVec, tU);
              vector<double> LplustU;
              calcMat->vectorAdd(srcPoint->getCoords(), tU, LplustU);
              Point* POItemp = new Point(LplustU);
              //check polygon intersection
              if(modelList[modNum]->getFaces()[polNum]->checkPolyIntersect(srcPoint, directionVec, POItemp) == true){
                //ray intersected with polygon, store t, POI, polNum, & modNum
                tVals.push_back(currTVal);
                modNumbers.push_back(modNum);
                polygonNumbers.push_back(polNum);
                intersections.push_back(POItemp);
              }// end case of polygon intersection
              else delete POItemp; //case that point was outside poly on plane, clean up memory
          }// end case of plane intersection
        }// end case of sphere intersection with poly
      }// end polygon loop
    }//end case of sphere intersection with model
  }// end modelLoop
  //done with modelLoop for this pixel, all tVals aquired, find smallest tVal > 0;
  if(tVals.size() > 0){
    int index = 0;
    double smallest = tVals[0];
    for(int i = 0; i < tVals.size(); i++){
      if(tVals[i] <= smallest){
        smallest = tVals[i];
        index = i;
      }//end loop through tVals
    }//found smallest tVal for pixel

    POI->setCoords(intersections[index]->getCoords());
    hitModNum = modNumbers[index];
    hitPolNum = polygonNumbers[index];
    //clean up memory
    // for(int i = 0; i < intersections.size(); i++){
    //   if(i != index){
    //     delete intersections[i];
    //     intersections[i] = NULL;
    //  }
    // }
    return true;
  }//end case that an intersection occurred
  //no tVals aquired, NO INTERSECTION occurred, return false
  else return false;
}//end intersection front-end method
