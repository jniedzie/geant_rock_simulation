#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

using namespace B4;

namespace B4c
{
    void ActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction);
    }

    void ActionInitialization::Build() const
    {
        SetUserAction(new PrimaryGeneratorAction);
        SetUserAction(new RunAction);

        auto steppingAction = new SteppingAction();

        SetUserAction(new EventAction(steppingAction));
        SetUserAction(steppingAction);
    }
}
