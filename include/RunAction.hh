#ifndef B4RunAction_h
#define B4RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

namespace B4
{
    class RunAction : public G4UserRunAction
    {
    public:
        RunAction();

        void BeginOfRunAction(const G4Run*) override;
        void   EndOfRunAction(const G4Run*) override;
    private:
        bool rockEdgesFilled = false;
    };

}

#endif

