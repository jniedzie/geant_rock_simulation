from array import array

import numpy as np
from ROOT import TH1D, TFile, TCanvas, gPad, TH2D, gStyle, TF1, TGraph
from ROOT import kBlack, kRed, kBlue, kViolet, kGreen, gROOT

# input_file_path = "/Users/Jeremi/Documents/Physics/superLLP/data/geant_output/muonVsRock_energy-userUniform_nEvents-10000.root"
# input_file_path = "/Users/Jeremi/Documents/Physics/superLLP/data/geant_output/muonVsRock_energy-userUniform_nEvents-100k_rock-10km_part-1.root"

# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/muMinus_vs_rock-10000events.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/muMinus_vs_rock-1000events.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/muMinus_vs_rock-1000events_moreGranular.root"

# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-10000events.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-10000events_moreGranular.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-100000events_moreGranular.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-10000events_moreGranular.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_test.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-10000events.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-100000events.root"
# input_file_path = "/Users/jeremi/Documents/Physics/superLLP/event_generation/geant4/geant4-v11.0.3/my_programs/muons_vs_rock_from_B4c/build/piMinus_vs_rock-10000events_ge1TeV.root"
# input_file_path = "../geant_output/piMinus_vs_rock-10000events.root"
input_file_path = "../geant_output/muons_vs_rock.root"

# particle = "mu-"
particle = "pi-"

save_all_plots = False

max_distance = 10  # m
max_index = None

n_fits = 50
log_scale_min = -3
log_scale_max = 5
rebin = 1

hist_to_show = (0, 1, 2, 10, 100, -1)

points_per_decade = [10**(x/10) for x in np.linspace(0, 10, 100)]
energy = [np.full(shape=(1,), fill_value=-1, dtype="f8")]

if particle == "mu-":
    starting_value = 1
if particle == "pi-":
    starting_value = 0.01

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

    global max_index

    for i in range(0, max_i):
        if index_to_position[i] > max_distance:
            max_index = i-1
            break

    if max_index is None:
        max_index = max_i
        
    print(f"max_index: {max_index}")


def get_log_bins(min_exp, max_exp, points_per_decade=(1,)):
    bins = []

    for exponent in range(min_exp, max_exp):
        for point in points_per_decade:
            bins.append(point * 10 ** exponent)

    return bins


bins_more = get_log_bins(log_scale_min, log_scale_max,
                         points_per_decade=points_per_decade)


def scale_by_bin_width(hist):
    if hist.GetEntries() == 0:
        return hist

    for i_bin in range(hist.GetNbinsX()):
        n_entries = hist.GetBinContent(i_bin + 1)
        bin_width = hist.GetXaxis().GetBinWidth(i_bin + 1)
        hist.SetBinContent(i_bin + 1, n_entries / bin_width)

    return hist


def get_bins(min, max, n_bins):
    bins = []

    for bin in np.linspace(min, max, n_bins):
        bins.append(bin)

    return bins


def get_average_along_y(input_hist):

    if input_hist is None:
        print("ERROR -- input hist does not exist")
        return None

    output_hist = TH1D("averages", "averages", len(
        bins_more) - 1, array("f", bins_more))

    previous_mean = -1

    for i_x in range(input_hist.GetNbinsX()):
        projection = input_hist.ProjectionY("", i_x+1, i_x+1)
        mean = projection.GetMean()

        if mean == 0:
            mean = previous_mean

        output_hist.SetBinContent(i_x+1, mean)

        previous_mean = mean

    return output_hist


def get_max_along_y(input_hist):
    output_hist = TH1D("max", "max", len(bins_more) - 1, array("f", bins_more))
    output_graph = TGraph()

    previous_max = -1

    i_point = 0

    for i_x in range(input_hist.GetNbinsX()):
        projection = input_hist.ProjectionY("", i_x + 1, i_x + 1)
        max = projection.GetXaxis().GetBinCenter(projection.GetMaximumBin())
        # max = projection.GetMean()

        if max > (10**log_scale_min * 1.2):
            # print(f"Setting point {i_point} to {input_hist.GetXaxis().GetBinCenter(i_x + 1)} and {max}")
            output_graph.SetPoint(
                i_point, input_hist.GetXaxis().GetBinCenter(i_x + 1), max)
            i_point += 1

        if max == 0:
            max = previous_max

        output_hist.SetBinContent(i_x + 1, max)
        output_hist.SetBinError(i_x + 1, 0)
        # output_hist.SetBinContent(log(i_x + 1), log(max))

        previous_max = max

    output_hist.Rebin(rebin)
    output_hist.Scale(1/rebin)

    # return output_hist
    return output_graph


def find_slope_beginning(input_hist):

    first_nonempty_bin = -1

    # for i_x in range(input_hist.GetNbinsX()):
    for i_x in range(input_hist.GetN()):
        # bin_content = input_hist.GetBinContent(i_x+1)
        bin_content = input_hist.GetY()[i_x]

        if bin_content > 10**log_scale_min * 1.2:
            first_nonempty_bin = i_x
            break

    # return input_hist.GetBinCenter(first_nonempty_bin)
    return input_hist.GetX()[first_nonempty_bin]


def setup_branches(tree):
    tree.SetBranchAddress(f"E_initial", energy[0])

    for i_segment in range(max_index+1):
        energy.append(np.full(shape=(1,), fill_value=-1, dtype="f8"))
        tree.SetBranchAddress(
            f"E_after_rock_{i_segment}", energy[i_segment + 1])


def get_2d_hist(name=""):
    hist_2d = TH2D(name, name,
                   len(bins_more) - 1, array("f", bins_more),
                   len(bins_more) - 1, array("f", bins_more))
    hist_2d.GetXaxis().SetTitle("Initial energy (GeV)")
    hist_2d.GetYaxis().SetTitle("Final energy (GeV)")

    return hist_2d


def fit_hist(hist, name="", starting_value=starting_value):
    # fit_function = TF1(name, "pow(10, 10+[0]*log((x-[1])/(1e10-[1])))", 0, 1e4)
    # fit_function.FixParameter(0, 0.435)
    # fit_function.SetParameter(1, 0)

    fit_function = TF1(
        name, "pow(10, 10 * (1 + (log(x-[0])-log(1e10-[0])) / (log(1e20-[0])-log(1e10-[0]))) + [1]*log(x)+[2])", 0, 1e4)

    fit_function.SetParameter(0, starting_value)
    fit_function.SetParameter(1, 1e-2)
    fit_function.FixParameter(2, 0)

    # fit_result = hist.Fit(fit_function, "", "", find_slope_beginning(hist), 1e4)

    if len(hist.GetX()) == 0:
        return None

    fit_result = hist.Fit(fit_function, "", "", hist.GetX()[0], 1e4)
    print(f"chi2/nDoF: {fit_function.GetChisquare()}")

    return fit_function


def main():
    gStyle.SetOptStat(0)
    gROOT.SetBatch(True)

    file = TFile.Open(input_file_path)
    tree = file.Get("ntuple")

    create_positions_map(file)
    setup_branches(tree)

    hists_2d = []

    print(f"total_n_layers: {total_n_layers}")

    for i in range(max_index+1):
        hists_2d.append(get_2d_hist(f"hist_2d_{i}"))

    for i_event in range(tree.GetEntries()):
        tree.GetEntry(i_event)

        for i in range(max_index+1):
            hists_2d[i].Fill(energy[0] / 1000, energy[i+1][0] / 1000)

    hists_average = {}
    functions = {}

    fitted_hists_indices = []

    previous_fun_param = starting_value

    fit_bins = range(0, max_index+1) if n_fits < 0 else np.linspace(0, max_index - 1, n_fits)

    for i in fit_bins:
        index = int(i)

        # hists_average[index] = get_average_along_y(hists_2d[index])

        print(f"index: {index}")

        hists_average[index] = get_max_along_y(hists_2d[index])
        # functions[index] = fit_hist(hists_average[index], name=f"function_{index}", starting_value=previous_fun_param)
        functions[index] = fit_hist(hists_average[index], name=f"function_{index}")

        if functions[index] is None:
            continue
        functions[index].SetLineStyle(2)
        functions[index].SetLineWidth(1)

        previous_fun_param = functions[index].GetParameter(0)

        fitted_hists_indices.append(index)

    canvas = TCanvas("canvas", "canvas", 100000, 5000)
    canvas.Divide(n_fits+1, 2)

    for i_pad, i_hist in enumerate(fitted_hists_indices):

        canvas.cd(i_pad+1)
        gPad.SetLogx()
        gPad.SetLogy()
        hists_2d[i_hist].Draw("colz")

        canvas.cd(i_pad+1 + n_fits + 1)
        gPad.SetLogx()
        gPad.SetLogy()
        hists_average[i_hist].Draw("")

        if save_all_plots:
            tmp_canvas = TCanvas("tmp_canvas", "tmp_canvas", 1000, 1000)
            tmp_canvas.cd()
            gPad.SetLogx()
            gPad.SetLogy()

            hists_average[i_hist].SetMarkerStyle(20)
            hists_average[i_hist].SetMarkerColor(kBlack)
            hists_average[i_hist].SetMarkerSize(0.5)

            hists_average[i_hist].Draw("AP")

            hists_average[i_hist].SetMinimum(1e-4)
            hists_average[i_hist].SetMaximum(1e4)
            hists_average[i_hist].GetXaxis().SetLimits(1e-4, 1e4)

            functions[i_hist].Draw("same")
            tmp_canvas.Update()
            distance = f"{index_to_position[i_hist]:.3f}".replace(".", "p")

            tmp_canvas.SaveAs(f"plots/fit_energy_loss_{distance}.pdf")

    canvas.cd(n_fits+1)
    offset_graph = TGraph()

    for i_point, i in enumerate(fit_bins):
        index = int(i)

        if functions[index] is None:
            continue

        offset_graph.SetPoint(
            i_point, index_to_position[index], functions[index].GetParameter(0))

    offset_graph.SetMarkerStyle(20)
    offset_graph.SetMarkerColor(kViolet)

    offset_graph.Draw("AP")

    offset_graph.GetXaxis().SetTitle("Rock thickness (m)")
    offset_graph.GetYaxis().SetTitle("E_{crit} (GeV)")

    lin_function = TF1("lin_function", "[0]*x", 0, 100000)
    offset_graph.Fit(lin_function, "", "", 0, 1000)

    canvas.Update()
    canvas.SaveAs("plots/fit_energy_loss.pdf")

    canvas_offset = TCanvas("canvas_offset", "canvas_offset", 1000, 1000)
    canvas_offset.cd()
    gPad.SetLogx()
    gPad.SetLogy()
    offset_graph.Draw("AP")
    lin_function.Draw("same")
    canvas_offset.Update()
    canvas_offset.SaveAs("plots/fit_energy_loss_offset.pdf")


if __name__ == "__main__":
    main()
