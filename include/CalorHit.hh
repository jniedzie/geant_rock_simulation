#ifndef B4cCalorHit_h
#define B4cCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

namespace B4c
{
    class CalorHit : public G4VHit
    {
    public:
        CalorHit() {}
        CalorHit(const CalorHit&) = default;
        ~CalorHit() override {}

        // operators
        CalorHit& operator=(const CalorHit&) = default;
        G4bool operator==(const CalorHit& right) const { return this == &right; }

        inline void* operator new(size_t);
        inline void  operator delete(void*);

        // methods to handle data
        void Add(G4double de, G4double dl);

        // get methods
        G4double GetEdep() const { return fEdep; }
        G4double GetTrackLength() const { return fTrackLength; }
    private:
        G4double fEdep = 0.;        ///< Energy deposit in the sensitive volume
        G4double fTrackLength = 0.; ///< Track length in the  sensitive volume
    };

    using CalorHitsCollection = G4THitsCollection<CalorHit>;
    extern G4ThreadLocal G4Allocator<CalorHit>* CalorHitAllocator;

    inline void* CalorHit::operator new(size_t){
        if(!CalorHitAllocator) CalorHitAllocator = new G4Allocator<CalorHit>;
        return (void*) CalorHitAllocator->MallocSingle();
    }

    inline void CalorHit::operator delete(void *hit){
        if(!CalorHitAllocator) CalorHitAllocator = new G4Allocator<CalorHit>;
        CalorHitAllocator->FreeSingle((CalorHit*) hit);
    }

    inline void CalorHit::Add(G4double de, G4double dl) {
        fEdep += de;
        fTrackLength += dl;
    }
}

#endif
