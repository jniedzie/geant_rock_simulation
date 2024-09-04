#ifndef DMXMinEkineCuts_h
#define DMXMinEkineCuts_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "DMXSpecialCuts.hh"


class DMXMinEkineCuts : public DMXSpecialCuts
{
public:
    DMXMinEkineCuts(const G4String& processName ="DMXMinEkineCuts");
    virtual ~DMXMinEkineCuts() {}

    virtual G4double PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition);
private:
    DMXMinEkineCuts(DMXMinEkineCuts&);
    DMXMinEkineCuts& operator=(const DMXMinEkineCuts& right);
};

#endif

