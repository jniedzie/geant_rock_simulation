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

  // Create histograms and ntuples
  analysisManager->SetHistoDirectoryName("histograms");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  analysisManager->CreateH1("rock_end_edges", "rock_end_edges", 10000, 0, 10000);
  analysisManager->CreateNtuple("ntuple", "ntuple");

  for (int iRock = 0; iRock < nLayers; iRock++) {
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
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("../geant_output/muons_vs_rock.root");
}

void RunAction::EndOfRunAction(const G4Run*) {
  auto analysisManager = G4AnalysisManager::Instance();

  if (IsMaster()) {
    auto detector = std::make_unique<B4c::DetectorConstruction>();
    int nLayers = detector->GetNumberOfLayers();
    auto rockEndEdges = detector->GetRockEndEdges();

    for (int i = 0; i < 10000; i++) {
      double rockEnd = (i < nLayers) ? rockEndEdges[i] * 1e-3 : -1;
      analysisManager->FillH1(0, i, rockEnd);
    }
  }

  analysisManager->Write();
  analysisManager->CloseFile();
}
}  // namespace B4
