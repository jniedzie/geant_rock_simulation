from array import array

import numpy as np
from ROOT import TH1D, TFile, TCanvas, gPad, TH2D, gStyle, TF1, TGraph
from ROOT import kBlack, kRed, kBlue, kViolet, kGreen

# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-100000events.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-10000events.root"
input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-10000events_uniformZ.root"

# particle = "mu-"
particle = "pi-"


index_to_position = {}
total_n_layers = None


def create_positions_map(input_file):
    hist = input_file.Get("histograms/rock_end_edges")

    for i in range(0, 10000):

        edge = hist.GetBinContent(i+1)

        if edge < 0:
            max_i = i
            break

        index_to_position[i] = edge

    global total_n_layers
    total_n_layers = max_i


def get_2d_hist(name=""):
    bins_x = np.logspace(-1, 5, 50)
    # bins_y = np.logspace(-2, 2, 1000)
    
    bins_y = [index_to_position[i] for i in range(total_n_layers)]
    
    hist_2d = TH2D(name, name, len(bins_x) - 1, array("f", bins_x),
                   len(bins_y) - 1, array("f", bins_y))
    hist_2d.GetXaxis().SetTitle("Initial energy (GeV)")
    hist_2d.GetYaxis().SetTitle("Distance traveled (m)")

    return hist_2d


def get_last_non_zero_energy_index(energy):
    for i, e in enumerate(reversed(energy)):
        if e > 0:
            return i

    return 0


def main():
    gStyle.SetOptStat(0)

    file = TFile.Open(input_file_path)
    tree = file.Get("ntuple")

    create_positions_map(file)

    energy = []

    for i_segment in range(total_n_layers):
        energy.append(np.full(shape=(1,), fill_value=-1, dtype="f8"))
        tree.SetBranchAddress(f"E_after_rock_{i_segment}", energy[i_segment])

    print(f"total_n_layers: {total_n_layers}")

    hists_2d = get_2d_hist(f"hist_2d")

    non_passing_count = 0

    for i_event in range(tree.GetEntries()):
    # for i_event in range(100):
        tree.GetEntry(i_event)
        
        event_energies = [e[0] for e in energy]
        initial_energy = event_energies[0]
        
        if initial_energy <= 0:
            non_passing_count += 1
            continue
        
        max_index = get_last_non_zero_energy_index(event_energies)
        
        
        # print(f"Event: {i_event}, initial energy: {initial_energy/1000}, distance: {index_to_position[max_index]:.2f}")
        
        hists_2d.Fill(initial_energy / 1000, index_to_position[max_index])

    print(f"non_passing_count: {non_passing_count}")

    canvas = TCanvas("canvas", "canvas", 100000, 5000)
    canvas.cd()
    gPad.SetLogx()
    gPad.SetLogy()
    gPad.SetLogz()
    hists_2d.Draw("colz")

    canvas.Update()
    canvas.SaveAs("plots/distance_vs_energy.pdf")


if __name__ == "__main__":
    main()
