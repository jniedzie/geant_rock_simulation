#ifndef B3PhysicsList_h
#define B3PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4HadronInelasticQBBC.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"

namespace B3
{

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList() {
      SetVerboseLevel(1);
      RegisterPhysics( new G4EmStandardPhysics()); // EM Physics
      RegisterPhysics( new G4EmExtraPhysics()); // Synchroton Radiation & GN Physics
      RegisterPhysics( new G4DecayPhysics()); // Decays
      RegisterPhysics( new G4NeutronTrackingCut()); // Neutron tracking cut
      RegisterPhysics(new G4RadioactiveDecayPhysics()); // Radioactive decay

      RegisterPhysics( new G4HadronElasticPhysicsXS()); // Hadron Physics

      RegisterPhysics( new G4StoppingPhysics());
      RegisterPhysics( new G4IonPhysicsXS());
      RegisterPhysics( new G4IonElasticPhysics());
      RegisterPhysics( new G4HadronInelasticQBBC());
  }

  void SetCuts() override {G4VUserPhysicsList::SetCuts();}
};

}

#endif

