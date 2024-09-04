#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4UnitsTable.hh"

using namespace B4;

namespace B4c {

void SteppingAction::UserSteppingAction(const G4Step *step) {
  if (!step) return;
  if (!step->GetTrack()) return;

  if (step->GetTrack()->GetParentID() == 0) {
    if (step->IsFirstStepInVolume() && firstStepInEvent) {
      G4StepPoint *prePoint = step->GetPreStepPoint();
      if (!prePoint) return;
      initialEnergy = prePoint->GetTotalEnergy();
      firstStepInEvent = false;
    }

    if (step->IsLastStepInVolume()) {
      G4StepPoint *postPoint = step->GetPostStepPoint();
      if (!postPoint) return;
      energiesAfterDetectors.push_back(postPoint->GetTotalEnergy());
    }
  } else {
    step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

}  // namespace B4c
