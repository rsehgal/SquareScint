/*
**	Filename : Muon.h
**	2025-03-17
**	username : rsehgal
*/
#ifndef Muon_h
#define Muon_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h> // For Ubuntu Linux

struct Muon {
  double angleX, angleY, angleZ;
  double energy;

  Muon()
  {
    angleX = 0.;
    angleY = 0.;
    angleZ = 0.;
    energy = 0.;
  }

  Muon(Muon &m)
  {
    angleX = m.angleX;
    angleY = m.angleY;
    angleZ = m.angleZ;
    energy = m.energy;
  }

  Muon(double anglex, double angley, double anglez, double ener)
  {
    angleX = anglex;
    angleY = angley;
    angleZ = anglez;
    energy = ener;
  }

  void Set(double anglex, double angley, double anglez, double ener)
  {
    angleX = anglex;
    angleY = angley;
    angleZ = anglez;
    energy = ener;
  }

  void Print()
  {
    std::cout << "(" << angleX << " , " << angleY << " , " << angleZ << ") : Energy : " << energy << std::endl;
  }
};

#endif
