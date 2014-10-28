{
  if(Recompute) {
    MPAFDisplay md;
 
    //Bloody René Brun
    bool * rtmp= const_cast<bool*> pr;
    *rtmp = false;

  }
  else md.refresh();


  //general parameters ********************* general parameters
  string dir="test";
  string fileName="test"; //was treeName in LUNE
  string hName="";
 
  bool mcOnly = false;
  
  //if(md.isInitStatus()) {
    md.anConf.configureNames( dir, fileName, hName );
    md.anConf.configureData(false, 0, mcOnly);
    //}
  
  //observables ********************** 
  //string obs[6]={"","","","","",""};
  md.dp.setObservables("qT");//STVsMassFinal

  //Binning & title ************************* Binning & titre
  string yTitle="number of events";
  int binning=1;
  int addBinBkg=1; //BinB = binning*AddBin
  double rangeY[2]={0,0};
  double rangeX[2]={10.,200.};
  int xDiv[3]={8,6,0};
  int yDiv[3]={6,6,0}; //Nlabel /  sous-Div /ssdiv
  bool logYScale=false;
  bool overFlowBin=true;
  bool underFlowBin=false;
  bool showDMCRatio=false;
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
  float lumi=1000; //pb-1 19470

  bool useXS=true;

  map<string,float> LumisXS;
  LumisXS[ "top" ]=1.;
  LumisXS[ "Z" ]=100.;
  LumisXS[ "W" ]=200.;

  //via XSect
  
  map<string,float> KFactors;
     // if( md.isInitStatus() )
  md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================
  // Samples **************************  samples
  //if( md.isInitStatus() ) {
    md.anConf.addSample( "top",             "top",    kRed+1 );
    md.anConf.addSample( "Z",             "Z",    kBlue+1 );
    md.anConf.addSample( "W",             "W",    kGreen+1 );

    // }
 //===============================================================

 //*********************************************************************²
 //Execution macro ******************************************************
 
 //Configuration ================
  //if( md.isInitStatus() ) {
  
   //md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );
   
   // }

 //plotting ================
 md.dp.setLumi( lumi );
 md.dp.setNormalization( Norm );
 md.dp.configureDisplay(yTitle, rangeY, rangeX, logYScale, xDiv,
 			yDiv, binning, addBinBkg, overFlowBin,
 			underFlowBin, showDMCRatio, showGrid, 
			stacking, addSystematics, mcStatSyst,
			markerSize, lineWidth,summedSignal, 
 			mcOnly,cmsPrel, uncDet);

 md.doPlot();

 // md.dp.addText(xt,yt,st,addText);

}
