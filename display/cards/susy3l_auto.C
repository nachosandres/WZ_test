
{


    if(Recompute) {
        MPAFDisplay md;
 
        bool * rtmp= const_cast<bool*> pr;
        *rtmp = false;

    }
    else 
    md.refresh();





    string signal = "T1t412";
    //string signal = "T1t415";


    //general parameters ********************* general parameters
    string dir="SUSY3L";    // directory in workdir/stats
    string fileName="susy3l_SIGNALREGION_PAIRSTATE";
    string fileList="susy3l_SIGNALREGION_PAIRSTATE"; 
    string hName="";

    bool mcOnly = false;
  
    md.anConf.configureNames( dir, fileName, fileList);
    md.anConf.configureData(false, 0, mcOnly);
 
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

    md.anConf.loadXSDB("XSectionsPhys14.db");

    map<string,float> LumisXS;
 
    //via XSect
    map<string,float> KFactors;
    
    md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

    //===============================================================
    
    md.anConf.addSample( "WZJetsTo3LNu"                     ,  "diboson"            , kGreen    );
    md.anConf.addSample( "ZZTo4L"                           ,  "diboson"            , kGreen    );

    md.anConf.addSample( "TTZJets"                          ,  "ttZ"                , kBlue     );

    md.anConf.addSample( "TTWJets"                          ,  "ttW"                , kYellow   );
    
    md.anConf.addSample( "TTH"                              ,  "rare"               , kCyan     );
    //md.anConf.addSample( "GGHZZ4L"                          ,  "rare"               , kCyan     );
    
    //Drell-Yan
    md.anConf.addSample( "DYJetsToLL_M50_HT100to200"        ,  "fake"   , kRed      );
    md.anConf.addSample( "DYJetsToLL_M50_HT200to400"        ,  "fake"    , kRed      );
    md.anConf.addSample( "DYJetsToLL_M50_HT400to600"        ,  "fake"    , kRed      );
    md.anConf.addSample( "DYJetsToLL_M50_HT600toInf"        ,  "fake"    , kRed      );
    
    //t production
    md.anConf.addSample( "TBarToLeptons_sch"                ,  "fake"    , kRed      );
    md.anConf.addSample( "TBarToLeptons_tch"                ,  "fake"    , kRed      );
    md.anConf.addSample( "TBar_tWch"                        ,  "fake"    , kRed      );
    md.anConf.addSample( "TTJets"                           ,  "fake"    , kRed      );
    md.anConf.addSample( "TToLeptons_sch"                   ,  "fake"    , kRed      );
    md.anConf.addSample( "TToLeptons_tch"                   ,  "fake"    , kRed      );
    md.anConf.addSample( "T_tWch"                           ,  "fake"    , kRed      );
    
    //W+Jets
    md.anConf.addSample( "WJetsToLNu_HT100to200"            ,  "fake"    , kRed      );
    md.anConf.addSample( "WJetsToLNu_HT200to400"            ,  "fake"    , kRed      );
    //md.anConf.addSample( "WJetsToLNu_HT400to600"            ,  "fake"    , kRed      );
    md.anConf.addSample( "WJetsToLNu_HT600toInf"            ,  "fake"    , kRed      );
    
    //signal
    if(signal=="T1t412"){
        md.anConf.addSample( "SMS_T1tttt_2J_mGl1200_mLSP800"    ,  "T1t412" , kViolet-3 );
    }
    if(signal=="T1t415"){
        md.anConf.addSample( "SMS_T1tttt_2J_mGl1500_mLSP100"    ,  "T1t415" , kOrange+3  );
    }
   
   
    md.addNuisanceParameter("fratio","fake","lnN","1.50");
    md.addNuisanceParameter("rNorm1","diboson","lnN","1.20");
    md.addNuisanceParameter("rNorm2","ttZ","lnN","1.20");
    md.addNuisanceParameter("rNorm3","ttW","lnN","1.20");
    md.addNuisanceParameter("rNorm4","rare","lnN","1.20");
    md.addNuisanceParameter("sigNorm",signal,"lnN","1.10");

   
    
    //===============================================================

    //plotting ================
    md.dp.setLumiAndEnergy( lumi, energy );
    md.dp.setNormalization( Norm );
    md.prepareDisplay();
    md.doPlot();

    md.makeSingleDataCard(signal, "global", "SR MET selection", fileList+"_"+signal);


    //md.doStatisticsPlot();
    //md.savePlot("SUSY3L");
    // md.dp.addText(xt,yt,st,addText);
    
    gROOT->ProcessLine(".q");
}
