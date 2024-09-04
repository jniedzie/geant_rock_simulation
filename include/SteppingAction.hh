#ifndef B4aSteppingAction_h
#define B4aSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include <vector>

namespace B4
{
  class DetectorConstruction;
}

namespace B4c
{

class EventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in Absober and Gap layers and
/// updated in EventAction.

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction() {}

  void UserSteppingAction(const G4Step* step) override;

  void ResetEvent() {
      firstStepInEvent = true;
      initialEnergy = -1;
      energiesAfterDetectors.clear();

  }

    double GetInitialEnergy() { return  initialEnergy; }
    std::vector<double> GetEnergiesAfterDetectors() { return energiesAfterDetectors; }

private:
  bool firstStepInEvent = true;
  double initialEnergy = -1;
  std::vector<double> energiesAfterDetectors;
};

}

#endif
