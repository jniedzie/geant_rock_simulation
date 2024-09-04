#ifndef B4PrimaryGeneratorAction_h
#define B4PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

class G4ParticleGun;
class G4Event;

namespace B4
{
    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction() override;

        void GeneratePrimaries(G4Event* event) override;

        void SetRandomFlag(G4bool value);

    private:
//        G4ParticleGun* fParticleGun = nullptr;
        G4GeneralParticleSource* fParticleGun = nullptr;
    };
}

#endif
