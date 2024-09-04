#include "EventAction.hh"

#include "CalorHit.hh"
#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

namespace B4c {
CalorHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const {
  auto hitsCollection = static_cast<CalorHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
  return hitsCollection;
}

void EventAction::BeginOfEventAction(const G4Event* event) { fSteppingAction->ResetEvent(); }

void EventAction::EndOfEventAction(const G4Event* event) {
  double initialEnergy = fSteppingAction->GetInitialEnergy();
  std::vector<double> energiesAfterDetectors = fSteppingAction->GetEnergiesAfterDetectors();
  std::vector<double> energiesAfterRock;
  for (size_t i = 0; i < energiesAfterDetectors.size(); i += 2) {
    energiesAfterRock.push_back(energiesAfterDetectors[i]);
  }

  auto detector = std::make_unique<B4c::DetectorConstruction>();
  int nLayers = detector->GetNumberOfLayers();

  // Get hits collections IDs (only once)
  if (!filledID) {
    for (int iRock = 0; iRock < nLayers; iRock++) {
      rockID.push_back(G4SDManager::GetSDMpointer()->GetCollectionID("RockHitsCollection_" + std::to_string(iRock)));
    }
    filledID = true;
  }

  auto analysisManager = G4AnalysisManager::Instance();

  for (int iRock = 0; iRock < nLayers; iRock++) {
    if (rockID[iRock] < 0) continue;

    // Get hits collections
    auto rockHitsCollection = GetHitsCollection(rockID[iRock], event);
    auto detectorHistsCollection = GetHitsCollection(detID[iRock], event);

    if (!rockHitsCollection) continue;

    // Get hit with total values
    auto rockHit = (*rockHitsCollection)[rockHitsCollection->entries() - 1];
    
    analysisManager->FillNtupleDColumn(0 + 4 * iRock, rockHit->GetEdep());
    analysisManager->FillNtupleDColumn(1 + 4 * iRock, rockHit->GetTrackLength());
    analysisManager->FillNtupleDColumn(2 + 4 * iRock, rockHit->GetTrackLength() != 0);

    double energyAfter = iRock < (int)energiesAfterRock.size() ? energiesAfterRock[iRock] : 0.0;

    analysisManager->FillNtupleDColumn(3 + 4 * iRock, energyAfter);

    //    G4cout << "Energy after rock " << iRock << ": " << energiesAfterRock[iRock]/1000 << " GeV" << G4endl;
    //    G4cout << "Track length in rock " << iRock << ": " << rockHit->GetTrackLength() << " m" << G4endl;
  }

  analysisManager->FillNtupleDColumn(4 * nLayers, initialEnergy);

  analysisManager->AddNtupleRow();
}

}  // namespace B4c
