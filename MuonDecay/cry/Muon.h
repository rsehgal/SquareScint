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
  double time;
  int pdgid;

  Muon()
  {
    angleX = 0.;
    angleY = 0.;
    angleZ = 0.;
    energy = 0.;
    time =0.;
    pdgid=-13;
  }

  Muon(Muon &m)
  {
    angleX = m.angleX;
    angleY = m.angleY;
    angleZ = m.angleZ;
    energy = m.energy;
    time =m.time;
    pdgid=m.pdgid;
  }

  Muon(double anglex, double angley, double anglez, double ener)
  {
    angleX = anglex;
    angleY = angley;
    angleZ = anglez;
    energy = ener;
    time = 0.;
    pdgid=-13;
  }

  void Set(double anglex, double angley, double anglez, double ener, double tm)
  {
    angleX = anglex;
    angleY = angley;
    angleZ = anglez;
    energy = ener;
    time = tm;
    pdgid=-13;
  }
  void Set(double anglex, double angley, double anglez, double ener, double tm,int pid)
  {
    angleX = anglex;
    angleY = angley;
    angleZ = anglez;
    energy = ener;
    time = tm;
    pdgid=pid;
  }

  void Print()
  {
    std::cout << "(" << angleX << " , " << angleY << " , " << angleZ << ") : Energy : " << energy << std::endl;
  }
};

#endif
