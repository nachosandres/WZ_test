
{


    if(Recompute) {
        MPAFDisplay md;
 
        bool * rtmp= const_cast<bool*> pr;
        *rtmp = false;

    }
    else 
    md.refresh();


    //general parameters ********************* general parameters
    string dir="SUSY3L";
    string fileName="template3L_skimmed"; //was treeName in LUNE susy_cut_lowpt
    string fileList="template3L_skimmed"; //CH: since AnaConfig needs a fileName to open, we need to put the data files into a different variable
    string hName="";

    bool mcOnly = false;
  
    //if(md.isInitStatus()) {
    md.anConf.configureNames( dir, fileName, fileList, hName );
    md.anConf.configureData(false, 0, mcOnly);
    //}
  
    //observables **********************
    //string obs[6]={"","","","","",""};
    //md.dp.setObservables("Zmass");
    //md.dp.setObservables("deltaR_elmu");
    //md.dp.setObservables("BR_NJets");
    //md.dp.setObservables("BR_NBJets");
    md.dp.setObservables("BR_HT");
    //md.dp.setObservables("BR_MET");
    //md.dp.setObservables("SR_NJets");
    //md.dp.setObservables("SR_NBJets");
    //md.dp.setObservables("SR_HT");
    //md.dp.setObservables("SR_MET");


    //Binning & title ************************* Binning & titre
    string yTitle="number of events";
    int binning=50;
    int addBinBkg=1; //BinB = binning*AddBin
    double rangeY[2]={0,0};
    double rangeX[2]={0,800};
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

    //string autoBinFile="susybinninghigh";
    //md.dp.loadAutoBinning(autoBinFile);

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

    //cross sections   
    
    //Drell-Yan
    //LumisXS[ "DYJetsToLL_M-50_13TeV-madgraph-pythia8"                       ] = 2829164 / (6024             );
    LumisXS[ "DYJetsToLL_M50_HT100to200_skim"                ] = 4054159 / ( 194.3   * 1.27  );
    LumisXS[ "DYJetsToLL_M50_HT200to400_skim"                ] = 4666496 / (  52.24  * 1.27  );
    LumisXS[ "DYJetsToLL_M50_HT400to600_skim"                ] = 4931372 / (   6.546 * 1.27  );
    LumisXS[ "DYJetsToLL_M50_HT600toInf_skim"                ] = 4493574 / (   2.179 * 1.27  );

    //Higgs->ZZ
    LumisXS[ "GGHZZ4L_skim"                                  ] =  204684 / (0.01212192       );  // cross-section: 43.92*2.76E-04
    
    //signal
    LumisXS[ "SMS_T1tttt_2J_mGl1200_mLSP800_skim"            ] =  100322 / (0.0856418 * 20.  );
    LumisXS[ "SMS_T1tttt_2J_mGl1500_mLSP100_skim"            ] =  105679 / (0.0141903 * 20.  );
    LumisXS[ "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_skim"] =   52499 / (0.3254    * 20.  );
    LumisXS[ "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_skim"] =       0 / (1         * 1    );
    
    LumisXS[ "TBarToLeptons_sch_skim"                        ] =  250000 / (1.34784          );  // 4.16*0.108*3
    LumisXS[ "TBarToLeptons_tch_skim"                        ] = 1999800 / (26.23428         );  // 80.97*0.108*3
    LumisXS[ "TBar_tWch_skim"                                ] =  971800 / (35.6             ); 
    LumisXS[ "TTH_skim"                                      ] =  199700 / (1         * 1    );
    LumisXS[ "TTJets_skim"                                   ] =25446993 / (809.1            );
    LumisXS[ "TToLeptons_sch_skim"                           ] =  500000 / (2.3328           );  // 7.20*0.108*3
    LumisXS[ "TToLeptons_tch_skim"                           ] = 3991000 / (44.0802          );  // 136.05*0.108*3
    LumisXS[ "T_tWch_skim"                                   ] =  986100 / (35.6             );  // 35.6
   
    //TTV 
    LumisXS[ "TTWJets_skim"                                  ] =  246521 / (0.6647           );
    LumisXS[ "TTZJets_skim"                                  ] =  249275 / (0.8565           );
      
    //LumisXS[ "WJetsToLNu_13TeV-madgraph-pythia8-tauola"                    ] = 10017462 / (20508.9        ); 
    LumisXS[ "WJetsToLNu_HT100to200_skim"                    ] = 5262265  / (1817.0   * 1.23 );
    LumisXS[ "WJetsToLNu_HT200to400_skim"                    ] = 4936077  / ( 471.6   * 1.23 );
    LumisXS[ "WJetsToLNu_HT400to600_skim"                    ] = 4640594  / (  55.61  * 1.23 );
    LumisXS[ "WJetsToLNu_HT600toInf_skim"                    ] = 237484   / (  18.81  * 1.23 );

    //Diboson
    LumisXS[ "WZJetsTo3LNu_skim"                             ] =   237484 / (2.29            );
    LumisXS[ "ZZTo4L_skim"                                   ] =  1958600 / (0.3231          );  // cross-section 31.8*(3*0.0336)*(3*0.0336)
    


    //via XSect
  
    map<string,float> KFactors;
    // if( md.isInitStatus() )
    md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

    //===============================================================
    // SDYJetsM50_HT600toInf_PU_S14_POSTLS170_skimamples **************************  samples
    //if( md.isInitStatus() ) {
    
    md.anConf.addSample( "WZJetsTo3LNu_skim"                     ,  "WZ+ZZ"              , kGreen    );
    md.anConf.addSample( "ZZTo4L_skim"                           ,  "WZ+ZZ"              , kGreen    );

    md.anConf.addSample( "TTZJets_skim"                          ,  "t#bar{t}Z"          , kBlue     );

    md.anConf.addSample( "TTWJets_skim"                          ,  "t#bar{t}W"          , kYellow   );
  
    md.anConf.addSample( "GGHZZ4L_skim"                          ,  "rare SM"            , kCyan     );
  
    //Drell-Yan
    //md.anConf.addSample( "DYJetsToLL_M-50_13TeV-madgraph-pythia8"                       ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "DYJetsToLL_M50_HT100to200_skim"        ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "DYJetsToLL_M50_HT200to400_skim"        ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "DYJetsToLL_M50_HT400to600_skim"        ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "DYJetsToLL_M50_HT600toInf_skim"        ,  "non-prompt e/#mu"    , kRed      );
  
    //t production
    md.anConf.addSample( "TBarToLeptons_sch_skim"                ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TBarToLeptons_tch_skim"                ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TBar_tWch_skim"                        ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TTH_skim"                            ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TTJets_skim"                           ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TToLeptons_sch_skim"                   ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TToLeptons_tch_skim"                   ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "T_tWch_skim"                           ,  "non-prompt e/#mu"    , kRed      );
  
    //W+Jets
    md.anConf.addSample( "WJetsToLNu_HT100to200_skim"            ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "WJetsToLNu_HT200to400_skim"            ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "WJetsToLNu_HT400to600_skim"            ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "WJetsToLNu_HT600toInf_skim"            ,  "non-prompt e/#mu"    , kRed      );
  
    //signal
    md.anConf.addSample( "SMS_T1tttt_2J_mGl1200_mLSP800_skim"    ,  "T1tttt (800) * 20 sig" , kViolet-3 );
    //md.anConf.addSample( "SMS_T1tttt_2J_mGl1500_mLSP100_skim"  ,  "T1tttt (100) #cdot 20 sig" , kViolet+3  );
    md.anConf.addSample( "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280_skim"  ,  "T5tttt (1000) * 20 sig" , kOrange-3 );
    //md.anConf.addSample( "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_skim"  ,  "T5tttt (1300) #cdot 20 sig" , kOrange+6 );
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
    //md.doStatisticsPlot();
    md.savePlot("SUSY3L");
    // md.dp.addText(xt,yt,st,addText);

}
