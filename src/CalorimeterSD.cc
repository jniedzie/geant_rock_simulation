#include "CalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

namespace B4c
{
    CalorimeterSD::CalorimeterSD(const G4String& name, const G4String& hitsCollectionName) : G4VSensitiveDetector(name)
    {
        collectionName.insert(hitsCollectionName);
    }

    void CalorimeterSD::Initialize(G4HCofThisEvent* hce)
    {
        // Create hits collection
        fHitsCollection = new CalorHitsCollection(SensitiveDetectorName, collectionName[0]);

        // Add this collection in hce
        auto hcID= G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
        hce->AddHitsCollection( hcID, fHitsCollection);

        // Create hits
        fHitsCollection->insert(new CalorHit());
        fHitsCollection->insert(new CalorHit());
    }

    G4bool CalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
    {
        auto edep = step->GetTotalEnergyDeposit();

        G4double stepLength = 0.;
        if(step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) stepLength = step->GetStepLength();

        if(edep==0. && stepLength == 0.) return false;

        auto touchable = step->GetPreStepPoint()->GetTouchable();

        // Get calorimeter cell id
        auto layerNumber = touchable->GetReplicaNumber(1);

        // Get hit accounting data for this cell
        auto hit = (*fHitsCollection)[layerNumber];
        auto hitTotal= (*fHitsCollection)[fHitsCollection->entries()-1];

        if(step->GetTrack()->GetParentID() != 0) stepLength = 0;

        // Add values
        hit->Add(edep, stepLength);
        hitTotal->Add(edep, stepLength);

        return true;
    }

}
