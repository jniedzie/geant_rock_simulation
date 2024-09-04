#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4PhysListFactory.hh"
#include "DMXPhysicsList.hh"
#include "PhysicsList.hh"

namespace {
    void PrintUsage() {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " exampleB4c [-m macro ] [-u UIsession] [-t nThreads] [-vDefault]" << G4endl;
        G4cerr << "   note: -t option is available only for multi-threaded mode." << G4endl;
    }
}

int main(int argc,char** argv)
{
    // Evaluate arguments
    //
    if ( argc > 5 ) {
        PrintUsage();
        return 1;
    }

    G4String macro, session;
    G4int nThreads = 0;

    for(G4int i=1; i<argc; i=i+2) {
        if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
        else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
        else if ( G4String(argv[i]) == "-t" ) nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
        else {
            PrintUsage();
            return 1;
        }
    }

    // Detect interactive mode (if no macro provided) and define UI session
    //
    G4UIExecutive* ui = nullptr;
    if(!macro.size()) ui = new G4UIExecutive(argc, argv, session);


    // Use G4SteppingVerboseWithUnits
    G4SteppingVerbose::UseBestUnit(4);

    auto *runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

#ifdef G4MULTITHREADED
    if(nThreads > 0) runManager->SetNumberOfThreads(nThreads);
#endif

    G4String physicsListName = "FTFP_BERT_EMZ";

//    G4String physicsListName = "QGSC";
//    G4String physicsListName = "QGSC_EMV";

//    G4String physicsListName = "QGSP";
//    G4String physicsListName = "QGSP_EMV";
//    G4String physicsListName = "QGSP_EFLOW";

//    G4String physicsListName = "QGSP_BERT";
//    G4String physicsListName = "QGSP_BERT_EMV";
//    G4String physicsListName = "QGSP_BERT_HP";
//    G4String physicsListName = "QGSP_BERT_TRV";
//
//    G4String physicsListName = "QGSP_BIC";
//    G4String physicsListName = "QGSP_BIC_HP";
//
//    G4String physicsListName = "QGSP_NEQ";
//    G4String physicsListName = "QGSP_EMV_NQE";
//    G4String physicsListName = "QGSP_BERT_NQE";
//
//    G4String physicsListName = "QGSP_INCLXX";
//
//    G4String physicsListName = "FTFP_BERT";
//    G4String physicsListName = "FTFP";
//    G4String physicsListName = "FTFP_EMV";


    G4PhysListFactory plFactory;
    auto physicsList = plFactory.GetReferencePhysList(physicsListName);

    runManager->SetUserInitialization(physicsList);
//    runManager->SetUserInitialization(new PhysicsList);

//    runManager->SetUserInitialization(new DMXPhysicsList);

    runManager->SetUserInitialization(new B4c::DetectorConstruction());
    runManager->SetUserInitialization(new B4c::ActionInitialization());

    // Initialize visualization
    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if ( macro.size() ) {
        // batch mode
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);
    }
    else  {
        // interactive mode : define UI session
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}
