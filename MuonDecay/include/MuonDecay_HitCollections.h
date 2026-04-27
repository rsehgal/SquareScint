/*
**	Filename : MuonDecay_HitCollections.h
**	2026-04-26
**	username : rsehgal
*/
#ifndef MuonDecay_HitCollections_h
#define MuonDecay_HitCollections_h

#include "G4THitsCollection.hh"
#include "MuonDecay_PMT_Hit.h"
#include "MuonDecay_Scint_Hit.h"

using PMT_HitCollection=G4THitsCollection<MuonDecay_PMT_Hit>;
using Scint_HitCollection=G4THitsCollection<MuonDecay_Scint_Hit>;

#endif
