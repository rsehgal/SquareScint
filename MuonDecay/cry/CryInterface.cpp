/*
**	Filename : CryInterface.cpp
**	2025-03-17
**	username : rsehgal
*/

#include "CRYGenerator.h"
#include "CRYSetup.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h> // For Ubuntu Linux

#include "Muon.h"
#include "CryInterface.h"

CryInterface::CryInterface()
{
  fMuon = new Muon;
 
  fInputFile.open("setup.file", std::ios::in);
  char buffer[1000];

  while (!fInputFile.getline(buffer, 1000).eof()) {
    fSetupString.append(buffer);
    fSetupString.append(" ");
  }


  fSetup = new CRYSetup(fSetupString, "./data");
  fGen = new CRYGenerator(fSetup);
  fEv = new std::vector<CRYParticle *>;

}

CryInterface::~CryInterface() {}

Muon *CryInterface::SampleMuon()
{

  fEv->clear();
  fGen->genEvent(fEv);

  for (unsigned j = 0; j < fEv->size(); j++) {
    CRYParticle *p = (*fEv)[j];
    if (std::string(CRYUtils::partName(p->id())) == "muon") {
      fMuon->Set(p->u(), p->w(), p->v(), p->ke(),p->t(),p->PDGid());
      break;
    }
  }
  return fMuon;
}
