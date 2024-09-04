#ifndef B4cEventAction_h
#define B4cEventAction_h 1

#include "G4UserEventAction.hh"
#include "SteppingAction.hh"
#include "CalorHit.hh"
#include "globals.hh"


namespace B4c
{
    class EventAction : public G4UserEventAction
    {
    public:
        EventAction(SteppingAction *steppingAction) : fSteppingAction(steppingAction) {}

        void BeginOfEventAction(const G4Event* event) override;
        void EndOfEventAction(const G4Event* event) override;

    private:
        CalorHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
        SteppingAction *fSteppingAction = nullptr;

        std::vector<G4int> rockID;
        std::vector<G4int> detID;
        bool filledID = false;
    };

}

#endif


