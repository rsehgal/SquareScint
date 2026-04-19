/*
**	Filename : NonSegmented_HitCollections.h
**	2025-07-02
**	username : rsehgal
*/
#ifndef NonSegmented_HitCollections_h
#define NonSegmented_HitCollections_h

#include "G4THitsCollection.hh"
#include "NonSegmented_PMT_Hit.h"
#include "NonSegmented_Slab_Hit.h"
using PMT_HitCollection  = G4THitsCollection<NonSegmented_PMT_Hit>;
using Slab_HitCollection = G4THitsCollection<NonSegmented_Slab_Hit>;

#endif
