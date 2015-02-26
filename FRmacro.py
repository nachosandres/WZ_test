import ROOT, copy, os, subprocess

ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPaintTextFormat("4.3f")
ROOT.TGaxis.SetMaxDigits(3)

lumi = 10000.


def drawFakeRatio(c, hist, name):

	hist.SetMarkerColor(ROOT.kBlack)
	hist.SetMarkerSize(1.6)
	hist.GetXaxis().SetTitle("P_T(lep)")
	hist.GetYaxis().SetTitle("|#eta|(lep)")
	hist.GetXaxis().SetTitleSize(0.045)
	hist.GetXaxis().SetLabelSize(0.045)
	hist.GetYaxis().SetTitleSize(0.045)
	hist.GetYaxis().SetLabelSize(0.045)
	hist.SetMinimum(0.0)
	hist.SetMaximum(0.5)
	name = hist.GetTitle() + "_" + name
	hist.SetTitle("")
	hist.Draw("text colz e")

	c.SaveAs("/shome/cheidegg/d/MPAF/workdir/plots/FakeRatio/png/" + name + ".png")




def makeFRplot(c, h, d, t, l):
	h.Divide(d)
	h.SetTitle(t)
	drawFakeRatio(c, h, l)


_c = ROOT.TCanvas("c", "c", 975, 700)
_c.SetBottomMargin(0.12)
_c.SetLeftMargin(0.12)
_c.SetRightMargin(0.12)
_c.SetTopMargin(0.09)

vars    =  ["MR_LLepMap"  , \
            "MR_TLepMap"  , \
            "MR_LNTLepMap", \
            "MR_LElMap"   , \
            "MR_TElMap"   , \
            "MR_LMuMap"   , \
            "MR_TMuMap"   ]
wjets   = [["WJetsToLNu_HT100to200_skimfake"    , 5262265.  / (1817.0   * 1.23)], \
           ["WJetsToLNu_HT200to400_skimfake"    , 4936077.  / ( 471.6   * 1.23)], \
           ["WJetsToLNu_HT400to600_skimfake"    , 4640594.  / (  55.61  * 1.23)], \
           ["WJetsToLNu_HT600toInf_skimfake"    , 237484.   / (  18.81  * 1.23)]]
dyjets  = [["DYJetsToLL_M50_HT100to200_skimfake", 4054159.  / ( 194.3   * 1.27)], \
           ["DYJetsToLL_M50_HT200to400_skimfake", 4666496.  / (  52.24  * 1.27)], \
           ["DYJetsToLL_M50_HT400to600_skimfake", 4931372.  / (   6.546 * 1.27)], \
           ["DYJetsToLL_M50_HT600toInf_skimfake", 4493574.  / (   2.179 * 1.27)]]
ttbar   = [["TTJets_skimfake"                   , 25446993. / 809.1            ]]
rares   = [["TTWJets_skimfake"                  , 246521.   / 0.6647           ], \
           ["TTZJets_skimfake"                  , 249275.   / 0.8565           ], \
           ["WZJetsTo3LNu_skimfake"             , 237484.   / 2.29             ]]
singlet = [["TBarToLeptons_sch_skimfake"        , 250000.   / 0.44928          ], \
           ["TBarToLeptons_tch_skimfake"        , 1999800.  / 8.74476          ], \
           ["TToLeptons_sch_skimfake"           , 500000.   / 0.7776           ], \
           ["TToLeptons_tch_skimfake"           , 3991000.  / 14.6934          ], \
           ["TBar_tWch_skimfake"                , 971800.   / 35.6             ], \
           ["T_tWch_skimfake"                   , 986100.   / 35.6             ]]

denom_wjets   = [ROOT.TH2F(), ROOT.TH2F(), ROOT.TH2F()]
denom_dyjets  = [ROOT.TH2F(), ROOT.TH2F(), ROOT.TH2F()]
denom_ttbar   = [ROOT.TH2F(), ROOT.TH2F(), ROOT.TH2F()]
denom_rares   = [ROOT.TH2F(), ROOT.TH2F(), ROOT.TH2F()]
denom_singlet = [ROOT.TH2F(), ROOT.TH2F(), ROOT.TH2F()]

pipe = subprocess.Popen("ls /shome/cheidegg/d/MPAF/workdir/root/FakeRatio/newmatch/FakeRatio_MR00_cut.root", shell = True, stdout = subprocess.PIPE, stderr = subprocess.PIPE)

while True:
	line = pipe.stdout.readline()
	if line == "": break
	line = line.strip("\n")

	f = ROOT.TFile(line)
	print "processing file " + line

	add = line[line.rfind("FakeRatio_")+10:-5]

	for var in vars:

		# w
		h = ROOT.TH2F(f.Get(var + "/" + wjets[0][0]))
		h.Scale(lumi/wjets[0][1])
		for j in range(1,len(wjets)):
			h1 = ROOT.TH2F(f.Get(var + "/" + wjets[j][0]))
			h1.Scale(lumi/wjets[j][1])
			h.Add(h1)
		drawFakeRatio(_c, h, "wjets_" + add)

		if var == "MR_LElMap" : denom_wjets[0] = copy.deepcopy(h)
		if var == "MR_LLepMap": denom_wjets[1] = copy.deepcopy(h)
		if var == "MR_LMuMap" : denom_wjets[2] = copy.deepcopy(h)

		if var == "MR_TElMap" : makeFRplot(_c, h, denom_wjets[0], "MR_RElMap" , "wjets_" + add) 
		if var == "MR_TLepMap": makeFRplot(_c, h, denom_wjets[1], "MR_RLepMap", "wjets_" + add) 
		if var == "MR_TMuMap" : makeFRplot(_c, h, denom_wjets[2], "MR_RMuMap" , "wjets_" + add) 


		# dy
		#for j in range(len(dyjets)):
		#	h_pdg = ROOT.TH1F(f.Get(var.rstrip("Map") + "MPdg/" + dyjets[j][0]))
		#	h_pdg.Scale(lumi/dyjets[j][1])
		#	h_pdg.Draw("hist")
		#	_c.SaveAs("/shome/cheidegg/d/MPAF/workdir/plots/FakeRatio/png/" + var.rstrip("Map") + "_" + dyjets[j][0] + ".png")

		h = ROOT.TH2F(f.Get(var + "/" + dyjets[0][0]))
		print "retrieving histogram " + var + "/" + dyjets[0][0] + " (" + str(h.GetEntries()) + ")"
		h.Scale(lumi/dyjets[0][1])
		for j in range(1,len(dyjets)):
			h1 = ROOT.TH2F(f.Get(var + "/" + dyjets[j][0]))
			h1.Scale(lumi/dyjets[j][1])
			h.Add(h1)
		drawFakeRatio(_c, h, "dyjets_" + add)

		if var == "MR_LElMap" : denom_dyjets[0] = copy.deepcopy(h)
		if var == "MR_LLepMap": denom_dyjets[1] = copy.deepcopy(h)
		if var == "MR_LMuMap" : denom_dyjets[2] = copy.deepcopy(h)

		if var == "MR_TElMap" : makeFRplot(_c, h, denom_dyjets[0], "MR_RElMap" , "dyjets_" + add) 
		if var == "MR_TLepMap": makeFRplot(_c, h, denom_dyjets[1], "MR_RLepMap", "dyjets_" + add) 
		if var == "MR_TMuMap" : makeFRplot(_c, h, denom_dyjets[2], "MR_RMuMap" , "dyjets_" + add) 


		# ttbar
		h = ROOT.TH2F(f.Get(var + "/" + ttbar[0][0]))
		h.Scale(lumi/ttbar[0][1])
		drawFakeRatio(_c, h, "ttbar_" + add)

		if var == "MR_LElMap" : denom_ttbar[0] = copy.deepcopy(h)
		if var == "MR_LLepMap": denom_ttbar[1] = copy.deepcopy(h)
		if var == "MR_LMuMap" : denom_ttbar[2] = copy.deepcopy(h)

		if var == "MR_TElMap" : makeFRplot(_c, h, denom_ttbar[0], "MR_RElMap" , "ttbar_" + add) 
		if var == "MR_TLepMap": makeFRplot(_c, h, denom_ttbar[1], "MR_RLepMap", "ttbar_" + add) 
		if var == "MR_TMuMap" : makeFRplot(_c, h, denom_ttbar[2], "MR_RMuMap" , "ttbar_" + add) 


		# rares
		h = ROOT.TH2F(f.Get(var + "/" + rares[0][0]))
		h.Scale(lumi/rares[0][1])
		for j in range(1,len(rares)):
			h1 = ROOT.TH2F(f.Get(var + "/" + rares[j][0]))
			h1.Scale(lumi/rares[j][1])
			h.Add(h1)
		drawFakeRatio(_c, h, "rares_" + add)

		if var == "MR_LElMap" : denom_rares[0] = copy.deepcopy(h)
		if var == "MR_LLepMap": denom_rares[1] = copy.deepcopy(h)
		if var == "MR_LMuMap" : denom_rares[2] = copy.deepcopy(h)

		if var == "MR_TElMap" : makeFRplot(_c, h, denom_rares[0], "MR_RElMap" , "rares_" + add) 
		if var == "MR_TLepMap": makeFRplot(_c, h, denom_rares[1], "MR_RLepMap", "rares_" + add) 
		if var == "MR_TMuMap" : makeFRplot(_c, h, denom_rares[2], "MR_RMuMap" , "rares_" + add) 


		# t
		h = ROOT.TH2F(f.Get(var + "/" + singlet[0][0]))
		h.Scale(lumi/singlet[0][1])
		for j in range(1,len(singlet)):
			h1 = ROOT.TH2F(f.Get(var + "/" + singlet[j][0]))
			h1.Scale(lumi/singlet[j][1])
			h.Add(h1)
		drawFakeRatio(_c, h, "singlet_" + add)

		if var == "MR_LElMap" : denom_singlet[0] = copy.deepcopy(h)
		if var == "MR_LLepMap": denom_singlet[1] = copy.deepcopy(h)
		if var == "MR_LMuMap" : denom_singlet[2] = copy.deepcopy(h)

		if var == "MR_TElMap" : makeFRplot(_c, h, denom_singlet[0], "MR_RElMap" , "singlet_" + add) 
		if var == "MR_TLepMap": makeFRplot(_c, h, denom_singlet[1], "MR_RLepMap", "singlet_" + add) 
		if var == "MR_TMuMap" : makeFRplot(_c, h, denom_singlet[2], "MR_RMuMap" , "singlet_" + add) 

	f.Close()





