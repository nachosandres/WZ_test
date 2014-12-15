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

    float HT();


private:

    //counter categories, 0 is ALWAYS global (even if not specified later)
    enum {kGlobal=0, kElId, kElVeto, kMuId, kMuVeto, kJetId, kBJetId};

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

    float _HT;
};

#endif
