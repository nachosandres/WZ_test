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
  string fileName="testconfigname"; //was treeName in LUNE
  string hName="";
 
  bool mcOnly = false;
  
  //if(md.isInitStatus()) {
    md.anConf.configureNames( dir, fileName, hName );
    md.anConf.configureData(false, 0, mcOnly);
    //}
  
  //observables ********************** 
  //string obs[6]={"","","","","",""};
  md.dp.setObservables("SR_HT");//STVsMassFinal

  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=10;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={200.,500.};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=false;
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
  LumisXS[ "DYJetsM50_HT100to200_PU_S14_POSTLS170" ]=  20911.8;
  LumisXS[ "DYJetsM50_HT200to400_PU_S14_POSTLS170" ]=  176031.2;
  LumisXS[ "DYJetsM50_HT400to600_PU_S14_POSTLS170" ]= 757009.6;
  LumisXS[ "DYJetsM50_HT600toInf_PU_S14_POSTLS170" ]=   2072135.3;
  LumisXS[ "WJetsToLNu_HT100to200_PU_S14_POSTLS170" ]=  2811.0;
  LumisXS[ "WJetsToLNu_HT200to400_PU_S14_POSTLS170" ]=  10421.0;
  LumisXS[ "WJetsToLNu_HT400to600_PU_S14_POSTLS170" ]=  83347.9;
  LumisXS[ "WJetsToLNu_HT600toInf_PU_S14_POSTLS170" ]=  246396.2;
  LumisXS[ "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170" ]=  59873.5; 

  //via XSect
  
  map<string,float> KFactors;
     // if( md.isInitStatus() )
  md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // Samples **************************  samples
  //if( md.isInitStatus() ) {
  md.anConf.addSample( "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170",             "t#bar{t}",    kRed+1 );
  md.anConf.addSample( "WJetsToLNu_HT100to200_PU_S14_POSTLS170",             "W+jets",    kOrange+7 );
  md.anConf.addSample( "WJetsToLNu_HT200to400_PU_S14_POSTLS170",             "W+jets",    kOrange+7 );
  md.anConf.addSample( "WJetsToLNu_HT400to600_PU_S14_POSTLS170",             "W+jets",    kOrange+7 );
  md.anConf.addSample( "WJetsToLNu_HT600toInf_PU_S14_POSTLS170",             "W+jets",    kOrange+7 );
  md.anConf.addSample( "DYJetsM50_HT100to200_PU_S14_POSTLS170",             "Z+jets",    kOrange-2 );
  md.anConf.addSample( "DYJetsM50_HT200to400_PU_S14_POSTLS170",             "Z+jets",    kOrange-2 );
  md.anConf.addSample( "DYJetsM50_HT400to600_PU_S14_POSTLS170",             "Z+jets",    kOrange-2 );
  md.anConf.addSample( "DYJetsM50_HT600toInf_PU_S14_POSTLS170",             "Z+jets",    kOrange-2 );

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
