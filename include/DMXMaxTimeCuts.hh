#ifndef DMXMaxTimeCuts_h
#define DMXMaxTimeCuts_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "DMXSpecialCuts.hh"


class DMXMaxTimeCuts : public DMXSpecialCuts
{
  public:     

     DMXMaxTimeCuts(const G4String& processName ="DMXMaxTimeCuts" );

     virtual ~DMXMaxTimeCuts() {}
     virtual G4double PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition);

  private:
      DMXMaxTimeCuts(DMXMaxTimeCuts&);
      DMXMaxTimeCuts& operator=(const DMXMaxTimeCuts& right);
};

#endif

