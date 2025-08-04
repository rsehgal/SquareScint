/*
**	Filename : Histograms.h
**	2025-08-04
**	username : rsehgal
*/

#include <TH1F.h>
#include <vector>

#ifndef Histograms_h
#define Histograms_h

extern TH1F *histQ0;
extern TH1F *histQ1;
extern TH1F *histQ2;
extern TH1F *histQ3;
extern TH1F *histQ4;
extern TH1F *histQ5;
extern TH1F *histQ6;
extern TH1F *histQ7;

extern std::vector<TH1F*> vecOfHist;

#endif
