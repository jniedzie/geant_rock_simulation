#ifndef B4cActionInitialization_h
#define B4cActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

namespace B4c
{
    class ActionInitialization : public G4VUserActionInitialization
    {
    public:
        ActionInitialization() {}

        void BuildForMaster() const override;
        void Build() const override;
    };
}

#endif
