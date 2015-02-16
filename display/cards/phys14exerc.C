{
  if(Recompute) {
    MPAFDisplay md;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

  }
  else md.refresh();


  //general parameters ********************* general parameters
  string dir="phys14exerc";
  string fileName="phys14exerc_SR20"; // not needed for statistics
  string fileList="phys14exerc_SR00_hh_all"; // put command line that gives all files as in a "ls" command
  //string fileList="phys14exerc_SIGNALREGION_PTSCENARIO_FLAVORS"; // put command line that gives all files as in a "ls" command
  string hName=""; // not needed for statistics

  bool mcOnly = false;
  
  //if(md.isInitStatus()) {
    md.anConf.configureNames( dir, fileName, fileList, hName, true );
    md.anConf.configureData(false, 0, mcOnly);
    //}

  //observables **********************
  //string obs[6]={"","","","","",""};
    md.dp.setObservables("SR_MET");
  

  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=40;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={0.,500.};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=true;
  bool underFlowBin=false;
  bool showDMCRatio=true;
  bool showGrid=false;
  float markerSize=0.8;
  float lineWidth=2;

  bool summedSignal=false;
  bool stacking=true;

  bool cmsPrel=true;

  float xt=0.68;
  float yt=0.48;
  float st=0.039;
  string addText="";

  string autoBinFile="susybinninghigh";
  md.dp.loadAutoBinning(autoBinFile);

  //Systematic uncertainties ********************************
  bool addSystematics=true;
  
  bool mcStatSyst=true;
  string systSources="";

  bool uncDet=false;

  string Norm="";
  
  //Lumis( or XSections ) pb-1 & KFactors ************************************
  float lumi=10000; //pb-1 19470
  float energy=13; //TeV

  bool useXS=false;

  map<string,float> LumisXS;

  //LumisXS[ "WJetsToLNu_13TeV-madgraph-pythia8-tauola_skim"                    ] = 10017462 / (20508.9); 
  LumisXS[ "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_skim"         ] = 5262265  / (1817.0   * 1.23);
  LumisXS[ "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_skim"         ] = 4936077  / ( 471.6   * 1.23); 
  LumisXS[ "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_skim"         ] = 4640594  / (  55.61  * 1.23);
  LumisXS[ "WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skim"         ] = 4581841  / (  18.81  * 1.23);
 
  //LumisXS[ "DYJetsToLL_M-50_13TeV-madgraph-pythia8_skimfake"                  ] = 2829164  / 6024;
  LumisXS[ "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola_skimfake"] = 4054159  / ( 194.3   * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola_skimfake"] = 4666496  / (  52.24  * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_skimfake"] = 4931372  / (   6.546 * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skimfake"] = 4493574  / (   2.179 * 1.27);
  LumisXS[ "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimfake" ] = 25446993 / 809.1;

  //LumisXS[ "DYJetsToLL_M-50_13TeV-madgraph-pythia8_skimmisId"                  ] = 2829164  / 6024;
  LumisXS[ "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola_skimmisId"] = 4054159  / ( 194.3   * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola_skimmisId"] = 4666496  / (  52.24  * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_skimmisId"] = 4931372  / (   6.546 * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skimmisId"] = 4493574  / (   2.179 * 1.27);
  LumisXS[ "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimmisId" ] = 25446993 / 809.1;

  //LumisXS[ "DYJetsToLL_M-50_13TeV-madgraph-pythia8_skimprompt"                  ] = 2829164  / 6024;
  LumisXS[ "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola_skimprompt"] = 4054159  / ( 194.3   * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola_skimprompt"] = 4666496  / (  52.24  * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_skimprompt"] = 4931372  / (   6.546 * 1.27);
  LumisXS[ "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skimprompt"] = 4493574  / (   2.179 * 1.27);
  LumisXS[ "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimprompt" ] = 25446993 / 809.1;

  LumisXS[ "TTWJets_Tune4C_13TeV-madgraph-tauola_skim"                        ] = 246521 / 0.6647;
  LumisXS[ "TTZJets_Tune4C_13TeV-madgraph-tauola_skim"                        ] = 249275 / 0.8565;
  LumisXS[ "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_skim"                   ] = 237484 / 2.29;

  LumisXS[ "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_skim"] = 100322 / (0.0856418 );
  //LumisXS[ "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_skim"] = 105679 / (0.0141903 );
  //LumisXS[ "T1ttbbWW_mGo1300_mCh300_mChi290_skim"                             ] = 43975  / (0.0460525 );
  LumisXS[ "T1ttbbWW_mGo1000_mCh725_mChi720_skim"                             ] = 52212  / (0.325388  );
  LumisXS[ "T5ttttDeg_mGo1000_mStop300_mChi280_skim"                          ] = 52102  / (0.325388  );
  //LumisXS[ "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_skim"                   ] = 52499  / (0.3254    );
  LumisXS[ "T5qqqqWW_mGo1200_mCh1000_mChi800_skim"                            ] = 112720 / (0.0856418 );
  LumisXS[ "T6ttWW_mSbot600_mCh425_mChi50_skim"                               ] = 52954  / (0.174599  );
  //LumisXS[ "T6ttWW_mSbot650_mCh150_mChi50_skim"                               ] = 105124 / (0.107045  );


  //via XSect
  
  map<string,float> KFactors;
     // if( md.isInitStatus() )
  md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // SDYJetsM50_HT600toInf_PU_S14_POSTLS170_skimamples **************************  samples
  //if( md.isInitStatus() ) {



  //md.anConf.addSample( "WJetsToLNu_13TeV-madgraph-pythia8-tauola_skim"                    , "fake", kAzure-2); 
  md.anConf.addSample( "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_skim"         , "fake", kAzure-2); 
  md.anConf.addSample( "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_skim"         , "fake", kAzure-2);  
  md.anConf.addSample( "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_skim"         , "fake", kAzure-2); 
  md.anConf.addSample( "WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skim"         , "fake", kAzure-2); 
 
  //md.anConf.addSample( "DYJetsToLL_M-50_13TeV-madgraph-pythia8_skimfake"                  , "fake", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola_skimfake", "fake", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola_skimfake", "fake", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_skimfake", "fake", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skimfake", "fake", kAzure+6); 
  md.anConf.addSample( "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimfake" , "fake", kBlue+1); 

  //md.anConf.addSample( "DYJetsToLL_M-50_13TeV-madgraph-pythia8_skimmisId"                  , "misId", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola_skimmisId", "misId", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola_skimmisId", "misId", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_skimmisId", "misId", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skimmisId", "misId", kRed-6); 
  md.anConf.addSample( "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimmisId" , "misId", kRed+1); 

  //md.anConf.addSample( "DYJetsToLL_M-50_13TeV-madgraph-pythia8_skimprompt"                  , "prompt", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola_skimprompt", "prompt", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola_skimprompt", "prompt", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_skimprompt", "prompt", kRed-6); 
  md.anConf.addSample( "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_skimprompt", "prompt", kRed-6); 
  md.anConf.addSample( "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimprompt" , "prompt", kRed+1); 

  md.anConf.addSample( "TTWJets_Tune4C_13TeV-madgraph-tauola_skim"                        , "rare", kOrange-2); 
  md.anConf.addSample( "TTZJets_Tune4C_13TeV-madgraph-tauola_skim"                        , "rare", kOrange-2); 
  md.anConf.addSample( "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola_skim"                   , "rare", kOrange-2); 

  md.anConf.addSample( "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_skim", "T1tttt (1.2/0.8) sig", kViolet-3);
  //md.anConf.addSample( "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_skim", "T1tttt (1.5/0.1) sig", kViolet-3);
  //md.anConf.addSample( "T1ttbbWW_mGo1300_mCh300_mChi290_skim"                             , "T1ttbbWW (1.3/0.3) sig", kBlue); 
  md.anConf.addSample( "T1ttbbWW_mGo1000_mCh725_mChi720_skim"                             , "T1ttbbWW (1.0/0.7) sig ", kBlue); 
  md.anConf.addSample( "T5ttttDeg_mGo1000_mStop300_mChi280_skim"                          , "T5tttt deg. (1.0/0.3, 4bd) sig", kGreen+2); 
  //md.anConf.addSample( "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_skim"                   , "T5tttt deg. (1.0/0.3, Chi) sig", kGreen+2); 
  md.anConf.addSample( "T5qqqqWW_mGo1200_mCh1000_mChi800_skim"                            , "T5qqqqWW (1.2/0.8) sig", kOrange+6); 
  md.anConf.addSample( "T6ttWW_mSbot600_mCh425_mChi50_skim"                               , "T6ttWW (600/425/50) sig", kRed+1); 
  //md.anConf.addSample( "T6ttWW_mSbot650_mCh150_mChi50_skim"                               , "T6ttWW (650/150/50) sig", kRed+1); 
  
   

  // }
  //===============================================================

  //*********************************************************************²
  //Execution macro ******************************************************
 
  //Configuration ================
  //if( md.isInitStatus() ) {
  
  //md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );
  // }

  //plotting ================
  md.dp.setLumiAndEnergy( lumi, energy );
  md.dp.setNormalization( Norm );
  md.dp.configureDisplay(yTitle, rangeY, rangeX, logYScale, xDiv,
          yDiv, binning, addBinBkg, overFlowBin,
          underFlowBin, showDMCRatio, showGrid,
          stacking, addSystematics, mcStatSyst,
          markerSize, lineWidth,summedSignal,
          mcOnly,cmsPrel, uncDet);

  md.prepareDisplay();
  md.doPlot();
  md.getStatistics();
  //md.makeDataCard(fileList, "global", "SR b-jet multiplicity");
  //md.savePlot("phys14exerc");
  // md.dp.addText(xt,yt,st,addText);

  //gROOT->ProcessLine(".q");

}
