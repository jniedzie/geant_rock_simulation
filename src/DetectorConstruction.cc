#include "DetectorConstruction.hh"

#include "CalorimeterSD.hh"
#include "G4AnalysisManager.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"

namespace B4c {
G4VPhysicalVolume* DetectorConstruction::Construct() {
  InitializeThicknesses();
  DefineMaterials();
  return DefineVolumes();
}

void DetectorConstruction::InitializeThicknesses() {
  fRockThickness.clear();

  // for pions
  // for(double thickness=0.0002 ; thickness<0.002 ; thickness+=0.002) fRockThickness.push_back(thickness*m);
  // for(double thickness=0.002 ; thickness<0.1 ; thickness+=0.002) fRockThickness.push_back(thickness*m);
  // for(double thickness=0.1 ; thickness<1 ; thickness+=0.2) fRockThickness.push_back(thickness*m);
  // for(double thickness=1 ; thickness<10 ; thickness+=2) fRockThickness.push_back(thickness*m);
  // for(double thickness=10; thickness<200; thickness+=10) fRockThickness.push_back(thickness*m);
  for (double thickness = 0.002; thickness < 1; thickness += 0.002) fRockThickness.push_back(thickness * m);
  for (double thickness = 1; thickness < 200; thickness += 1) fRockThickness.push_back(thickness * m);

  // for muons
  // for(double thickness=1 ; thickness<100 ; thickness+=1  ) fRockThickness.push_back(thickness*m);
  // for(double thickness=100 ; thickness<1000 ; thickness+=10  ) fRockThickness.push_back(thickness*m);
  // for(double thickness=1000; thickness<10000; thickness+=100 ) fRockThickness.push_back(thickness*m);
}

void DetectorConstruction::DefineMaterials() {
  auto nistManager = G4NistManager::Instance();

  auto oxygen = nistManager->FindOrBuildMaterial("G4_O");
  auto magnesium = nistManager->FindOrBuildMaterial("G4_Mg");
  auto calcium = nistManager->FindOrBuildMaterial("G4_Ca");
  auto silicon = nistManager->FindOrBuildMaterial("G4_Si");
  auto aluminium = nistManager->FindOrBuildMaterial("G4_Al");
  auto ferrum = nistManager->FindOrBuildMaterial("G4_Fe");
  auto sodium = nistManager->FindOrBuildMaterial("G4_Na");
  auto potassium = nistManager->FindOrBuildMaterial("G4_K");
  auto titanium = nistManager->FindOrBuildMaterial("G4_Ti");
  auto hydrogen = nistManager->FindOrBuildMaterial("G4_H");
  auto air = nistManager->FindOrBuildMaterial("G4_AIR");

  nistManager->FindOrBuildMaterial("G4_Galactic");

  new G4Material("SimpleStandardRock", 11, 22, 2.65 * g / cm3);
  auto standardRock = new G4Material("StandardRock", 2.65 * g / cm3, 11, kStateSolid);
  standardRock->AddMaterial(oxygen, 46.6 * perCent);
  standardRock->AddMaterial(silicon, 27.7 * perCent);
  standardRock->AddMaterial(aluminium, 8.1 * perCent);
  standardRock->AddMaterial(ferrum, 5.0 * perCent);
  standardRock->AddMaterial(calcium, 3.6 * perCent);
  standardRock->AddMaterial(sodium, 2.8 * perCent);
  standardRock->AddMaterial(potassium, 2.6 * perCent);
  standardRock->AddMaterial(magnesium, 2.1 * perCent);
  standardRock->AddMaterial(titanium, 0.5 * perCent);
  standardRock->AddMaterial(hydrogen, 0.2 * perCent);
  standardRock->AddMaterial(air, 0.8 * perCent);  // just adding some air to make it sum up to 100%
}

std::vector<double> DetectorConstruction::GetRockEndEdges() {
  InitializeThicknesses();

  G4double detectorThickness = 1. * mm;
  G4double transverseSize = 1000. * m;

  G4double rockPlusDetectorsThickness = fRockThickness[fRockThickness.size() - 1] + fRockThickness.size() * detectorThickness;

  auto worldSizeXY = 1.2 * transverseSize;
  auto worldSizeZ = 1.2 * rockPlusDetectorsThickness * 2;
  fWorldZmin = worldSizeZ / 2;

  G4double rockBeginning = 0;
  G4double thicknessSoFar = 0;

  fRockEndEdges.clear();

  for (int iRock = 0; iRock < (int)fRockThickness.size(); iRock++) {
    auto thisRockThickness = fRockThickness[iRock] - thicknessSoFar;
    thicknessSoFar += thisRockThickness + detectorThickness;
    fRockEndEdges.push_back(rockBeginning + thisRockThickness);
    rockBeginning += thisRockThickness + detectorThickness;
  }

  return fRockEndEdges;
}

int DetectorConstruction::GetNumberOfLayers() {
  InitializeThicknesses();
  return fRockThickness.size();
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {
  G4double detectorThickness = 1. * mm;
  G4double transverseSize = 1000. * m;

  G4cout << "Total rock thickness: " << G4BestUnit(fRockThickness[fRockThickness.size() - 1], "Length") << G4endl;

  G4double rockPlusDetectorsThickness = fRockThickness[fRockThickness.size() - 1] + fRockThickness.size() * detectorThickness;

  auto worldSizeXY = 1.2 * transverseSize;
  auto worldSizeZ = 1.2 * rockPlusDetectorsThickness * 2;
  fWorldZmin = worldSizeZ / 2;

  auto vacuum = G4Material::GetMaterial("G4_Galactic");
  auto standardRock = G4Material::GetMaterial("StandardRock");

  auto worldVolume = GetVolume("World", worldSizeXY, worldSizeZ, vacuum);
  auto worldPV = PlaceVolume(worldVolume);

  G4double rockBeginning = 0;
  G4double thicknessSoFar = 0;

  // Visualization attributes
  worldVolume->SetVisAttributes(G4VisAttributes::GetInvisible());
  auto simpleBoxVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  simpleBoxVisAtt->SetVisibility(true);

  for (int iRock = 0; iRock < (int)fRockThickness.size(); iRock++) {
    auto thisRockThickness = fRockThickness[iRock] - thicknessSoFar;
    thicknessSoFar += thisRockThickness + detectorThickness;
    auto rockVolume = GetVolume("Rock_" + std::to_string(iRock), transverseSize, thisRockThickness, standardRock);
    auto detectorVolume = GetVolume("Detector_" + std::to_string(iRock), transverseSize, detectorThickness, vacuum);

    PlaceVolume(rockVolume, worldVolume, rockBeginning + thisRockThickness / 2);
    PlaceVolume(detectorVolume, worldVolume, rockBeginning + thisRockThickness + detectorThickness / 2);
    rockVolume->SetVisAttributes(simpleBoxVisAtt);
    detectorVolume->SetVisAttributes(simpleBoxVisAtt);

    rockBeginning += thisRockThickness + detectorThickness;
  }

  return worldPV;
}

void DetectorConstruction::ConstructSDandField() {
  for (int iRock = 0; iRock < (int)fRockThickness.size(); iRock++) {
    auto rockSD = new CalorimeterSD("RockSD_" + std::to_string(iRock), "RockHitsCollection_" + std::to_string(iRock));
    G4SDManager::GetSDMpointer()->AddNewDetector(rockSD);
    SetSensitiveDetector("Rock_" + std::to_string(iRock), rockSD);

    auto detectorSD = new CalorimeterSD("DetectorSD_" + std::to_string(iRock), "DetectorHitsCollection_" + std::to_string(iRock));
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector("Detector_" + std::to_string(iRock), detectorSD);
  }
}

G4PVPlacement* DetectorConstruction::PlaceVolume(G4LogicalVolume* volume, G4LogicalVolume* mother, G4double zShift) {
  auto position = G4ThreeVector(0, 0, zShift);
  return new G4PVPlacement(0, position, volume, volume->GetName(), mother, false, 0, fCheckOverlaps);
}

G4LogicalVolume* DetectorConstruction::GetVolume(G4String name, G4double sizeXY, G4double sizeZ, G4Material* material) {
  auto box = GetBox(name, sizeXY, sizeZ);
  return new G4LogicalVolume(box, material, name);
}

G4Box* DetectorConstruction::GetBox(G4String name, G4double sizeXY, G4double sizeZ) {
  return new G4Box(name, 0.5 * sizeXY, 0.5 * sizeXY, 0.5 * sizeZ);
}
}  // namespace B4c
