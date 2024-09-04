#ifndef B4cDetectorConstruction_h
#define B4cDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Material.hh"

#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

namespace B4c
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction() {}
    void InitializeThicknesses();
    std::vector<double> GetRockEndEdges();
    int GetNumberOfLayers();

  public:
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;
    G4double GetWorldZmin() { return fWorldZmin; }

  private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    G4PVPlacement* PlaceVolume(G4LogicalVolume* volume, G4LogicalVolume *mother = 0, G4double zShift = 0);
    G4LogicalVolume* GetVolume(G4String name, G4double sizeXY, G4double sizeZ, G4Material *material);
    G4Box* GetBox(G4String name, G4double sizeXY, G4double sizeZ);

    G4bool fCheckOverlaps = true;
    std::vector<G4double> fRockThickness;
    G4double fWorldZmin = 0;

    int nLayers;
    std::vector<double> fRockEndEdges;
};

}

#endif

