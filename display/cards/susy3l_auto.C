{
    if(Recompute) {
        MPAFDisplay md;
 
        bool * rtmp= const_cast<bool*> pr;
        *rtmp = false;

    }
    else md.refresh();

    //general parameters ********************* general parameters
    string dir="SUSY3L";    // directory in workdir/stats
    string fileName="susy3l_SIGNALREGION_PAIRSTATE"; // file name in workdir/stats/dir
    string fileList="susy3l_SIGNALREGION_PAIRSTATE"; //CH: since AnaConfig needs a fileName to open, we need to put the data files into a different variable
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
    //md.dp.setObservables("BR_HT");
    //md.dp.setObservables("BR_MET");
    //md.dp.setObservables("SR_NJets");
    //md.dp.setObservables("SR_NBJets");
    //md.dp.setObservables("SR_HT");
    //md.dp.setObservables("SR_MET");


    //Binning & title ************************* Binning & titre
    string yTitle="number of events";
    int binning=1;
    int addBinBkg=1; //BinB = binning*AddBin
    double rangeY[2]={0,0};
    double rangeX[2]={0,7};
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
    LumisXS[ "DYJetsToLL_M-50_13TeV-madgraph-pythia8"                       ] = 2829164 / (6024             );
    LumisXS[ "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola"     ] = 4054159 / ( 194.3   * 1.27  );
    LumisXS[ "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola"     ] = 4666496 / (  52.24  * 1.27  );
    LumisXS[ "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola"     ] = 4931372 / (   6.546 * 1.27  );
    LumisXS[ "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola"     ] = 4493574 / (   2.179 * 1.27  );

    //Higgs->ZZ
    LumisXS[ "GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6"                 ] =  204684 / (0.01212192       );  // cross-section: 43.92*2.76E-04
    
    //signal
    LumisXS[ "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola" ] =  100322 / (0.0856418 * 1    );
    LumisXS[ "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola" ] =  105679 / (0.0141903 * 1    );
    LumisXS[ "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280"                    ] =   52499 / (0.3254    * 1    );
    LumisXS[ "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280"                    ] =       0 / (1         * 1    );
    
    LumisXS[ "TbarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"   ] =  250000 / (1.34784          );  // 4.16*0.108*3
    LumisXS[ "TbarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola"   ] = 1999800 / (26.23428         );  // 80.97*0.108*3
    LumisXS[ "Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola"          ] =  971800 / (35.6             ); 
    LumisXS[ "TtbarH_M-125_13TeV_amcatnlo-pythia8-tauola"                   ] =  199700 / (1         * 1    );
    LumisXS[ "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola"      ] =25446993 / (809.1            );
    LumisXS[ "TtoLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"      ] =  500000 / (2.3328           );  // 7.20*0.108*3
    LumisXS[ "TtoLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"      ] = 3991000 / (44.0802          );  // 136.05*0.108*3
    LumisXS[ "T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola"             ] =  986100 / (35.6             );  // 35.6
   
    //TTV 
    LumisXS[ "TTWJets_Tune4C_13TeV-madgraph-tauola"                         ] =  246521 / (0.6647           );
    LumisXS[ "TTZJets_Tune4C_13TeV-madgraph-tauola"                         ] =  249275 / (0.8565           );
      
    //LumisXS[ "WJetsToLNu_13TeV-madgraph-pythia8-tauola"                    ] = 10017462 / (20508.9        ); 
    LumisXS[ "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola"          ] = 5262265  / (1817.0   * 1.23 );
    LumisXS[ "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola"          ] = 4936077  / ( 471.6   * 1.23 );
    LumisXS[ "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola"          ] = 4640594  / (  55.61  * 1.23 );
    LumisXS[ "WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola"          ] = 237484   / (  18.81  * 1.23 );

    //Diboson
    LumisXS[ "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola"                    ] =   237484 / (2.29            );
    LumisXS[ "ZZTo4L_Tune4C_13TeV-powheg-pythia8"                           ] =  1958600 / (0.3231          );  // cross-section 31.8*(3*0.0336)*(3*0.0336)
    


    //via XSect
  
    map<string,float> KFactors;
    // if( md.isInitStatus() )
    md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

    //===============================================================
    // SDYJetsM50_HT600toInf_PU_S14_POSTLS170_skimamples **************************  samples
    //if( md.isInitStatus() ) {
    
    md.anConf.addSample( "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola"                    ,  "WZ+ZZ"              , kGreen    );
    md.anConf.addSample( "ZZTo4L_Tune4C_13TeV-powheg-pythia8"                           ,  "WZ+ZZ"              , kGreen    );

    md.anConf.addSample( "TTZJets_Tune4C_13TeV-madgraph-tauola"                         ,  "t#bar{t}Z"          , kBlue     );

    md.anConf.addSample( "TTWJets_Tune4C_13TeV-madgraph-tauola"                         ,  "t#bar{t}W"          , kYellow   );
    
    md.anConf.addSample( "GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6"                 ,  "rare SM"            , kCyan     );
    
    //Drell-Yan
    md.anConf.addSample( "DYJetsToLL_M-50_13TeV-madgraph-pythia8"                       ,  "non-prompt e/#mu"    , kRed      );
    //md.anConf.addSample( "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola"     ,  "non-prompt e/#mu"    , kRed      );
    //md.anConf.addSample( "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola"     ,  "non-prompt e/#mu"    , kRed      );
    //md.anConf.addSample( "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola"     ,  "non-prompt e/#mu"    , kRed      );
    //md.anConf.addSample( "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola"     ,  "non-prompt e/#mu"    , kRed      );
    
    //t production
    md.anConf.addSample( "TbarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"   ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TbarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola"   ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola"          ,  "non-prompt e/#mu"    , kRed      );
    //md.anConf.addSample( "TtbarH_M-125_13TeV_amcatnlo-pythia8-tauola"                   ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola"      ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TtoLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"      ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "TtoLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola"      ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola"             ,  "non-prompt e/#mu"    , kRed      );
    
    //W+Jets
    md.anConf.addSample( "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola"          ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola"          ,  "non-prompt e/#mu"    , kRed      );
    md.anConf.addSample( "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola"          ,  "non-prompt e/#mu"    , kRed      );
    
    //signal
    md.anConf.addSample( "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola" ,  "T1tttt (800) * 20 sig" , kViolet-3 );
    md.anConf.addSample( "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola" ,  "T1tttt (100) * 20 sig" , kViolet+3  );
    md.anConf.addSample( "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280"                    ,  "T5tttt (1000) * 20 sig" , kOrange-3 );
    md.anConf.addSample( "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280"                    ,  "T5tttt (1300) * 20 sig" , kOrange+6 );
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

    md.makeDataCard(fileList, "global", "SR MET selection");
    //md.doStatisticsPlot();
    //md.savePlot("SUSY3L");
    // md.dp.addText(xt,yt,st,addText);
    
    gROOT->ProcessLine(".q");
}
