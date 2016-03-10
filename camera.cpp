/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4
date: 10.24.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "camera.h"
#include "matrix.h"

using namespace std;

/*TODO
  check that lookat and N arent parallel
  should i be using epsilon on color tracer??
  -----------------------------------------------------

*/

//----------------------------CAMERA constructor
//assumes the file has already been openned and checked
Camera::Camera(ifstream& camFile){
  //vars
  double dHolder;
  int iHolder;
  vector<double> fpHolder;
  vector<double> LAPHolder;
  vector<double> VUPHolder;

  for(int i = 0; i < 3; i++){//get the focal point coordinates
    camFile >> dHolder;
    if(camFile.fail()) validCam = false;
    fpHolder.push_back(dHolder);
  }
  FP = new Point(fpHolder);//create FP

  for(int i = 0; i < 3; i++){//get the look at points coords
    camFile >> dHolder;
    if(camFile.fail()) validCam = false;
    LAPHolder.push_back(dHolder);
  }
  lookAt = new Point(LAPHolder);//create look at point

  for(int i = 0; i < 3; i++){//get the VUP vector
    camFile >> dHolder;
    if(camFile.fail()) validCam = false;
    VUPHolder.push_back(dHolder);
  }
  VUP = new Point(VUPHolder);//create VUP vector(as point)

  camFile >> focalLength;  //get focal length
  if(camFile.fail()) validCam = false;
  else if(focalLength <= 0){ //check focal length is valid
    cout << "error: impossible camera due to focal length" << endl;
    validCam = false;
  }

  //get the image window values
  camFile >> Umin;
  if(camFile.fail()) validCam = false;
  camFile >> Vmin;
  if(camFile.fail()) validCam = false;
  camFile >> Umax;
  if(camFile.fail()) validCam = false;
  camFile >> Vmax;
  if(camFile.fail()) validCam = false;

  //check that window is properly formatted
  if((Umax < Umin) || (Vmax < Vmin)){
    cout << "error: impossible camera due to image window" << endl;
    validCam = false;
  }
  //check that fp and lookAt aren't the same
  if(FP->getCoords() == lookAt->getCoords()){
    cout << "error: impossible camera due to fp == l.@.p" << endl;
    validCam = false;
  }
  //fill in n, u, and v unit vectors
  this->createCamProj();
  //check that nUnit and VUP aren't the same
  if(VUP->getCoords() == nUnit){
    cout << "error: impossible camera due to VUP == N" << endl;
    validCam = false;
  }
  //cam is fine, set valid to true
  validCam = true;
  //create camera's Image
  scene = new Image();
  //set image width and height
  scene->setWidth(Vmax - Vmin+1);
  scene->setHeight(Umax - Umin+1);
  //create calculation matrix
  calcMat = new Matrix();
  //close file containing camera
  camFile.close();
}//end specific constructor

//method to get u, v, and n vectors of camera
void Camera::createCamProj(){
  //vars
  Matrix* holderMat = new Matrix(); //just a matrix used for calling member fxns of matrix (NO DATA)

  //CHECK THAT N AND LOOKAT ARENT PARALLEL

  //get n and normalize
  vector<double> VPN;
  for(int i = 0; i < 3; i++)
    VPN.push_back(lookAt->getCoords()[i]-FP->getCoords()[i]);
  holderMat->makeUnit(VPN,nUnit);
  //get u and normalize
  vector<double> UPcrossN;
  holderMat->crossProd(VUP->getCoords(), nUnit, UPcrossN);
  holderMat->makeUnit(UPcrossN, uUnit);
  //get v and normalize
  vector<double> NcrossU;
  holderMat->crossProd(nUnit, uUnit, NcrossU);
  holderMat->makeUnit(NcrossU,vUnit);

  //clean up memory
  delete holderMat;
  holderMat = NULL;
}//end camera u,v,n getter/setter

//method that takes in the material file
//extracts and creates lights in file
bool Camera::getLights(ifstream& lightFile){
  //vars
  string currLine;
  char firstLetter;
  double red, green, blue, x, y, z;
  //begin processing material file
  getline(lightFile, currLine);
  while(!lightFile.eof()){
    //getline(lightFile, currLine);
    //cout << currLine << endl;//debugging
    if(currLine.at(0) == 'L' || currLine.at(0) == 'l'){
      stringstream lineHolder;
      lineHolder.str(string());
      lineHolder.str(currLine);
      lineHolder >> firstLetter;
      lineHolder >> red;
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(red > 1 || red < 0) return false;
      lineHolder >> green;
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(green > 1 || green < 0) return false;
      lineHolder >> blue;
      if(lineHolder.fail()) return false;
      //check that 0<=#<=1
      if(blue > 1 || blue < 0) return false;

      //check if light is ambient or not
      if(currLine.at(currLine.length()-1) == 'A' || currLine.at(currLine.length()-2) == 'A'){ //light is ambient
        //cout << "ambLight"<<endl;//debugging
        //add light to camera's light list
        this->lights.push_back(new Light(red, green, blue));
      }
      else{                       //light is NOT ambient: has position
        //cout << "posLight"<<endl;//debugging
        lineHolder >> x;
        if(lineHolder.fail()) return false;
        lineHolder >> y;
        if(lineHolder.fail()) return false;
        lineHolder >> z;
        if(lineHolder.fail()) return false;
        //add light to camera's light list
        this->lights.push_back(new Light(red, green, blue, new Point(x, y, z)));
      }
      //done with line, close lineHolder
      //lineHolder.close();
    }//end case that line was a light
    //get next line and loop
    getline(lightFile,currLine);
  }//end of file
  lightFile.close();
  //no problems have occurred return true
  return true;
}//end camera light getter

//method that does pa3's pixel color calculation
//takes in a polygon and the point that the ray hit that polygon
/*
  calc V (-1U)
  check NdotV >= 0 (else N=-1N)
  for each light:
    if ambient: calc Kd*ba
    else:
      calc L and normalize
      calc NdotL (if < 0 set to 0)
      else:
      check if L intersects other polys
        if so: NdotL = 0
        else:
          calc R
          calc VdotR (if < 0 set to 0)
          calc KdBi * (LdotN) //diffuse
          calc KsBi * (VdotR) //specular phong
          add diffuse and specular to sum
*/
vector<double> Camera::tracePixelColor(Point* intersectP, Polygon* hitPoly, vector<double>& Uvector,
                                vector<Model*>& models, int modNum, int polNum, double currKs, double currKt, int recCount){
  vector<double> sum(3,0.0); //initialize sum
  vector<double> zero(3,0.0);
  vector<double> V;
  calcMat->scalarMult(-1, Uvector, V);
  vector<double> N = hitPoly->getSurfNorm();
  double NdotV = calcMat->dotProd(N, V);
  if(NdotV < 0){
    vector<double> temp;
    calcMat->scalarMult(-1, N, temp);
    N = temp;
  }
  //loop through lights
  for(int i = 0; i < lights.size(); i++){
    if(lights[i]->getAmbient() == true){ //case that the light was ambient
      vector<double> ambientLight(3);
      ambientLight[0] = (hitPoly->getMaterial()->getKr() * lights[i]->getR());
      ambientLight[1] = (hitPoly->getMaterial()->getKg() * lights[i]->getG());
      ambientLight[2] = (hitPoly->getMaterial()->getKb() * lights[i]->getB());
      //add ambient light into color sum
      vector<double> tempSum;
      calcMat->vectorAdd(ambientLight, sum, tempSum);
      sum = tempSum;
    }//end case AMBIENT LIGHT
    else{  //case that the light was a point light
      vector<double> pointLight(3,0.0);
      vector<double> diffuse, specular;
      vector<double> tVals;
      //calc L = (Pl - POI)/||(Pl - POI)||
      vector<double> toLight;
      calcMat->vectorSub(intersectP->getCoords(), lights[i]->locate()->getCoords(), toLight);
      vector<double> L;//normalized L vector
      calcMat->makeUnit(toLight, L);
      double lightTval = calcMat->magnitude(toLight);//get light's t val to compare later
      //check if NdotL is less than 0
      double NdotL = calcMat->dotProd(N, L);
      if(NdotL < 0) NdotL = 0;
      else{//check if L intersects any other polygon
        for(int j = 0; j < models.size(); j++){
          if(models[j]->getSphere()->checkSphereIntersect(intersectP, L) == true){
            //for each polygon in model
            for(int k = 0; k < models[j]->getFaces().size(); k++){
              //check that not looking at our poly
              if(!(j == modNum && k == polNum)){
                //check sphere intersection for each polygon
                if(models[j]->getFaces()[k]->getSphere()->checkSphereIntersect(intersectP, L) == true){
                  double currTVal;
                  //check plane intersection-- sets the t val that corresponds to intersection point with plane
                  if(models[j]->getFaces()[k]->checkPlaneIntersect(intersectP, L, currTVal) == true){
                      //plug t into eq to get actual intersection point
                      vector<double> tL;
                      calcMat->scalarMult(currTVal, L, tL);
                      vector<double> POIplustL;
                      calcMat->vectorAdd(intersectP->getCoords(), tL, POIplustL);
                      Point* newPoi = new Point(POIplustL);
                      //check polygon intersection
                      if(models[j]->getFaces()[k]->checkPolyIntersect(intersectP, L, newPoi) == true){
                        //ray intersected with polygon, store t
                        delete newPoi;
                        if(currTVal < lightTval && currTVal > 0){
                          NdotL = 0;
                          break;
                        }
                      }// end case of polygon intersection
                      else delete newPoi; //case that point was outside poly on plane, clean up memory
                  }// end case of plane intersection
                }// end case of sphere intersection with poly
              }//end check for same poly that is 'hitPoly'
            }// end polygon loop
          }//end case of sphere intersection with model
          if(NdotL == 0) break; //CATCH CASE THAT RAY HIT ANOTHER POLY AND STOP CHECKING
        }// end modelLoop
      }//end polygon intersection check (for L)

      //calc R = (2(NdotL)N -L)
      vector<double> rLeft;
      calcMat->scalarMult((2*NdotL), N, rLeft);
      vector<double> R;
      calcMat->vectorSub(L, rLeft, R);
      //calc VdotR and check
      double VdotR = calcMat->dotProd(V, R);
      if(VdotR < 0) VdotR = 0;
      //CALCULATE DIFFUSE AND SPECULAR REFLECTION
      vector<double> KdB(3);
      KdB[0] = (hitPoly->getMaterial()->getKr() * lights[i]->getR());
      KdB[1] = (hitPoly->getMaterial()->getKg() * lights[i]->getG());
      KdB[2] = (hitPoly->getMaterial()->getKb() * lights[i]->getB());
      calcMat->scalarMult(NdotL, KdB, diffuse);

      vector<double> KsB(3);
      KsB[0] = (hitPoly->getMaterial()->getKs() * lights[i]->getR());
      KsB[1] = (hitPoly->getMaterial()->getKs() * lights[i]->getG());
      KsB[2] = (hitPoly->getMaterial()->getKs() * lights[i]->getB());
      double VdotRtoAlpha = pow(VdotR, hitPoly->getMaterial()->getAlpha());
      calcMat->scalarMult(VdotRtoAlpha, KsB, specular);
      //END CALC OF DIFFUSE AND SPECULAR REFLECTION

      vector<double> interm;
      calcMat->vectorAdd(diffuse, pointLight, interm);
      pointLight.clear();
      calcMat->vectorAdd(specular, interm, pointLight);
      //add point light into color sum
      vector<double> tempSum1;
      calcMat->vectorAdd(pointLight, sum, tempSum1);
      sum = tempSum1;
    }//end case POINT LIGHT
  }//end light loop

  //inter reflection recursion if necessary
  if(hitPoly->getMaterial()->getKs() > 0){
    if((currKs*hitPoly->getMaterial()->getKs()) < double(1/512)) ;//do nothing value too small
    else if(recCount >= 20) ;//house of mirror case, stop adding in color
    else{
      //calc Rv = (2(VdotN)N -V)
      vector<double> rvLeft;
      double VdotN = calcMat->dotProd(V, N);
      calcMat->scalarMult((2*VdotN), N, rvLeft);
      vector<double> Rv;
      calcMat->vectorSub(V, rvLeft, Rv);

      //create vectors to hold list of tVals, and corresponding mod# & poly# for intersections
      vector<double> reflectTVals;
      vector<int> reflectModNumbers;
      vector<int> reflectPolyNumbers;
      vector<Point*> reflectIntersections;

      //check intersection
      for(int j = 0; j < models.size(); j++){
        if(models[j]->getSphere()->checkSphereIntersect(intersectP, Rv) == true){
          //for each polygon in model
          for(int k = 0; k < models[j]->getFaces().size(); k++){
            //check that not looking at our poly
            if(!(j == modNum && k == polNum)){
              //check sphere intersection for each polygon
              if(models[j]->getFaces()[k]->getSphere()->checkSphereIntersect(intersectP, Rv) == true){
                double currRefTVal;
                //check plane intersection-- sets the t val that corresponds to intersection point with plane
                if(models[j]->getFaces()[k]->checkPlaneIntersect(intersectP, Rv, currRefTVal) == true){
                    //plug t into eq to get actual intersection point
                    vector<double> tRv;
                    calcMat->scalarMult(currRefTVal, Rv, tRv);
                    vector<double> POIplusReflect;
                    calcMat->vectorAdd(intersectP->getCoords(), tRv, POIplusReflect);
                    Point* reflectPoi = new Point(POIplusReflect);
                    //check polygon intersection
                    if(models[j]->getFaces()[k]->checkPolyIntersect(intersectP, Rv, reflectPoi) == true){
                      //ray intersected with polygon, store t
                      reflectTVals.push_back(currRefTVal);
                      reflectModNumbers.push_back(j);
                      reflectPolyNumbers.push_back(k);
                      reflectIntersections.push_back(reflectPoi);
                      //delete reflectPoi;
                    }// end case of polygon intersection
                    else delete reflectPoi; //case that point was outside poly on plane, clean up memory
                }// end case of plane intersection
              }// end case of sphere intersection with poly
            }//end check for same poly that is 'hitPoly'
          }// end polygon loop
        }//end case of sphere intersection with model
      }// end modelLoop

      //done with modelLoop for this pixel, all tVals aquired, find smallest tVal > 0;
      if(reflectTVals.size() > 0){
        int index = 0;
        double smallest = reflectTVals[0];
        for(int i = 0; i < reflectTVals.size(); i++){
          if(reflectTVals[i] <= smallest){
            smallest = reflectTVals[i];
            index = i;
          }
        }//found smallest tVal for pixel
      // Point* rayPOI = new Point();
      // int modelNum, pNum;
      // if(world->intersectRay(pixelPoint, uVecUnit, rayPOI, modelNum, pNum) == true){

        //calculate color vector of this pixel using smallest tVal's intersection, and polygon intersection is with
        vector<double> reflectColorTemp = tracePixelColor(reflectIntersections[index], models[reflectModNumbers[index]]->getFaces()[reflectPolyNumbers[index]], Rv,
                                                                models, reflectModNumbers[index], reflectPolyNumbers[index],
                                                                (currKs * models[reflectModNumbers[index]]->getFaces()[reflectPolyNumbers[index]]->getMaterial()->getKs()),
                                                                (currKt * models[reflectModNumbers[index]]->getFaces()[reflectPolyNumbers[index]]->getMaterial()->getKt()), recCount+1);
        vector<double> reflectColor;
        calcMat->scalarMult(currKs, reflectColorTemp, reflectColor);
        vector<double> reflectTemp;
        calcMat->vectorAdd(sum, reflectColor, reflectTemp);
        sum = reflectTemp;

        //clean up memory
        for(int i = 0; i < reflectIntersections.size(); i++){
          delete reflectIntersections[i];
        }
      //}
    }//end case reflection ray intersected a polygon
    else ; //case that ray didn't intersect a poly, don't add into sum
    }//end case of checking interReflection intersection---------------------------
  }//end case that hitPoly has Ks > 0



  //translucent refraction if necessary
  if(hitPoly->getMaterial()->getKt() > 0){
    if((currKt * hitPoly->getMaterial()->getKt()) < double(1/512)) ;//do nothing value too small
    else if(recCount >= 20) ;//house of mirror case, stop adding in color
    else{
      //create intesection arrays
      vector<double> refractTVals;
      vector<int> refractModNumbers;
      vector<int> refractPolyNumbers;
      vector<Point*> refractIntersections;

      for(int j = 0; j < models.size(); j++){
        if(models[j]->getSphere()->checkSphereIntersect(intersectP, Uvector) == true){
          //for each polygon in model
          for(int k = 0; k < models[j]->getFaces().size(); k++){
            //check that not looking at our poly
            if(!(j == modNum && k == polNum)){
              //check sphere intersection for each polygon
              if(models[j]->getFaces()[k]->getSphere()->checkSphereIntersect(intersectP, Uvector) == true){
                double currRefractTVal;
                //check plane intersection-- sets the t val that corresponds to intersection point with plane
                if(models[j]->getFaces()[k]->checkPlaneIntersect(intersectP, Uvector, currRefractTVal) == true){
                  //plug t into eq to get actual intersection point
                  vector<double> tU;
                  calcMat->scalarMult(currRefractTVal, Uvector, tU);
                  vector<double> POIplusRefract;
                  calcMat->vectorAdd(intersectP->getCoords(), tU, POIplusRefract);
                  Point* refractPoi = new Point(POIplusRefract);
                  //check polygon intersection
                  if(models[j]->getFaces()[k]->checkPolyIntersect(intersectP, Uvector, refractPoi) == true){
                    //ray intersected with polygon, store t
                    refractTVals.push_back(currRefractTVal);
                    refractModNumbers.push_back(j);
                    refractPolyNumbers.push_back(k);
                    refractIntersections.push_back(refractPoi);
                    //delete refractPoi;
                  }// end case of polygon intersection
                  else delete refractPoi; //case that point was outside poly on plane, clean up memory
                }// end case of plane intersection
              }// end case of sphere intersection with poly
            }//end check for same poly that is 'hitPoly'
          }// end polygon loop
        }//end case of sphere intersection with model
      }// end modelLoop

      //done with modelLoop for this pixel, all tVals aquired, find smallest tVal > 0;
      if(refractTVals.size() > 0){
        int index = 0;
        double smallest = refractTVals[0];
        for(int i = 0; i < refractTVals.size(); i++){
          if(refractTVals[i] <= smallest){
            smallest = refractTVals[i];
            index = i;
          }
        }//found smallest tVal for pixel
      // Point* rayPOI = new Point();
      // int modelNum, pNum;
      // if(world->intersectRay(pixelPoint, uVecUnit, rayPOI, modelNum, pNum) == true){

        //calculate color vector of this pixel using smallest tVal's intersection, and polygon intersection is with
        vector<double> refractColorTemp = tracePixelColor(refractIntersections[index], models[refractModNumbers[index]]->getFaces()[refractPolyNumbers[index]], Uvector,
                                                                models, refractModNumbers[index], refractPolyNumbers[index],
                                                                (currKs * models[refractModNumbers[index]]->getFaces()[refractPolyNumbers[index]]->getMaterial()->getKs()),
                                                                 (currKt * models[refractModNumbers[index]]->getFaces()[refractPolyNumbers[index]]->getMaterial()->getKt()), recCount+1);
        vector<double> refractColor;
        calcMat->scalarMult(currKt, refractColorTemp, refractColor);
        vector<double> refractTemp;
        calcMat->vectorAdd(sum, refractColor, refractTemp);
        sum = refractTemp;

        //clean up memory
        for(int i = 0; i < refractIntersections.size(); i++){
          delete refractIntersections[i];
        }
      //}
      }//end case refraction ray intersected a polygon
      else ;//ray didn't intersect a polygon, don't add to sum
      }//end case of refraction ray intersecting a polygon
    }//end case that polygon is translucent (Kt > 0)

  //return the aggregate of all lights for that pixel
  //plus the final sum value after all recursion
  return sum;
}//end color calculator (color tracing)
