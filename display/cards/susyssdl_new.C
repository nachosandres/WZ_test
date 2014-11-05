{
  if(Recompute) {
    MPAFDisplay md;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

  }
  else md.refresh();


  //general parameters ********************* general parameters
  string dir="SUSYSSDL";
  string fileName="susy_highpt"; //was treeName in LUNE
  string hName="";
 
  bool mcOnly = false;
  
  //if(md.isInitStatus()) {
    md.anConf.configureNames( dir, fileName, hName );
    md.anConf.configureData(false, 0, mcOnly);
    //}
  
  //observables **********************
  //string obs[6]={"","","","","",""};
    md.dp.setObservables("BR_MET");

  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=40;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={80.,250.};
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
  // LumisXS[ "DYJetsToLLM50_PU_S14_POSTLS170" ]= 856.;
  //LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170fake" ]=  20911.8;
  //LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170fake" ]=  176031.2;
  //LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170fake" ]= 757009.6;
  //LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170fake" ]=   2072135.3;
  //LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170fake" ]=  2811.0;
  //LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170fake" ]=  10421.0;
  //LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170fake" ]=  83347.9;
  //LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170fake" ]=  246396.2;
  //LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170fake" ]=  59873.5; 
  
  LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170_skimfake" ]=  20911.8;
  LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170_skimfake" ]=  176031.2;
  LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170_skimfake" ]= 757009.6;
  LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skimfake" ]=   2072135.3;
  LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skimfake" ]=  2811.0;
  LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skimfake" ]=  10421.0;
  LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skimfake" ]=  83347.9;
  LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skimfake" ]=  246396.2;
  LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skimfake" ]=  59873.5; 

  LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170_skimmisId" ]=  20911.8;
  LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170_skimmisId" ]=  176031.2;
  LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170_skimmisId" ]= 757009.6;
  LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skimmisId" ]=   2072135.3;
  LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skimmisId" ]=  2811.0;
  LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skimmisId" ]=  10421.0;
  LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skimmisId" ]=  83347.9;
  LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skimmisId" ]=  246396.2;
  LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skimmisId" ]=  59873.5; 

  LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170_skimprompt" ]=  20911.8;
  LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170_skimprompt" ]=  176031.2;
  LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170_skimprompt" ]= 757009.6;
  LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skimprompt" ]=   2072135.3;
  LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skimprompt" ]=  2811.0;
  LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skimprompt" ]=  10421.0;
  LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skimprompt" ]=  83347.9;
  LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skimprompt" ]=  246396.2;
  LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skimprompt" ]=  59873.5; 

  LumisXS[ "T1tttt2J_6_PU_S14_POSTLS170_skim" ]=105679 / 0.0141903 * 100; //CH: arbitrary rescaling (100) to make it visible
  LumisXS[ "T1tttt2J_7_PU_S14_POSTLS170_skim" ]=100322 / 0.0856418 * 100; //CH: arbitrary rescaling (100) to make it visible
  //LumisXS[ "T2tt_S14_skim" ]=1034154 /1.31169;
  LumisXS[ "T5Full_1200_1000_800_skim" ]= 121497 / 0.0856418 * 100; //CH: arbitrary rescaling (100) to make it visible
  LumisXS[ "T5Full_1500_800_100_skim" ]= 127139 / 0.0141903 * 100; //CH: arbitrary rescaling (100) to make it visible
  LumisXS[ "TTHnlo_S14_skim" ] = 213300 / 0.5085  ;
  LumisXS[ "TTWJets_S14_skim" ] = 249415 / 1.152;
  LumisXS[ "TTZJets_S14_skim" ] = 249275 / 2.232 ;
  

  //via XSect
  
  map<string,float> KFactors;
     // if( md.isInitStatus() )
  md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // SDYJetsM50_HT600toInf_PU_S14_POSTLS170_skimamples **************************  samples
  //if( md.isInitStatus() ) {
//  md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skim",             "t#bar{t}",    kBlue+1 );
//  md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skim",             "W+jets",    kAzure-2 );
//  md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skim",             "W+jets",    kAzure-2 );
//  md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skim",             "W+jets",    kAzure-2 );
//  md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skim",             "W+jets",    kAzure-2 );
//  md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170_skim",             "Z+jets",    kAzure+6 );
//  md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170_skim",             "Z+jets",    kAzure+6 );
//  md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170_skim",             "Z+jets",    kAzure+6 );
//  md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skim",             "Z+jets",    kAzure+6 );
 
  md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skimfake",             "t#bar{t} fake",    kBlue+1 );
  md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skimfake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skimfake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skimfake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skimfake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170_skimfake",             "Z+jets fake",    kAzure+6 );
  md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170_skimfake",             "Z+jets fake",    kAzure+6 );
  md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170_skimfake",             "Z+jets fake",    kAzure+6 );
  md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skimfake",             "Z+jets fake",    kAzure+6 );

  md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skimmisId",             "t#bar{t} misId",    kRed+1 );
  //md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skimmisId",             "W+jets misId",    kOrange+5 );
  //md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skimmisId",             "W+jets misId",    kOrange+5 );
  //md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skimmisId",             "W+jets misId",    kOrange+5 );
  //md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skimmisId",             "W+jets misId",    kOrange+5 );
  md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170_skimmisId",             "Z+jets misId",    kRed-6 );
  md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170_skimmisId",             "Z+jets misId",    kRed-6 );
  md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170_skimmisId",             "Z+jets misId",    kRed-6 );
  md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skimmisId",             "Z+jets misId",    kRed-6 );

  //md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skimprompt",      "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skimprompt",           "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skimprompt",           "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skimprompt",           "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skimprompt",           "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170_skimprompt",            "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170_skimprompt",            "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170_skimprompt",            "t#bar{t}/EWK prompt", kOrange+7 );
  //md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skimprompt",            "t#bar{t}/EWK prompt", kOrange+7 );
  
  md.anConf.addSample( "TTHnlo_S14_skim",             "Rare",    kOrange-2 );
  md.anConf.addSample( "TTWJets_S14_skim",             "Rare",    kOrange-2 );
  md.anConf.addSample( "TTZJets_S14_skim",             "Rare",    kOrange-2 );
  md.anConf.addSample( "T1tttt2J_7_PU_S14_POSTLS170_skim",        "T1tttt(HM) sig",    kGreen+2 );
  md.anConf.addSample( "T1tttt2J_6_PU_S14_POSTLS170_skim",        "T1tttt(HL) sig",    kGreen+2 );
  //md.anConf.addSample( "T2tt_S14_skim",                         "T2tt sig",    kGreen+2 );
  md.anConf.addSample( "T5Full_1200_1000_800_skim",               "T5WW(HM) sig",    kGreen+2 );
  md.anConf.addSample( "T5Full_1500_800_100_skim",                "T5WW(HL) sig",    kGreen+2 );

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

 md.doPlot();
 md.savePlot("SUSYSSDL_highpt");
 // md.dp.addText(xt,yt,st,addText);

}
