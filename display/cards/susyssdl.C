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
  string fileName="susy_lowpt"; //was treeName in LUNE
  string hName="";
 
  bool mcOnly = false;
  
  //if(md.isInitStatus()) {
    md.anConf.configureNames( dir, fileName, hName );
    md.anConf.configureData(false, 0, mcOnly);
    //}
  
  //observables **********************
  //string obs[6]={"","","","","",""};
    md.dp.setObservables("BR_NMuons");

  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=0;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={0.,3.};
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
  LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170fake" ]=  20911.8;
  LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170fake" ]=  176031.2;
  LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170fake" ]= 757009.6;
  LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170fake" ]=   2072135.3;
  LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170fake" ]=  2811.0;
  LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170fake" ]=  10421.0;
  LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170fake" ]=  83347.9;
  LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170fake" ]=  246396.2;
  LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170fake" ]=  59873.5; 

 LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170misId" ]=  20911.8;
  LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170misId" ]=  176031.2;
  LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170misId" ]= 757009.6;
  LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170misId" ]=   2072135.3;
  LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170misId" ]=  2811.0;
  LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170misId" ]=  10421.0;
  LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170misId" ]=  83347.9;
  LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170misId" ]=  246396.2;
  LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170misId" ]=  59873.5; 

 LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170prompt" ]=  20911.8;
  LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170prompt" ]=  176031.2;
  LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170prompt" ]= 757009.6;
  LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170prompt" ]=   2072135.3;
  LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170prompt" ]=  2811.0;
  LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170prompt" ]=  10421.0;
  LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170prompt" ]=  83347.9;
  LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170prompt" ]=  246396.2;
  LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170prompt" ]=  59873.5; 


  //via XSect
  
  map<string,float> KFactors;
     // if( md.isInitStatus() )
  md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // Samples **************************  samples
  //if( md.isInitStatus() ) {
  md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170fake",             "t#bar{t} fake",    kBlue+1 );
  md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170fake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170fake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170fake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170fake",             "W+jets fake",    kAzure-2 );
  md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170fake",             "Z+jets fake",    kAzure+6 );
  md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170fake",             "Z+jets fake",    kAzure+6 );
  md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170fake",             "Z+jets fake",    kAzure+6 );
  md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170fake",             "Z+jets fake",    kAzure+6 );

  md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170misId",             "t#bar{t} misId",    kRed+1 );
  md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170misId",             "W+jets misId",    kOrange+5 );
  md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170misId",             "W+jets misId",    kOrange+5 );
  md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170misId",             "W+jets misId",    kOrange+5 );
  md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170misId",             "W+jets misId",    kOrange+5 );
  md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170misId",             "Z+jets misId",    kRed-6 );
  md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170misId",             "Z+jets misId",    kRed-6 );
  md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170misId",             "Z+jets misId",    kRed-6 );
  md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170misId",             "Z+jets misId",    kRed-6 );

  md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170prompt",             "t#bar{t} prompt",    kOrange-2 );
  md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170prompt",             "W+jets prompt",    kOrange );
  md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170prompt",             "W+jets prompt",    kOrange );
  md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170prompt",             "W+jets prompt",    kOrange );
  md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170prompt",             "W+jets prompt",    kOrange );
  md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170prompt",             "Z+jets prompt",    kOrange-4 );
  md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170prompt",             "Z+jets prompt",    kOrange-4 );
  md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170prompt",             "Z+jets prompt",    kOrange-4 );
  md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170prompt",             "Z+jets prompt",    kOrange-4 );

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
 md.savePlot("test");
 // md.dp.addText(xt,yt,st,addText);

}
