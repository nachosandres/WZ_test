MPAFDisplay md;

void dataCardProd_CATTAG_SIGTAG_LUMTAG() {
  md.refresh();


  //general parameters ********************* general parameters
  string dir="SSDL2015";
  string fileName="fakeEstim_SR"; // not needed for statistics but provides the normalization
  string fileList="fakeEstim_SR"; // put command line that gives all files as in a "ls" command

  bool mcOnly = false;
  
  md.anConf.configureNames( dir, fileName, fileList );
  md.anConf.configureData(false, 0, mcOnly);
  
  
  //Lumis( or XSections ) pb-1 & KFactors ************************************
  float lumi=LUMTAG; //pb-1 19470
  float energy=13; //TeV

  bool useXS=false;

  md.anConf.loadXSDB("XSectionsPhys14.db");

  map<string,float> LumisXS;

  //via XSect
  map<string,float> KFactors;
  

  md.anConf.configureLumi( LumisXS, KFactors, lumi, useXS );

  //===============================================================

  md.addDataCardSigSample("CATTAG:SIGNAME","SIGTAG");
  
  md.addDataCardSample("CATTAG:WZJetsTo3LNu", "wz");
  md.addDataCardSample("CATTAG:TTH", "tth");
  md.addDataCardSample("CATTAG:TTZJets", "ttz");
  md.addDataCardSample("CATTAG:TTWJets", "ttw");
  
  md.addDataCardSample("CATTAG:TTJetscharge", "chMId");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT100to200charge", "chMId");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT200to400charge", "chMId");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT400to600charge", "chMId");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT600toInfcharge", "chMId");

  md.addDataCardSample("CATTAG_Fake:TTJets", "fake");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT100to200", "fake");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT200to400", "fake");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT400to600", "fake");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT600toInf", "fake");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT100to200", "fake");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT200to400", "fake");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT400to600", "fake");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT600toInf", "fake");

  // md.addDataCardSample("CATTAG:TTJetsfake", "fake");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT100to200fake", "fake");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT200to400fake", "fake");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT400to600fake", "fake");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT600toInffake", "fake");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT100to200fake", "fake");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT200to400fake", "fake");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT400to600fake", "fake");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT600toInffake", "fake");
  
  md.addDataCardSample("CATTAG:WZJetsTo3LNu", "pseudodata");
  md.addDataCardSample("CATTAG:TTH", "pseudodata");
  md.addDataCardSample("CATTAG:TTZJets", "pseudodata");
  md.addDataCardSample("CATTAG:TTWJets", "pseudodata");
  md.addDataCardSample("CATTAG:TTJetscharge", "pseudodata");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT100to200charge", "pseudodata");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT200to400charge", "pseudodata");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT400to600charge", "pseudodata");
  md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT600toInfcharge", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:TTJets", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT100to200", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT200to400", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT400to600", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:WJetsToLNu_HT600toInf", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT100to200", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT200to400", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT400to600", "pseudodata");
  md.addDataCardSample("CATTAG_Fake:DYJetsToLL_M50_HT600toInf", "pseudodata");
  // md.addDataCardSample("CATTAG:TTJetsfake", "pseudodata");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT100to200fake", "pseudodata");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT200to400fake", "pseudodata");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT400to600fake", "pseudodata");
  // md.addDataCardSample("CATTAG:WJetsToLNu_HT600toInffake", "pseudodata");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT100to200fake", "pseudodata");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT200to400fake", "pseudodata");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT400to600fake", "pseudodata");
  // md.addDataCardSample("CATTAG:DYJetsToLL_M50_HT600toInffake", "pseudodata");


  //experimental uncertainties
  md.addNuisanceParameter("jes","ttw:ttz:SIGTAG","lnN","JESTAG");

  //extrapolation uncertainties
  md.addNuisanceParameter("ttwTail","ttw","lnN","TTWTAILTAG");
  md.addNuisanceParameter("ttzTail","ttz","lnN","TTZTAILTAG");
  md.addNuisanceParameter("wzB","wz","lnN","WZBTAG");

  //flat uncertainties -> cross sections
  // md.addNuisanceParameter("ttwNorm","ttw","lnN","1.1"); //->merged with the "tail" source
  // md.addNuisanceParameter("ttwNorm","ttz","lnN","1.1");
  md.addNuisanceParameter("wzNorm","wz","lnN","WZLUMITAG");
  
  //Data-driven methods
  md.addNuisanceParameter("chMID","chMId","lnN","1.50");
  md.addNuisanceParameter("fratio","fake","lnN","1.40");

  //syst from stat uncertainties == automatic
  //md.addNuisanceParameter("wzstat","wz","lnN",""); //--> already given by the 8/15% on Xsect
  // md.addNuisanceParameter("tthstat","tth","lnN","");
  // md.addNuisanceParameter("ttwstat","ttw","lnN","");
  // md.addNuisanceParameter("ttzstat","ttz","lnN","");
  // md.addNuisanceParameter("fakestat","fake","lnN","");
  // md.addNuisanceParameter("chMIdstat","chMId","lnN","");
  // md.addNuisanceParameter("SIGTAGstat","SIGTAG","lnN","");

  
  string srs[66]={ 
    "SR1A", "SR2A", "SR3A", "SR4A", "SR5A", "SR6A", "SR7A", "SR8A",
    "SR9A", "SR10A", "SR11A", "SR12A", "SR13A", "SR14A", "SR15A", "SR16A",
    "SR17A", "SR18A", "SR19A", "SR20A", "SR21A", "SR22A", "SR23A", "SR24A",
    "SR25A", "SR26A", "SR27A", "SR28A", "SR29A", "SR30A", "SR31A", "SR32A",
    
    "SR1B", "SR2B", "SR3B", "SR4B", "SR5B", "SR6B", "SR7B", "SR8B",
    "SR9B", "SR10B", "SR11B", "SR12B", "SR13B", "SR14B", "SR15B", "SR16B",
    "SR17B", "SR18B", "SR19B", "SR20B", "SR21B", "SR22B", "SR23B", "SR24B",
    "SR25B","SR26B",
    
    "SR1C", "SR2C", "SR3C", "SR4C", "SR5C", "SR6C", "SR7C", "SR8C"
  };



  for(int isr=0;isr<66;isr++) {
    if(srs[isr]=="CATTAG") {
      md.addNuisanceParameter("tth"+srs[isr]+"stat","tth","lnN","");
      md.addNuisanceParameter("ttw"+srs[isr]+"stat","ttw","lnN","");
      md.addNuisanceParameter("ttz"+srs[isr]+"stat","ttz","lnN","");
      md.addNuisanceParameter("fake"+srs[isr]+"stat","fake","lnN","");
      md.addNuisanceParameter("chMId"+srs[isr]+"stat","chMId","lnN","");
      md.addNuisanceParameter("SIGTAG"+srs[isr]+"stat","SIGTAG","lnN","");
    }
    else {
      md.addNuisanceParameter("tth"+srs[isr]+"stat","tth","lnN","-");
      md.addNuisanceParameter("ttw"+srs[isr]+"stat","ttw","lnN","-");
      md.addNuisanceParameter("ttz"+srs[isr]+"stat","ttz","lnN","-");
      md.addNuisanceParameter("fake"+srs[isr]+"stat","fake","lnN","-");
      md.addNuisanceParameter("chMId"+srs[isr]+"stat","chMId","lnN","-");
      md.addNuisanceParameter("SIGTAG"+srs[isr]+"stat","SIGTAG","lnN","-");
    }
  }//signal regions  


  // md.addNuisanceParameter("SIGTAGStat","SIGTAG","lnN","");
  // md.addNuisanceParameter("ttwStat","ttw","lnN","");
  // md.addNuisanceParameter("ttzStat","ttz","lnN","");
  // md.addNuisanceParameter("wzStat","wz","lnN","");

  //md.addNuisanceParameter("sigNorm","T1t415","lnN","1.10");


  // }
  //===============================================================

  //*********************************************************************²
  //Execution macro ******************************************************
  
  //plotting ================
  md.dp.setLumiAndEnergy( lumi, energy );
   
  md.prepareDisplay();
  
 //  int nCateg=66; //47
 //  vector<string> _categs(nCateg,"");
 
 // string srs[66]={ 

 //    "SR1A", "SR2A", "SR3A", "SR4A", "SR5A", "SR6A", "SR7A", "SR8A",
 //    "SR9A", "SR10A", "SR11A", "SR12A", "SR13A", "SR14A", "SR15A", "SR16A",
 //    "SR17A", "SR18A", "SR19A", "SR20A", "SR21A", "SR22A", "SR23A", "SR24A",
 //    "SR25A", "SR26A", "SR27A", "SR28A", "SR29A", "SR30A", "SR31A", "SR32A",
    
 //    "SR1B", "SR2B", "SR3B", "SR4B", "SR5B", "SR6B", "SR7B", "SR8B",
 //    "SR9B", "SR10B", "SR11B", "SR12B", "SR13B", "SR14B", "SR15B", "SR16B",
 //    "SR17B", "SR18B", "SR19B", "SR20B", "SR21B", "SR22B", "SR23B", "SR24B",
 //    "SR25B","SR26B",
    
 //    "SR1C", "SR2C", "SR3C", "SR4C", "SR5C", "SR6C", "SR7C", "SR8C"
 // }

  // _categs.assign(srs, srs+nCateg);
  // for(size_t ic=0;ic< _categs.size();ic++) {
  //   string cat=_categs[ic];
    md.makeSingleDataCard("SIGTAG", "nominal", "selected", "ssdl2015_CATTAG_SIGTAG_LUMTAG");
  // }

  gROOT->ProcessLine(".q");

}
