/*
**	Filename : Histograms.cpp
**	2025-08-04
**	username : rsehgal
*/
#include <iostream>
#include <Histograms.h>

int nbins = 512;

TH1F *histQ0 = new TH1F("Q0", "Q0", nbins, 0, 16384);
TH1F *histQ1 = new TH1F("Q1", "Q1", nbins, 0, 16384);
TH1F *histQ2 = new TH1F("Q2", "Q2", nbins, 0, 16384);
TH1F *histQ3 = new TH1F("Q3", "Q3", nbins, 0, 16384);
TH1F *histQ4 = new TH1F("Q4", "Q4", nbins, 0, 16384);
TH1F *histQ5 = new TH1F("Q5", "Q5", nbins, 0, 16384);
TH1F *histQ6 = new TH1F("Q6", "Q6", nbins, 0, 16384);
TH1F *histQ7 = new TH1F("Q7", "Q7", nbins, 0, 16384);

std::vector<TH1F *> vecOfHist = {histQ0, histQ1, histQ2, histQ3, histQ4, histQ5, histQ6, histQ7};
