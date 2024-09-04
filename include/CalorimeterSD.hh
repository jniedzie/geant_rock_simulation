#ifndef B4cCalorimeterSD_h
#define B4cCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "CalorHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;

namespace B4c
{
/// Calorimeter sensitive detector class
///
/// In Initialize(), it creates one hit for each calorimeter layer and one more
/// hit for accounting the total quantities in all layers.
///
/// The values are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step.

    class CalorimeterSD : public G4VSensitiveDetector
    {
    public:
        CalorimeterSD(const G4String& name, const G4String& hitsCollectionName);

        // methods from base class
        void   Initialize(G4HCofThisEvent* hitCollection) override;
        G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

    private:
        CalorHitsCollection* fHitsCollection = nullptr;
    };

}

#endif

