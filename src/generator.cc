#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(G4double radius, G4String fileName) {
    outerRadius = radius;

    G4bool newInput = true;

    t1RadSource = radsrc::CApi::newSource();
    t1Good = radsrc::CApi::loadConfig(t1RadSource, (const std::string) fileName);
    if(t1Good)
        pRadSource = t1RadSource;

    G4int n_particle = 1;
    G4ParticleGun *fParticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="gamma"));
}

MyPrimaryGenerator::~MyPrimaryGenerator() {
    delete fParticleGun;
}

double MyPrimaryGenerator::localran() {
    return G4UniformRand();
}

void MyPrimaryGenerator::RadsrcFromFile(G4String inputLine) {
    t2RadSource = radsrc::CApi::newSource();
    t2Good = radsrc::CApi::loadConfig(t2RadSource, inputLine);
    if(t2Good)
        pRadSource = t2RadSource;
}

void MyPrimaryGenerator::InputRadsrc(G4String inputLine) {
    if(newInput) {
        t2RadSource = radsrc::CApi::newSource();
        newInput = false;
    }
    radsrc::CApi::addConfig(t2RadSource, inputLine);
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    if(pRadSource == NULL)
        std::cout << "ExN01PrimaryGeneratorAction::GeneratePrimaries, RADSRC has not been defined";
    
    G4double energy = radsrc::CApi::getPhoton(pRadSource, localran) * CLHEP::keV;
    fParticleGun->SetParticleEnergy(energy);

    G4double phi      = 2.0*CLHEP::pi*G4UniformRand();
    G4double costheta = 2.0*G4UniformRand(); 
    G4double sintheta = pow(1.0-pow(costheta, 2.0), 0.5);
    G4double r        = outerRadius*pow(G4UniformRand(), 1.0/3.0);
    fParticleGun->SetParticlePosition(G4ThreeVector(r*costheta,r*sintheta*cos(phi),r*sintheta*sin(phi)));

    phi = 2.0*CLHEP::pi*G4UniformRand();
    costheta = 2.0*G4UniformRand()-1.0;
    sintheta = pow(1.0-pow(costheta,2.0),0.5);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(costheta,sintheta*cos(phi),sintheta*sin(phi)));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}