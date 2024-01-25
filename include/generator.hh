#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include "radsource.h"
#include "cpp_api.h"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    MyPrimaryGenerator(G4double radius, G4String fileName);
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);
    static double localran();
    void UpdateRadsrc();
    void InputRadsrc(G4String inputLine);
    void RadsrcFromFile(G4String inputLine);

private:
    G4ParticleGun *fParticleGun;
    radsrc::CRadSource *pRadSource;
    radsrc::CRadSource *t1RadSource;
    radsrc::CRadSource *t2RadSource;
    G4double outerRadius;
    G4bool t1Good, t2Good, newInput;
};

#endif 