MPAFDisplay md;

void susy3l_datacard() {
    md.refresh();


    //general parameters ********************* general parameters
    string dir="SUSY3L";
    string fileName="template3L_skimmed"; // not needed for statistics but provides the normalization
    string fileList="template3L_skimmed"; // put command line that gives all files as in a "ls" command

    bool mcOnly = false;
  
    md.anConf.configureNames( dir, fileName, fileList );
    md.anConf.configureData(false, 0, mcOnly);
  
  
    //Lumis( or XSections ) pb-1 & KFactors ************************************
    float lumi=10000; //pb-1 19470
    float energy=13; //TeV

    bool useXS=false;

    md.anConf.loadXSDB("XSectionsPhys14.db");

    map<string,float> LumisXS;

    //via XSect
    map<string,float> KFactors;
  

    md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

    //===============================================================

    //top    
    md.addDataCardBkgSample("TBarToLeptons_sch_skim", "fake");
    md.addDataCardBkgSample("TBarToLeptons_tch_skim", "fake");
    md.addDataCardBkgSample("TBar_tWch_skim", "fake");
    md.addDataCardBkgSample("TTJets_skim", "fake");
    md.addDataCardBkgSample("TToLeptons_sch_skim", "fake");
    md.addDataCardBkgSample("TToLeptons_tch_skim", "fake");
    md.addDataCardBkgSample("T_tWch_skim", "fake");
    

    //W+Jets
    md.addDataCardBkgSample("WJetsToLNu_HT100to200_skim", "fake");
    md.addDataCardBkgSample("WJetsToLNu_HT200to400_skim", "fake");
    //md.addDataCardBkgSample("WJetsToLNu_HT400to600_skim", "fake");
    md.addDataCardBkgSample("WJetsToLNu_HT600toInf_skim", "fake");

    //Drell-Yan
    md.addDataCardBkgSample("DYJetsToLL_M50_HT100to200_skim", "fake");
    md.addDataCardBkgSample("DYJetsToLL_M50_HT200to400_skim", "fake");
    md.addDataCardBkgSample("DYJetsToLL_M50_HT400to600_skim", "fake");
    md.addDataCardBkgSample("DYJetsToLL_M50_HT600toInf_skim", "fake");

    //di-boson
    md.addDataCardBkgSample("WZJetsTo3LNu_skim", "diboson");
    md.addDataCardBkgSample("ZZTo4L_skim", "diboson");
    
    //TTV
    md.addDataCardBkgSample("TTZJets_skim", "ttZ");
    md.addDataCardBkgSample("TTWJets_skim", "ttW");

    //rare
    md.addDataCardBkgSample("TTH_skim", "rare");
    md.addDataCardBkgSample("GGHZZ4L_skim", "rare");

    //signal
    //md.addDataCardSigSample("SMS_T1tttt_2J_mGl1200_mLSP800_skim","T1t412");
    md.addDataCardSigSample("SMS_T1tttt_2J_mGl1500_mLSP100_skim","T1t415");
    //md.addDataCardSigSample("T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_skim","T5t410");
    //md.addDataCardSigSample("T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_skim","T5t413");
  
   
    md.addNuisanceParameter("fratio","fake","lnN","1.50");
    md.addNuisanceParameter("rNorm","rare","lnN","1.20");
    md.addNuisanceParameter("sigNorm","T1t415","lnN","1.10");


    // }
    //===============================================================

    //*********************************************************************²
    //Execution macro ******************************************************
  
    //plotting ================
    md.dp.setLumiAndEnergy( lumi, energy );
    md.prepareDisplay();
  
    int nCateg=1; 
    vector<string> _categs(nCateg,"");
 
    // string srs[66]={
    // "SR1AL", "SR2AL", "SR3AL", "SR4AL", "SR1AH", "SR2AH", "SR3AH", "SR4AH", "SR5A", "SR6A",
    // "SR7AL", "SR8AL", "SR9AL", "SR10AL", "SR7AH", "SR8AH", "SR9AH", "SR10AH", "SR11A", "SR12A",
    // "SR13AL", "SR14AL", "SR15AL", "SR16AL", "SR13AH", "SR14AH", "SR15AH", "SR16AH", "SR17A", "SR18A",
    //"SR19AL", "SR19AH", "SR20A",
    // "SR1BL", "SR1BH", "SR2BL", "SR2BH",
    // "SR3BL", "SR3BH", "SR4BL", "SR4BH",
    // "SR5BL", "SR5BH", "SR6BL", "SR6BH",
    // "SR7BL", "SR7BH", 
    // };

    //string srs[28]={ 
    //    "SR1A", "SR2A", "SR3A", "SR4A", "SR5A", "SR6A", 
    //    "SR7A", "SR8A", "SR9A", "SR10A",  "SR11A", "SR12A",
    //    "SR13A", "SR14A", "SR15A", "SR16A", "SR17A", "SR18A", 
    //    "SR19A", "SR20A",
    //    "SR1B", "SR2B", 
    //    "SR3B", "SR4B",
    //    "SR5B", "SR6B",
    //    "SR7B", "SR8B" };

    string srs[1]={ 
        "SR00_on" };


    _categs.assign(srs, srs+nCateg);
    
    for(size_t ic=0;ic< _categs.size();ic++) {
        string cat=_categs[ic];
        md.makeSingleDataCard("T1t415", "global_"+cat, "SR MET selection", "susy3l_"+cat+"_T1t415");
    }

    gROOT->ProcessLine(".q");

}
