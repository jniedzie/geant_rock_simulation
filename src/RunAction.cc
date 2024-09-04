#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

namespace B4 {
RunAction::RunAction() {
  auto detector = std::make_unique<B4c::DetectorConstruction>();
  int nLayers = detector->GetNumberOfLayers();

  G4RunManager::GetRunManager()->SetPrintProgress(1);

  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  analysisManager->SetHistoDirectoryName("histograms");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  // Creating histograms
  //   for (int iRock = 0; iRock < nLayers; iRock++) {
  //     analysisManager->CreateH1("E_rock_" + to_string(iRock), "Energy deposit in rock", 100, 0., 2000 * GeV);
  //     analysisManager->CreateH1("E_det_" + to_string(iRock), "Energy deposit in detector", 100, 0., 1e-6 * eV);
  //     analysisManager->CreateH1("L_rock_" + to_string(iRock), "Track length in rock", 10000, 0., 30000 * m);
  //     analysisManager->CreateH1("L_det_" + to_string(iRock), "Track length in detector", 100, 0., 1 * mm);
  //     analysisManager->CreateH1("N_passing_" + to_string(iRock), "Number of events passing", 2, 0., 2);
  //   }

  analysisManager->CreateH1("rock_end_edges", "rock_end_edges", 10000, 0, 10000);
  analysisManager->CreateNtuple("ntuple", "ntuple");

  for (int iRock = 0; iRock < nLayers; iRock++) {
    // Creating ntuple
    analysisManager->CreateNtupleDColumn("E_rock_" + to_string(iRock));
    analysisManager->CreateNtupleDColumn("E_det_" + to_string(iRock));
    analysisManager->CreateNtupleDColumn("L_rock_" + to_string(iRock));
    analysisManager->CreateNtupleDColumn("L_det_" + to_string(iRock));
    analysisManager->CreateNtupleDColumn("N_passing_" + to_string(iRock));
    analysisManager->CreateNtupleDColumn("E_after_rock_" + to_string(iRock));
  }

  analysisManager->CreateNtupleDColumn("E_initial");

  analysisManager->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run*) {
  // inform the runManager to save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("muons_vs_rock.root");
}

void RunAction::EndOfRunAction(const G4Run*) {
  auto analysisManager = G4AnalysisManager::Instance();

  /*
  G4cout << "\n\n" << G4endl;
  for(int iRock=0; iRock<nLayers; iRock++) {

      G4double averageEnergyAbsorbed = analysisManager->GetH1(0+5*iRock)->mean() / 1000;
      G4double averageEnergyAbsorbedErr = analysisManager->GetH1(0+5*iRock)->rms() / 1000;
      G4double averageTrackLength = analysisManager->GetH1(2+5*iRock)->mean() / 1000;
      G4double averageTrackLengthErr = analysisManager->GetH1(2+5*iRock)->rms() / 1000;

      uint nPassing, nFailing;
      nPassing = analysisManager->GetH1(4+5*iRock)->bin_height(1);
      nFailing = analysisManager->GetH1(4+5*iRock)->bin_height(0);

      G4cout << "In segment " << iRock << G4endl;
      G4cout << "Average energy absorbed: " << averageEnergyAbsorbed << " +/- " << averageEnergyAbsorbedErr << " GeV" << G4endl;
      G4cout << "Average track length: " << averageTrackLength << " +/- " << averageTrackLengthErr << " m" << G4endl;
      G4cout << "Passing: " << nPassing << " failing:" << nFailing << G4endl;


  }
  G4cout << "\n\n" << G4endl;
*/

  if (IsMaster()) {
    auto detector = std::make_unique<B4c::DetectorConstruction>();
    int nLayers = detector->GetNumberOfLayers();
    auto rockEndEdges = detector->GetRockEndEdges();

    for (int i = 0; i < 10000; i++) {
      uint a_entries;
    //   double a_Sw, a_Sw2, a_Sxw, a_Sx2w;
    //   analysisManager->GetH1(0)->get_bin_content(i, a_entries, a_Sw, a_Sw2, a_Sxw, a_Sx2w);

    //   G4cout << "Bin " << i << " entries: " << a_entries << " Sw: " << a_Sw << " Sw2: " << a_Sw2 << " Sxw: " << a_Sxw << " Sx2w: " << a_Sx2w
            //  << G4endl;
      //   bool binAlreadyFilled = analysisManager->GetH1(0)->GetBinContent(i) > 0;
    //   if (a_entries != 0) continue;

      double rockEnd = (i < nLayers) ? rockEndEdges[i] * 1e-3 : -1;
      analysisManager->FillH1(0, i, rockEnd);
    }
  }

  analysisManager->Write();
  analysisManager->CloseFile();
}
}  // namespace B4
