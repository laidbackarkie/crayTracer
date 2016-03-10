/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa3
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "polygon.h"

using namespace std;

/*TODO
  unit test
*/

//specific constructor for set of points and a material
Polygon::Polygon(vector<Point*>& vSet, Material* mat){
  this->vertices = vSet;
  this->surfMat = mat;
  this->calcMat = new Matrix();
  //now that matrix and verts have been created set surfNormal
  setSurfNorm();
  this->polySphere = new Sphere(vSet);
}

//specific constructor for set of points with no material
Polygon::Polygon(vector<Point*>& vSet){
  this->vertices = vSet;
  this->surfMat = new Material();
  this->calcMat = new Matrix();
  //now that matrix and verts have been created set surfNormal
  setSurfNorm();
  this->polySphere = new Sphere(vSet);
}

//polygon surface material setter
//calls materials setters to adjust it's material
void Polygon::setSurfaceMaterial(double x, double y, double z, double s, int a, double t){
  surfMat->setKr(x);
  surfMat->setKg(y);
  surfMat->setKb(z);
  surfMat->setKs(s);
  surfMat->setAlpha(a);
  surfMat->setKt(t);
}


//polygon intersection
//check if ray intersects with the infinite plane containing the polygon
bool Polygon::checkPlaneIntersect(Point* pixPoint, vector<double>& uVector, double& t){
  /*PROCESS
  calc N (surface normal to plane)
  find d using N from above (plug into a point)
  check NdotU != 0
  calc t = (d-(NdotL)/(NdotU))
  calc point of intersection = (pixPoint + t*U)
  */
  //get d (only need negative for eq)
  double negD = calcMat->dotProd(surfNormal, vertices[0]->getCoords());

  double nDotU = calcMat->dotProd(surfNormal, uVector);
  double nDotL = calcMat->dotProd(surfNormal, pixPoint->getCoords());
  //if NdotU is 0, then the vector is parallel to plane
  if(nDotU > -.0000000001 && nDotU < .0000000001){
    return false;
  }
  //calc t
  t = (negD - nDotL)/nDotU;
  if(t< -.0001) return false;

  //if reached here w/out returning false, then the ray from the
  //pixel intersects the infinite plane containing the polygon:
  //set the intersection point and return true
  return true;
}//end plane intersection check


//plane containing polygon intersection
//check if ray interection is contained by edges of polygon
bool Polygon::checkPolyIntersect(Point* pixPoint, vector<double>& uVector, Point* intersectP){
  /*PROCESS
  for each edge:
    Epvj = intersect - Vj
    Np = Ej crossProd Epvj
    if NdotNp >= 0: intersect on correct side of Ej
    else: return false
  */
  //get this polygons vert count
  int vertCount = this->vertices.size();
  //vars
  vector<double> Epvj;
  vector<double> Ej;
  vector<double> Np;
  //loop through vertices
  //checking whether intersectP lies within each edge
  for(int j = 0; j < vertCount; j++){
    Epvj.clear(); //reset vectors
    Ej.clear(); //reset vectors
    Np.clear(); //reset vectors
    calcMat->vectorSub(vertices[j]->getCoords(), intersectP->getCoords(), Epvj);
    calcMat->vectorSub(vertices[j]->getCoords(), vertices[(j+1)%vertCount]->getCoords(), Ej);
    calcMat->crossProd(Ej, Epvj, Np);

    double nDotNp = calcMat->dotProd(surfNormal, Np);
    if(nDotNp < -.00001){
      return false;
    }
  }//end loop through edges
  //if reached here w/out returning false, then the ray from the
  //pixel intersects the polygon: return true
  return true;
}//end polygon intersection check



//set surface normal to Polygon
void Polygon::setSurfNorm(){
  //use calc matrix to use member fxns
  //get this polygons vert count
  int vertCount = this->vertices.size();
  // cout << "vertCount: " << vertCount << endl;//debugging
  int j = 0;
  vector<double> edge1;
  vector<double> edge2;
  vector<double> bigN;
  calcMat->vectorSub(vertices[j]->getCoords(),vertices[(j+1)%vertCount]->getCoords(), edge1);
  calcMat->vectorSub(vertices[(j+1)%vertCount]->getCoords(),vertices[(j+2)%vertCount]->getCoords(),edge2);
  calcMat->crossProd(edge1, edge2, bigN);
  //check if points are coparallel
  while(calcMat->checkZeroVec(bigN) == true){
    j++;
    //reset vectors for use with matrix math fxns
    edge1.clear();
    edge2.clear();
    bigN.clear();
    calcMat->vectorSub(vertices[j]->getCoords(),vertices[(j+1)%vertCount]->getCoords(), edge1);
    calcMat->vectorSub(vertices[(j+1)%vertCount]->getCoords(),vertices[(j+2)%vertCount]->getCoords(),edge2);
    calcMat->crossProd(edge1, edge2, bigN);
  }
  //normalize bigN, and set surfNormal for poly
  calcMat->makeUnit(bigN, surfNormal);

}//END SURFACE NORMAL TO POLYGON GETTER
