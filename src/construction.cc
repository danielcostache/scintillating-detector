#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() {

}

MyDetectorConstruction::~MyDetectorConstruction() {

}

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
    G4NistManager *nist = G4NistManager::Instance();
    std::vector<G4double> energy = {1.239841939*eV/0.9, 1.239841939*eV/0.2};

    // World material
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    std::vector<G4double> rindexWorld = {1.0, 1.0};

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

    worldMat->SetMaterialPropertiesTable(mptWorld);

    auto                *solidWorld = new G4Box("solidWorld", 0.25*m, 0.25*m, 0.25*m);
    G4LogicalVolume     *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume   *physWorld  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);   

    // EJ-228 / Pilot U / BC-428 Plastic scintillator
    G4Material *pilotU = new G4Material("PilotU", 1.023*g/cm3, 2);

    pilotU->AddElement(nist->FindOrBuildElement("C"), 47.46*perCent);
    pilotU->AddElement(nist->FindOrBuildElement("H"), 52.54*perCent);

    std::vector<G4double> scintEnergyPilotU = {1.239841939*eV/0.396, 1.239841939*eV/0.391, 1.239841939*eV/0.386};
    std::vector<G4double> rindexpilotU = {1.60, 1.58, 1.56};
    std::vector<G4double> scintFracPilotU = {0.1, 1.0, 0.1};

    G4MaterialPropertiesTable *mptpilotU = new G4MaterialPropertiesTable();
    mptpilotU->AddProperty("RINDEX", scintEnergyPilotU, rindexpilotU);
    mptpilotU->AddProperty("SCINTILLATIONCOMPONENT1", scintEnergyPilotU, scintFracPilotU);
    mptpilotU->AddConstProperty("SCINTILLATIONYIELD", 10.2/keV);
    mptpilotU->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptpilotU->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1.4*ns);
    mptpilotU->AddConstProperty("SCINTILLATIONYIELD1", 1.);

    pilotU->SetMaterialPropertiesTable(mptpilotU);

    // NaI(Tl) scintillator
    // G4Material *NaI = new G4Material("NaI", 3.67*g/cm3,2);
    // NaI->AddElement(nist->FindOrBuildElement("Na"), 1);
    // NaI->AddElement(nist->FindOrBuildElement("I"), 1);

    // G4Element *Tl = nist->FindOrBuildElement("Tl");

    // G4Material *NaITl = new G4Material("NaITl", 3.67*g/cm3, 2);
    // NaITl->AddMaterial(NaI, 99.99*perCent);
    // NaITl->AddElement(Tl, 0.01*perCent);

    // std::vector<G4double> scintEnergy = {2.89*eV, 2.99*eV, 3.09*eV};
    // std::vector<G4double> rindexNaITl = {1.87, 1.85, 1.83};
    // std::vector<G4double> scintFrac = {0.1, 1.0, 0.1};
    // std::vector<G4double> absLength = {17.*cm, 17.*cm, 17.*cm};

    // G4MaterialPropertiesTable *mptNaITl = new G4MaterialPropertiesTable();
    // mptNaITl->AddProperty("RINDEX", scintEnergy, rindexNaITl);
    // mptNaITl->AddProperty("SCINTILLATIONCOMPONENT1", scintEnergy, scintFrac);
    // mptNaITl->AddProperty("ABSLENGTH", scintEnergy, absLength);
    // mptNaITl->AddConstProperty("SCINTILLATIONYIELD", 38.0/keV);
    // mptNaITl->AddConstProperty("RESOLUTIONSCALE", 1.0);
    // mptNaITl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250*ns);
    // mptNaITl->AddConstProperty("SCINTILLATIONYIELD1", 1.);

    // NaITl->SetMaterialPropertiesTable(mptNaITl);

    auto                *solidScintillator = new G4Box("solidScintillator", 3.*cm, 3.*cm, 1.5*cm);
    G4LogicalVolume     *logicScintillator = new G4LogicalVolume(solidScintillator, pilotU, "logicScintillator");
    G4VPhysicalVolume   *physScintillator  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.1*m), logicScintillator, "physScintillator", logicWorld, false, 0, true);


    // Casing
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");

    auto                *solidCase1 = new G4Box("solidCase1", 0.1*cm, 3.2*cm, 1.5*cm);
    G4LogicalVolume     *logicCase1 = new G4LogicalVolume(solidCase1, Al, "logicCase1");
    G4VPhysicalVolume   *physCase1  = new G4PVPlacement(0, G4ThreeVector(3.1*cm, 0.*cm, 10.*cm), logicCase1, "physCase1", logicWorld, false, 0, true);
    auto                *solidCase2 = new G4Box("solidCase2", 0.1*cm, 3.2*cm, 1.5*cm);
    G4LogicalVolume     *logicCase2 = new G4LogicalVolume(solidCase2, Al, "logicCase2");
    G4VPhysicalVolume   *physCase2  = new G4PVPlacement(0, G4ThreeVector(-3.1*cm, 0.*cm, 10.*cm), logicCase2, "physCase2", logicWorld, false, 0, true);
    auto                *solidCase3 = new G4Box("solidCase3", 3.*cm, 0.1*cm, 1.5*cm);
    G4LogicalVolume     *logicCase3 = new G4LogicalVolume(solidCase3, Al, "logicCase3");
    G4VPhysicalVolume   *physCase3  = new G4PVPlacement(0, G4ThreeVector(0.*cm, 3.1*cm, 10.*cm), logicCase3, "physCase3", logicWorld, false, 0, true);
    auto                *solidCase4 = new G4Box("solidCase4", 3.*cm, 0.1*cm, 1.5*cm);
    G4LogicalVolume     *logicCase4 = new G4LogicalVolume(solidCase4, Al, "logicCase4");
    G4VPhysicalVolume   *physCase4  = new G4PVPlacement(0, G4ThreeVector(0.*cm, -3.1*cm, 10.*cm), logicCase4, "physCase4", logicWorld, false, 0, true);
    auto                *solidCase5 = new G4Box("solidCase5", 3.2*cm, 3.2*cm, 0.01*cm);
    G4LogicalVolume     *logicCase5 = new G4LogicalVolume(solidCase5, Al, "logicCase5");
    G4VPhysicalVolume   *physCase5  = new G4PVPlacement(0, G4ThreeVector(0.*cm, 0.*cm, 8.49*cm), logicCase5, "physCase5", logicWorld, false, 0, true);

    
    // Sensitive detectors
//     auto *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.01*m);
//     logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
//     for(G4int i = 0; i < 100; i++) {
//         for(G4int j = 0; j < 100; j++) {
//             G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/100, -0.5*m+(j+0.5)*m/100, 0.49*m),
//                                                                 logicDetector, "physDetector", logicWorld, false, j+i*100, true);
//         }
//     }

    return physWorld;
}

// void MyDetectorConstruction::ConstructSDandField() {
//     MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    
//     logicDetector->SetSensitiveDetector(sensDet);
// }