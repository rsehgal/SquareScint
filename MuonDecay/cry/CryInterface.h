/*
**	Filename : CryInterface.h
**	2025-03-17
**	username : rsehgal
*/
#ifndef CryInterface_h
#define CryInterface_h

#include "Muon.h"
#include "CRYGenerator.h"
#include "CRYSetup.h"

class CryInterface {

  Muon *fMuon;
  std::ifstream fInputFile;
  std::string fSetupString;
  CRYSetup *fSetup;
  CRYGenerator *fGen;
  std::vector<CRYParticle*> *fEv;
public:
  CryInterface();

  ~CryInterface();

  Muon* SampleMuon();
};
#endif
