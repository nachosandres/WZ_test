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
  string fileName="susy_replaceSIGNALREGION_replaceLEPPT_replaceLEPID_replaceBTAG_replaceLEPFLAV"; // not needed for statistics
  string fileList="susy_replaceSIGNALREGION_replaceLEPPT_replaceLEPID_replaceBTAG_replaceLEPFLAV"; // put command line that gives all files as in a "ls" command

  string hName=""; // not needed for statistics

  bool mcOnly = false;
  
  //if(md.isInitStatus()) {
    md.anConf.configureNames( dir, fileName, fileList, hName, true );
    md.anConf.configureData(false, 0, mcOnly);
    //}

  //observables **********************
  //string obs[6]={"","","","","",""};
    md.dp.setObservables("replaceVARIABLE");
  

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

  string autoBinFile="susybinning";
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
  LumisXS[ "WJetsToLNu_HT100to200_skim"         ] = 5262265  / (1817.0   * 1.23);
  LumisXS[ "WJetsToLNu_HT200to400_skim"         ] = 4936077  / ( 471.6   * 1.23); 
  LumisXS[ "WJetsToLNu_HT400to600_skim"         ] = 4640594  / (  55.61  * 1.23);
  LumisXS[ "WJetsToLNu_HT600toInf_skim"         ] = 237484   / (  18.81  * 1.23);
 
  //LumisXS[ "DYJetsToLL_M50_skim"                      ] = 2829164  / 6024;
  LumisXS[ "DYJetsToLL_M50_HT100to200_skim"    ] = 4054159  / ( 194.3   * 1.27);
  LumisXS[ "DYJetsToLL_M50_HT200to400_skim"    ] = 4666496  / (  52.24  * 1.27);
  LumisXS[ "DYJetsToLL_M50_HT400to600_skim"    ] = 4931372  / (   6.546 * 1.27);
  LumisXS[ "DYJetsToLL_M50_HT600toInf_skim"    ] = 4493574  / (   2.179 * 1.27);

  LumisXS[ "TTJets_skim"     ] = 25446993 / 809.1;

  LumisXS[ "TTWJets_skim"                        ] = 246521 / 0.6647;
  LumisXS[ "TTZJets_skim"                        ] = 249275 / 0.8565;
  LumisXS[ "WZJetsTo3LNu_skim"                   ] = 237484 / 2.29;

  LumisXS[ "SMS_T1tttt_2J_mGl1200_mLSP800_skim"] = 100322 / (0.0856418 * 20.);
  //LumisXS[ "SMS_T1tttt_2J_mGl1500_mLSP100_skim"] = 105679 / (0.0141903 * 20.);
  //LumisXS[ "T1ttbbWW_mGo1300_mCh300_mChi290_skim"                             ] = 43975  / (0.0460525 * 20.);
  LumisXS[ "T1ttbbWW_mGo1000_mCh725_mChi720_skim"                             ] = 52212  / (0.325388  * 20.);
  LumisXS[ "T5ttttDeg_mGo1000_mStop300_mChi280_skim"                          ] = 52102  / (0.325388  * 20.);
  //LumisXS[ "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_skim"                   ] = 52499  / (0.3254    * 20.);
  //LumisXS[ "T6ttWW_mSbot650_mCh150_mChi50_skim"                               ] = 105124 / (0.107045  * 20.);
  LumisXS[ "T6ttWW_mSbot600_mCh425_mChi50_skim"                               ] = 52954  / (0.174599  * 20.);
  LumisXS[ "T5qqqqWW_mGo1200_mCh1000_mChi800_dilep_skim"                            ] = 112720 / (0.0856418 * 20.);


  //via XSect
  
  map<string,float> KFactors;
     // if( md.isInitStatus() )
  md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // SDYJetsM50_HT600toInf_PU_S14_POSTLS170_skimamples **************************  samples
  //if( md.isInitStatus() ) {



  //md.anConf.addSample( "WJetsToLNu_13TeV-madgraph-pythia8-tauola_skim"                    , "W+Jets", kAzure-2); 
  md.anConf.addSample( "WJetsToLNu_HT100to200_skim"         , "W+Jets", kAzure-2); 
  md.anConf.addSample( "WJetsToLNu_HT200to400_skim"         , "W+Jets", kAzure-2);  
  md.anConf.addSample( "WJetsToLNu_HT400to600_skim"         , "W+Jets", kAzure-2); 
  md.anConf.addSample( "WJetsToLNu_HT600toInf_skim"         , "W+Jets", kAzure-2); 
 
  //md.anConf.addSample( "DYJetsToLL_M50_skim"                      , "Z+Jets", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M50_HT100to200_skim"    , "Z+Jets", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M50_HT200to400_skim"    , "Z+Jets", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M50_HT400to600_skim"    , "Z+Jets", kAzure+6); 
  md.anConf.addSample( "DYJetsToLL_M50_HT600toInf_skim"    , "Z+Jets", kAzure+6); 

  md.anConf.addSample( "TTJets_skim"     , "t#bar{t}", kBlue+1); 

  md.anConf.addSample( "TTWJets_skim"                        , "rare", kOrange-2); 
  md.anConf.addSample( "TTZJets_skim"                        , "rare", kOrange-2); 
  md.anConf.addSample( "WZJetsTo3LNu_skim"                   , "rare", kOrange-2); 

  md.anConf.addSample( "SMS_T1tttt_2J_mGl1200_mLSP800_skim", "T1tttt (1.2/0.8) sig * 20", kViolet-3);
  //md.anConf.addSample( "SMS_T1tttt_2J_mGl1500_mLSP100_skim", "T1tttt (1.5/0.1) sig * 20", kViolet-3);
  //md.anConf.addSample( "T1ttbbWW_mGo1300_mCh300_mChi290_skim"                             , "T1ttbbWW (1.3/0.3) sig * 20", kBlue); 
  md.anConf.addSample( "T1ttbbWW_mGo1000_mCh725_mChi720_skim"                             , "T1ttbbWW (1.0/0.7) sig * 20", kBlue); 
  md.anConf.addSample( "T5ttttDeg_mGo1000_mStop300_mChi280_skim"                          , "T5tttt deg. (1.0/0.3, 4bd) sig * 20", kGreen+2); 
  //md.anConf.addSample( "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_skim"                   , "T5tttt deg. (1.0/0.3, Chi) sig * 20", kGreen+2); 
  //md.anConf.addSample( "T6ttWW_mSbot650_mCh150_mChi50_skim"                               , "T6ttWW (650/150/50) sig * 20", kRed+1); 
  md.anConf.addSample( "T6ttWW_mSbot600_mCh425_mChi50_skim"                               , "T6ttWW (600/425/50) sig * 20", kRed+1); 
  md.anConf.addSample( "T5qqqqWW_mGo1200_mCh1000_mChi800_dilep_skim"                            , "T5qqqqWW (1.2/0.8) sig * 20", kOrange+6); 
  
   

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
  //md.makeDataCard(fileList, "global", "SR b-jet multiplicity");
  md.savePlot("phys14exerc");
  // md.dp.addText(xt,yt,st,addText);

  //gROOT->ProcessLine(".q");

}
