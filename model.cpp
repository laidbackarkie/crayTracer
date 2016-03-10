/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa2
date: 10.09.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "model.h"

using namespace std;

/*TODO
    update specific constructor
    update rest of methods accordingly
    ??fix model printer?? (requires use of the original int vector face scheme again)
--------
    test
    remove debugging

*/
//SPECIFIC MODEL CONSTRUCTOR------------------------------------
//takes in a file and processes it, producing a model in end
 Model::Model(ifstream& inStr){
   //vars
   string currLine, garbage;
   int vCount;
   int faceCount;
   double currVertVal;
   //begin processing ply file
   inStr>>currLine;
   if(currLine != "ply" || inStr.fail()) this->setValidity(false);
   while(currLine.find("end_header") == -1){
     getline(inStr, currLine);

     //get vertex count
     if(currLine.find("element vertex") != -1){
       //toss the two keywords and grab #vertices
       this->header.push_back(currLine);
       currLine.erase(0,14);
       stringstream lineHolder;
       lineHolder.str(string());
       lineHolder.str(currLine);

       lineHolder >> vCount;
       this->setVertCount(vCount);
     }
     //get face count
     else if(currLine.find("element face") != -1){
       //toss the two keywords and grab #faces
       this->header.push_back(currLine);
       currLine.erase(0,12);
       stringstream lineHolder1;
       lineHolder1.str(string());
       lineHolder1.str(currLine);

       lineHolder1 >> faceCount;
       this->setFaceCount(faceCount);
     }
     else{
       if(!(currLine.find("property float") != -1 && currLine.length() != 18 && currLine.length() != 16))
        this->header.push_back(currLine);
     }
   }//END HEADER AQUIRING
   //cout << currLine << endl; //debugging (should be end_header)

   //begin getting vertices
   for(int i = 0; i < vCount; i++){
     //cout << i <<": "; //debugging
     inStr >> currVertVal;
     this->xVals.push_back(currVertVal);
     //cout << currVertVal <<","; //debugging
     inStr >> currVertVal;
     this->yVals.push_back(currVertVal);
     //cout << currVertVal <<","; //debugging
     inStr >> currVertVal;
     this->zVals.push_back(currVertVal);
     //cout << currVertVal <<endl; //debugging
     //toss the rest of the line out
     char trash[100];
  	 inStr.getline(trash, 100);
   }//vertices obtained
   //turn vertices into points and combine into x,y,z vals
   this->setVertList();
   //now that point list is made, create model's sphere
   this->modelSphere = new Sphere(this->vertices);


   //begin getting faces
   //create each face and pushback polygon
   for(int j = 0; j < faceCount; j++){
     //var declarations
     int currFaceVal;
     //string currLine;
     //get line, and extract faces from it, NOT like this
     getline(inStr, currLine);

     stringstream lineHolder2;
     lineHolder2.str(string());
     lineHolder2.str(currLine);
     //create vector to hold point vals until turned to polygons
     vector<Point*> currFace;
     //toss out first number on line (gives number of verts in poly)
     lineHolder2 >>currFaceVal;
     while(!lineHolder2.eof()){
       //get all face numbers until line's done
       lineHolder2 >> currFaceVal;
       //if(lineHolder2.eof()) break;
       //cout << currFaceVal << ",";//debugging
       //add vertex# (point) to current face
       currFace.push_back(this->vertices[currFaceVal]);
     }
     //add the the polygon made from the face's points to model
     this->faces.push_back(new Polygon(currFace));
   }//end getting polygons/faces
    //everything when fine, valid model
     this->setValidity(true);
}//END SPECIFIC CONSTRUCTOR---------------------------------------------

//fxn to put model's x, y, and z vals into a single list
void Model::setVertList(){
  vertices = vector<Point*>(numVertices);
  for(int k = 0; k < this->getVertCount(); k++){
    vector<double> curr(3);
    curr[0] = (this->xVals[k]);
    curr[1] = (this->yVals[k]);
    curr[2] = (this->zVals[k]);
    //add set of x,y,z,1 to vertList
    this->vertices[k] = (new Point(curr));
  }
}

//method that takes in the material vals
//applies materials to model's polygons
//assumes that vals were checked upon reading, so none are made
void Model::setMaterial(int start, int stop, double x, double y, double z, double s, int exp, double trans){
  //loop through all polygons involved
  for(int i = start; i <= stop; i++){
    faces[i]->setSurfaceMaterial(x,y,z,s,exp,trans);
  }//end poly loop
}//end material setter for model

//method to print model's bounding box
void Model::printBBox(){
  //PA1 DATA PRINTING (used for debugging)
  //print to console: #vertices, #faces, meanVertex, boundingBox of each model read-in
  cout << "--------------modelBBox----------------------"<<endl;
  cout << "vertex count: " << this->getVertCount() << endl;
  cout << "face count: " << this->getFaceCount() << endl;
  cout << "mean vertex: (" << this->getMeanVertex(this->xVals) << ","<< this->getMeanVertex(this->yVals)
        << ","<< this->getMeanVertex(this->zVals) << ")" << endl;
  cout << "bounding box:: (min,max)" << endl;
  cout << "    x: (" << this->getMin(this->xVals) << "," << this->getMax(this->xVals) << ")"<<endl;
  cout << "    y: (" << this->getMin(this->yVals) << "," << this->getMax(this->yVals) << ")"<<endl;
  cout << "    z: (" << this->getMin(this->zVals) << "," << this->getMax(this->zVals) << ")"<<endl;
  cout << "-------------------------------------------"<<endl;
  //END PA1 DATA PRINTING
}


//mean vertex getters
double Model::getMeanVertex(vector<double>& points){
  double avg = 0.0;
  for(int i = 0; i < points.size(); i++){
    avg += points[i];
    //cout<< avg << endl; //debugging
  }
  avg /= double(points.size()); //divide by numVertices
  return avg;
}//end avg



//bounding box fxns
double Model::getMin(vector<double>& points){
  double min = points[0];
  for(int i = 0; i < points.size(); i++){
    if(points[i] < min) min = points[i];
  }
  return min;
}//end min
double Model::getMax(vector<double>& points){
  double max = points[0];
  for(int i = 0; i < points.size(); i++){
    if(points[i] > max) max = points[i];
  }
  return max;
}//end max
