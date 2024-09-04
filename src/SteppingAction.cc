#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

using namespace B4;

namespace B4c
{

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    if(!step) return;
    if(!step->GetTrack()) return;

    if(step->GetTrack()->GetParentID() == 0){

        if(step->IsFirstStepInVolume() && firstStepInEvent){
            G4StepPoint *prePoint = step->GetPreStepPoint();
            if(!prePoint) return;
            initialEnergy = prePoint->GetTotalEnergy();
//            G4cout << "Initial energy: " << G4BestUnit( initialEnergy, "Energy") << G4endl;
            firstStepInEvent = false;
        }

        if(step->IsLastStepInVolume()) {
            G4StepPoint *postPoint = step->GetPostStepPoint();
            if(!postPoint) return;
            energiesAfterDetectors.push_back(postPoint->GetTotalEnergy());
//            G4cout << "energy: " << G4BestUnit( postPoint->GetTotalEnergy(), "Energy") << G4endl;
        }
    }
    else{
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}

}
