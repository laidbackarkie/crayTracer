/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4 - world
date: 12.06.15
email: laidbackarkie@yahoo.com
*/

#ifndef WORLD_H
#define WORLD_H

//include stl functions
#include <vector>
//include project headers
#include "model.h"
#include "camera.h"

using namespace std;
class World{
public:
  //default constructor
  World()
  : modelList(vector<Model*>()), sceneCam(new Camera()), calcMat(new Matrix())
  {}

  //specific constructor
  World(vector<Model*>& models, Camera* cam);

  //intersection front-end methods
  //completes all intersection code and returns true or false
  //if ret:true->poi, mod#,and pol# are all set
  bool intersectRay(Point* srcPoint, vector<double>& directionVec, Point* POI,
                          int hitModNum, int hitPolNum);


  //getters
  inline vector<Model*>& getModelList(){return modelList;}
  inline Camera* getCamera(){return sceneCam;}

  //destructor
  ~World(){
    //delete camera in world
    delete sceneCam;
    //delete calculation Matrix
    delete calcMat;
    //delete all models in world
    for(int i = 0; i < modelList.size(); i++)
      delete modelList[i];
  }

private:
  vector<Model*> modelList;
  Camera* sceneCam;
  Matrix* calcMat;
};
//close file guard
#endif
