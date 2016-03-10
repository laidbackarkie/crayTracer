/*
Author: Adrion Q Arkenberg
Assignment: cs410 pa4
date: 10.24.15
email: laidbackarkie@yahoo.com
*/

#ifndef MATERIAL_H
#define MATERIAL_H

//get stl headers

//using namespace std;

class Material{
public:
  //constructors
  //default constructor sets surface to matte grey w/ no specular reflectance and completely opaque
  Material()
  : Kr(0.5), Kg(0.5), Kb(0.5), Ks(0.0), alpha(5), Kt(0.0)
  {}
  //specific constructor
  Material(double r, double g, double b, double shiny, int aVal, double transVal);

  //getters
  inline double getKr(){return Kr;}
  inline double getKg(){return Kg;}
  inline double getKb(){return Kb;}
  inline double getKs(){return Ks;}
  inline double getKt(){return Kt;}
  inline int getAlpha(){return alpha;}

  //setters
  inline void setKr(double x){Kr = x;}
  inline void setKg(double x){Kg = x;}
  inline void setKb(double x){Kb = x;}
  inline void setKs(double x){Ks = x;}
  inline void setKt(double x){Kt = x;}
  inline void setAlpha(int x){alpha = x;}


  //destructor
  ~Material(){
    //all data primitive and on stack will be cleaned up
  }

private:
  double Kr, Kg, Kb, Ks, Kt;
  int alpha;

};//end Material class

#endif//close guard and Material header
