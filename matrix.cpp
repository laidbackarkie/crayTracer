/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4
date: 11.07.15
email: laidbackarkie@yahoo.com
*/
//include header
#include "matrix.h"

/*TODO:
  could test whether the vector is empty for unit,add,sub,xprod, scalar mult
  to do less push backs.  if it is then push, if not then calc with the vals there

*/

//vector magnitude fxn: squares all indices, sums, and sqroots
//returns a scalar that is the magnitude of this
double Matrix::magnitude(vector<double>& v){
  double mag = 0;
  //loop through vector, summing squares
  for(int i = 0; i < 3; i++){
    mag += v[i] * v[i];
  }
  //square-root sum and return
  return sqrt(mag);
}

//boolean fxn to determine if given vector a is 0-vector
//returns true if all elements are 0
//else returns false
bool Matrix::checkZeroVec(vector<double>& a){
  for(int i = 0; i< a.size(); i++){
    if(a[i] != 0.0) return false; //non-zero element, NOT ZERO VECTOR
  }
  //if cycled through all elements, then all are 0 so return true
  return true;
}

//vector fxn that makes the vector into a unit vector
//divides by it's magnitude, returns a vector of unit length
void Matrix::makeUnit(vector<double>& v, vector<double>& out){
  out.clear();
  out = vector<double>(3);
  double mag = this->magnitude(v);
  for(int i =0; i< 3; i++){
    out[i] = (v[i]/mag);
  }
}

//vector dot product fxn: mults all indices
//returns a scalar that is the dot product of a & b
//assumes that a and b are of equal length
double Matrix::dotProd(vector<double>& a, const vector<double>& b){
  double prod = 0.0;
  //loop through vector, summing squares
  for(int i = 0; i < 3; i++){
    prod += (a[i] * b[i]);
  }
  //square-root sum and return
  return prod;
}

//multiplies vector a by the scalar s
//returns a vector
void Matrix::scalarMult(double s, vector<double>& a, vector<double>& out){
  out = vector<double>(3);
  for(int i = 0; i < 3; i++){
    out[i] = (a[i] * s);
  }
}

//adds the vectors a and b
//returns a vector that is the sum between the two
//assumes that the two vectors are of equal length
void Matrix::vectorAdd(const vector<double>& a, const vector<double>& b, vector<double>& out){
  out = vector<double>(3);
  for(int i = 0; i < a.size(); i++){
    out[i] = (a[i] + b[i]);
  }
}

//subtracts the vectors b from a
//returns a vector that is the difference between the two
//assumes that the two vectors are of equal length
void Matrix::vectorSub(const vector<double>& a, const vector<double>& b, vector<double>& out){
  out = vector<double>(3);
  for(int i = 0; i < a.size(); i++){
    out[i] = (b[i] - a[i]);
  }
}

//vector cross-product method.
//takes 2 matrices (1 arg and this) and performs x-product EQ
//x-product EQ: ((ay*bz - by*az),(az*bx - bz*ax),(ax*by - bx*ay))
//returns a vector that is [A] X [B]
void Matrix::crossProd(const vector<double>& a, const vector<double>& b, vector<double>& out){
  out = vector<double>(3);
  //push back x,y,z for crossProd vector
  out[0] = ((a[1]*b[2]) - (b[1]*a[2]));
  out[1] = ((a[2]*b[0]) - (b[2]*a[0]));
  out[2] = ((a[0]*b[1]) - (b[0]*a[1]));
}

//matrix transpose method
//takes 'this'.elements and returns transpose of them
//assumes that the matrix is square
vector<vector<double> > Matrix::transpose(){
  vector<vector<double> > tMat;
  for(int i = 0; i < this->elements.size(); i++){
    vector<double> temp;
    temp.push_back(this->elements[0][i]);
    temp.push_back(this->elements[1][i]);
    temp.push_back(this->elements[2][i]);
    temp.push_back(this->elements[3][i]);
    tMat.push_back(temp);
  }
  return tMat;
}

//quadratic equation fxn used for sphere intersection
//if 'plus' == true then add sqrt
//else subtract sqrt
double Matrix::sphereQuadraticSolve(bool plus, vector<double>& uVec, vector<double>& tVec, double rad){
  double uDotT = this->dotProd(uVec, tVec);
  double tSquared = this->dotProd(tVec, tVec);
  double root = sqrt((uDotT*uDotT) - tSquared + (rad*rad));
  if(plus == true)
    return uDotT + root;
  else
    return uDotT - root;
}


//matrix multiply fxn, multiplies this by m
//'this' must be 4x4
//returns a matrix pointer
vector<vector<double> > Matrix::multiply(vector<vector<double> >& elements, int countFace){
  //cout << "numVert:" << countFace; //debugging
  vector<vector<double> > product = vector<vector<double> >(countFace, vector<double>(4));
  //go through multiplying i,j by j,i
  //cout << "numVert:" << countFace; //debugging
  for(int i = 0; i < 4; i++){
    //cout << "here1" << endl; //debugging
    for(int j = 0; j < countFace; j++){
      double sum = 0.0;
      for(int k = 0; k < 4; k++){
        //cout << "multiplying "<< this->elements[i][k] << " by " << elements[j][k]<< endl; //debugging
        sum += double(this->elements[i][k]) * double(elements[j][k]);
      }
      product[j][i] = sum;
    }//end vertex loop
  }
  // //debugging matrix
  // for(int i = 0; i < countFace; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << product[i][j] << " ";
  //   }
  //   cout << endl;
  // }//end debugging
  return product;
}//end normal matrix multiply method

//matrix multiply fxn, multiplies this by m
//'this' must be 4x4
//returns a matrix pointer
vector<vector<double> > Matrix::transMultiply(vector<vector<double> >& elements){
  //cout << "numVert:" << countFace; //debugging
  vector<vector<double> > product = vector<vector<double> >(4, vector<double>(4));
  //go through multiplying i,j by j,i
  //cout << "numVert:" << countFace; //debugging

  for(int i = 0; i < 4; i++){
    //cout << "here1" << endl; //debugging
    for(int j = 0; j < 4; j++){
      double sum = 0.0;
      for(int k = 0; k < 4; k++){
        //cout << "multiplying "<< this->elements[i][k] << " by " << elements[k][j]<< endl; //debugging
        sum += double(this->elements[i][k]) * double(elements[k][j]);
      }
      product[i][j] = sum;
    }//end vertex loop
  }
  // //debugging matrix
  // for(int i = 0; i < 4; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << product[i][j] << " ";
  //   }
  //   cout << endl;
  // }//end debugging
  return product;
}//end method to multiply 4x4 transformation matrices


//matrix scale, takes three coordinates
Matrix* Matrix::scale(double x, double y, double z){
  Matrix* trans = new Matrix();
  trans->elements = vector<vector<double> >(4, vector<double>(4));
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(i == 0 && j == 0) trans->elements[i][j] = double(x);
      else if(i ==1 && j ==1) trans->elements[i][j] = double(y);
      else if(i ==2 && j ==2) trans->elements[i][j] = double(z);
      else if(i ==3 && j ==3) trans->elements[i][j] = 1;
      else trans->elements[i][j] = 0;
    }
  }
  //check matrix debugging
  // for(int i = 0; i < 4; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << trans->elements[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  //end matrix check
  return trans;
}//end scale method

//matrix translate, takes three coordinates
Matrix* Matrix::translate(double x, double y, double z){
  Matrix* trans = new Matrix();
  trans->elements = vector<vector<double> >(4, vector<double>(4));
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(i==j) trans->elements[i][j] = 1;
      else if(i == 0 && j == 3) trans->elements[i][j] = double(x);
      else if(i ==1 && j ==3) trans->elements[i][j] = double(y);
      else if(i ==2 && j ==3) trans->elements[i][j] = double(z);
      else trans->elements[i][j] = 0;
    }
  }
  // //check matrix debugging
  // for(int i = 0; i < 4; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << trans->elements[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // //end matrix check
  return trans;
}

//matrix rotate, takes three coordinates (axis of rotation), and theta (degree of rotation)
Matrix* Matrix::rotate(double x, double y, double z, double theta){
  //prepare output matrix

  Matrix* projMatrix = new Matrix();
  projMatrix->elements = vector<vector<double> >(4, vector<double>(4));

  //rotate vector to make z axis axis of rotation
  //begin creating projection matrix
  vector<double> W;
  W.push_back(x);
  W.push_back(y);
  W.push_back(z);
  vector<double> wUnit;
  projMatrix->makeUnit(W, wUnit);
  for(int k = 0; k < W.size(); k++)
    cout << wUnit[k] << " ";
  cout << endl;
  //get any vector !parallel to W, make that M
  vector<double> M;
  M.push_back(x);
  M.push_back(y-1);
  M.push_back(z);
  vector<double> mUnit;
  projMatrix->makeUnit(M, mUnit);
  //get cross product of WXM, set equal to U
  vector<double> U;
  projMatrix->crossProd(W,M,U);
  vector<double> uUnit;
  projMatrix->makeUnit(U, uUnit);
  //get cross product of WXU, set equal to V
  vector<double> V;
  projMatrix->crossProd(wUnit,uUnit,V);
  //add the three vectors into the projection matrix
  projMatrix->elements[0] = V;  //should be unit
  projMatrix->elements[1] = uUnit;  //should be unit
  projMatrix->elements[2] = wUnit;
  projMatrix->elements[3][3] = 1;
  // //check matrix debugging
  // cout<< "proj matrix" << endl; //debugging
  // for(int i = 0; i < 4; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << projMatrix->elements[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // //end matrix check
  //rotate theta around given axis
  Matrix* rotMatrix = new Matrix();
  rotMatrix->elements = vector<vector<double> >(4, vector<double>(4));
  rotMatrix->elements[0][0] = cos((theta*double(M_PI))/double(180));
  rotMatrix->elements[0][1] = sin((theta*double(M_PI))/double(180));
  rotMatrix->elements[1][0] = -sin((theta*double(M_PI))/double(180));
  rotMatrix->elements[1][1] = cos((theta*double(M_PI))/double(180));
  rotMatrix->elements[2][2] = 1;
  rotMatrix->elements[3][3] = 1;
  // //check matrix debugging
  // cout<< "rot matrix" << endl; //debugging
  // for(int i = 0; i < 4; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << rotMatrix->elements[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // //end matrix check

  //undo zaxis normalization
  //multiply by the inverse of projMatrix (ie transpose b/c orthanormal)
  Matrix* projMatrixInv = new Matrix();
  projMatrixInv->elements = projMatrix->transpose();
  // //check matrix debugging
  // cout<< "projInverse matrix" << endl; //debugging
  // for(int i = 0; i < 4; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << projMatrixInv->elements[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // //end matrix check
  //multiply p' * r * p to get full matrix
  Matrix* endMat = new Matrix();
  endMat->elements = projMatrixInv->elements;
  endMat->elements = endMat->transMultiply(rotMatrix->elements);
  endMat->elements = endMat->transMultiply(projMatrix->elements);

  //after multiplying together clean up memory
  delete projMatrixInv;
  projMatrixInv = NULL;
  delete rotMatrix;
  rotMatrix = NULL;
  delete projMatrix;
  projMatrix = NULL;

  //return final (multiplied) rotation matrix
  return endMat;
}//end rotation method

//matrix project for cameraModels
//identity in upper 3x3, and then 1/length in [3][2]
Matrix* Matrix::camProj(double length){
  Matrix* retProj = new Matrix();
  retProj->elements = vector<vector<double> >(4, vector<double>(4));
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(i == 0 && j == 0) retProj->elements[i][j] = 1;
      else if(i ==1 && j ==1) retProj->elements[i][j] = 1;
      else if(i ==2 && j ==2) retProj->elements[i][j] = 1;
      else if(i ==3 && j ==2) retProj->elements[i][j] = double(1/length);
      else retProj->elements[i][j] = 0;
    }
  }
  // //check matrix debugging
  // for(int i = 0; i < 4; i++){
  //   for(int j = 0; j < 4; j++){
  //     cout << retProj->elements[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // //end matrix check
  return retProj;
}//end camProj method
