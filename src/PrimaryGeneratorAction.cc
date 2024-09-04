#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"

namespace B4
{

    PrimaryGeneratorAction::PrimaryGeneratorAction()
    {
//        fParticleGun = new G4ParticleGun(1);
        fParticleGun = new G4GeneralParticleSource();

        // auto particleDefinition= G4ParticleTable::GetParticleTable()->FindParticle("mu-");
        // auto particleDefinition= G4ParticleTable::GetParticleTable()->FindParticle("pi-");
        // fParticleGun->SetParticleDefinition(particleDefinition);
//        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

//        auto worldWolume = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
//        auto worldBox = dynamic_cast<G4Box*>(worldWolume->GetSolid());
//        G4double worldZHalfLength = worldBox->GetZHalfLength();
//        G4cout << "World z half length: "<<G4BestUnit(worldZHalfLength, "Length") << G4endl;
//        auto position = G4ThreeVector(0., 0., -77*m);
//        G4cout <<"Desired position: " << position << G4endl;
//        fParticleGun->SetParticlePosition(position);
//        G4cout <<"Source position: " << fParticleGun->GetParticlePosition() << G4endl;
    }

    PrimaryGeneratorAction::~PrimaryGeneratorAction()
    {
        delete fParticleGun;
    }

    void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
    {
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}
