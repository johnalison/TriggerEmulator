// -*- C++ -*-
#if !defined(TrigEmulatorTool_H)
#define TrigEmulatorTool_H

#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTBTagEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTHtEmulator.h"
#include <string>
#include <map>

class TRandom3;

namespace TriggerEmulator {

  enum class hTTurnOn { 
    L1ORAll_4j_3b,   
    L1ORAll_Ht300_4j_3b, 
    L1ORAll_Ht330_4j_3b,

    L1ORAll_2b100,   
    L1ORAll_2b116,      

    L1ORAll_2j_2j_3b,

    CaloHt300,           
    CaloHt320,          

    PFHt300,             
    PFHt330,            
  };

  enum class jetTurnOn { 
    L1100TandP,         
    L1100TandPDenMatch, 
    L1112,              
    L1112TandP,         
    L1112TandPDenMatch, 
    
    Calo30,             
    Calo45,             
    Calo100,            
    Calo90DenMatch,     
    Calo100DenMatch,    

    
    PF30,               
    PF40,               
    PF45,               
    PF60,               
    PF75,               
    PF116,              
    PF116Dr,            

    PF30DenMatch,       
    PF40DenMatch,       
    PF45DenMatch,       
    PF60DenMatch,       
    PF75DenMatch,       
    PF90DenMatch,       
    PF100DenMatch,      
    PF100DrDenMatch,    
    PF116DenMatch,      
    PF116DrDenMatch,    

    Calo45BTag,             
    Calo30BTag,             
    PF30BTag,
    PF40BTag,
    PF45BTag,
    PF60BTag,
    PF75BTag,
    L1112BTag,   
    Calo100BTag, 
    PF116BTag,   
    L1100BTag,   
    PF100BTag,   
    PF90BTag,   
    Calo90BTag,             

  };

  enum class bTagTurnOn { 
    CaloCSVMatchBtag,             
    CaloCSVMatchBtagDenMatch,     
    CaloCSV0p84MatchBtag,         
    CaloCSV0p84MatchBtagDenMatch, 

    PFDeepCSVMatchBtag,              
    CaloDeepCSV0p7MatchBtag,         
    PFDeepCSVMatchBtagDenMatch,      
    CaloDeepCSV0p7MatchBtagDenMatch, 

    PFDeepCSV,
    CaloDeepCSV,
    CaloDeepCSV2b116,
    PFCSV,
    CaloCSV,
      CaloCSV2b100,

      Calo100BTag,
  };

  //
  // Trigger Emulation
  //
  class TrigEmulatorTool{

  public:

    TrigEmulatorTool(std::string name, int mode = 1, unsigned int nToys=1000, std::string year = "2018", bool debug = false);

    // Just HT 
    void AddTrig(std::string trigName,  const std::vector<hTTurnOn>& HTNames);

    // jets 
    void AddTrig(std::string trigName,  const std::vector<jetTurnOn>&  JetNames, const std::vector<unsigned int>& JetMults);

    // HT and jets
    void AddTrig(std::string trigName,  const std::vector<hTTurnOn>& HTNames, const std::vector<jetTurnOn>&  JetNames, const std::vector<unsigned int>& JetMults);

    // jets and btags
    void AddTrig(std::string trigName,  const std::vector<jetTurnOn>&  JetNames, const std::vector<unsigned int>& JetMults, const std::vector<bTagTurnOn>& TagNames, const std::vector<unsigned int>& TagMults);

    // HT, jets and btags
    void AddTrig(std::string trigName,  const std::vector<hTTurnOn>& HTNames, const std::vector<jetTurnOn>&  JetNames, const std::vector<unsigned int>& JetMults, const std::vector<bTagTurnOn>& TagNames, const std::vector<unsigned int>& TagMults);

    //
    //  Sets decisions for all configured triggers (involves random numbers if mode == 1)
    //
    void SetDecisions(const std::vector<float>& offline_jet_pts, const std::vector<float>& offline_btagged_jet_pts, float ht=-1);

    //
    //  Return the value set in SetDecisions.  (So must call SetDecisions before GetDecision/Passed
    //
    bool GetDecision (std::string trigName);
    bool Passed      (std::string trigName) { return GetDecision(trigName); }

    //
    //  Sets weights for all configured triggers, which is the average nPass over nToys (involves random numbers if mode == 1)
    //
    void SetWeights(const std::vector<float>& offline_jet_pts, const std::vector<float>& offline_btagged_jet_pts, float ht=-1);
    //void SetWeightsOR(const std::vector<float>& offline_jet_pts, const std::vector<float>& offline_btagged_jet_pts, float ht=-1);

    //
    //  Return the value set in SetWeights.  (So must call SetWeights before GetWeight)
    //
    float GetWeight (std::string trigName);


    //
    //  Calculate the weight of the OR of the menu defined
    //
    float  GetWeightOR(const std::vector<float>& offline_jet_pts, const std::vector<float>& offline_btagged_jet_pts, float ht=-1, bool setSeed=false);

    // 
    //  For doing global run counting (Eg: in rate prediction)
    //
    void Fill(const std::vector<float>& offline_jet_pts, const std::vector<float>& offline_btagged_jet_pts, float ht=-1);

    void dumpResults();

  private:

    std::map<hTTurnOn,   HLTHtEmulator*>    m_Ht;
    std::map<jetTurnOn,  HLTJetEmulator*>   m_Jet;
    std::map<bTagTurnOn, HLTBTagEmulator*>  m_BTag;
    std::map<std::string, TrigEmulator*>     m_emulatedTrigMenu;
    std::map<std::string, bool>              m_emulatedDecisions;
    std::map<std::string, float>             m_emulatedWeights;

    std::string m_name;
    unsigned int m_nToys;
    
    bool m_debug;
    TRandom3*           m_rand        = nullptr;

    std::map<hTTurnOn,   std::array<std::string, 2> >  HTConfig;
    std::map<jetTurnOn,  std::array<std::string, 2> >  JetConfig;
    std::map<bTagTurnOn, std::array<std::string, 2> >  BTagConfig;

    void config2018();
    void config2017();
    void config2016();

    std::vector<std::vector<double> >  getRandWeights(const std::vector<float>& input, bool setSeed, int seedOffset);

  };


}

#endif // TrigEmulator_H

