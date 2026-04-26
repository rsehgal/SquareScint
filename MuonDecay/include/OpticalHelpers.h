/*
**	Filename : OpticalHelpers.h
**	2026-04-25
**	username : rsehgal
*/
#ifndef OpticalHelpers_h
#define OpticalHelpers_h

class G4OpticalSurface;
class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4String;

extern G4OpticalSurface *GetOpticalSurface();
extern void AttachOpticalProperties(G4Material *scintMat);
extern G4OpticalSurface* GetInterfacingSurface();
extern void AddReflectiveWrapping(G4LogicalVolume *logVolume);
extern void AddOpticalGreaseBetweenVolumes(G4String name,G4VPhysicalVolume *vol1, G4VPhysicalVolume *vol2);
extern G4LogicalVolume *GetPMT();
#endif
