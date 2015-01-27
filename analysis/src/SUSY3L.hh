/*****************************************************************************
******************************************************************************
******************************************************************************
**                                                                          **
** The Multi-Purpose Analysis Framework                                     **
**                                                                          **
** Jan Hoss, ETHZ, 12/2014                                  **
**                                                                          **
******************************************************************************
******************************************************************************
*****************************************************************************/

#ifndef SUSY3L_HH
#define SUSY3L_HH

#include "analysis/core/MPAF.hh"

class SUSY3L: public MPAF {

public:
    // Member Functions
    SUSY3L(std::string);
    virtual ~SUSY3L();

private:
    void initialize();

    void run();
    void defineOutput();
    void loadInput();
    void modifyWeight();
    void writeOutput();

    void modifySkimming();

    void collectKinematicObjects();
    bool electronSelection(int);
    bool muonSelection(int);
    bool vetoElectronSelection(int);
    bool vetoMuonSelection(int);
    bool bJetSelection(int);
    bool goodJetSelection(int);

    bool baseSelection();
    void setBaselineRegion();
    void setCut(std::string, float, std::string, float = 0);
    bool hardLegSelection();
    bool ZEventSelection();
    bool ZEventSelectionLoop();



    float HT();

    string _pairmass;
    string _BR;

private:

    //counter categories, 0 is ALWAYS global (even if not specified later)
    enum {kGlobal=0, kElId, kElVeto, kMuId, kMuVeto, kJetId, kBJetId, konZEvents};

    //cut variables
    float _valCutLepMultiplicityBR;
    float _pt_cut_hard_leg;
    float _valCutNJetsBR;
    float _valCutNBJetsBR;
    float _ZMassWindow;
    float _lowMllCut;
    float _valCutHTBR;
    float _valCutMETBR;
    
    std::string _cTypeLepMultiplicityBR;
    std::string _cTypeNJetsBR;
    std::string _cTypeNBJetsBR;
    std::string _cTypeHTBR;
    std::string _cTypeMETBR;

    float _upValCutLepMultiplicityBR;
    float _upValCutNJetsBR;
    float _upValCutNBJetsBR;
    float _upValCutHTBR;
    float _upValCutMETBR;

    //vectors for electron and muon candidates
    std::vector<int> _elIdx;
    std::vector<int> _muIdx;

    //length of candiate vectors
    int _nEls;
    int _nVEls;
    int _nMus;
    int _nVMus;
    int _nJets;
    int _nBJets;

    //list of object candidates
    CandList _els;
    CandList _vEls;
    CandList _mus;
    CandList _vMus;
    CandList _jets;
    CandList _bJets;
    Candidate* _met;
    Candidate* _Z;

    float _HT;
    
};

#endif
