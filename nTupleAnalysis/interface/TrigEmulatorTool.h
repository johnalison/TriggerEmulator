// -*- C++ -*-
#if !defined(TrigEmulatorTool_H)
#define TrigEmulatorTool_H

#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTHtEmulator.h"
#include <string>
#include "nTupleAnalysis/baseClasses/interface/jetData.h"



namespace TriggerEmulator {

  //
  // Trigger Emulation
  //
  class TrigEmulatorTool{

  public:

    TrigEmulatorTool(std::string name, int mode = 1, int nToys=1000, bool debug = false);

    void AddTrig(std::string trigName,  const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult);

    void AddTrig(std::string trigName,  std::string HTName, const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult);

    //
    //  Sets decisions for all configured triggers (involves random numbers if mode == 1)
    //
    void SetDecisions(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht=-1);

    //
    //  Return the value set in SetDecisions.  (So must call SetDecisions before GetDecision/Passed
    //
    bool GetDecision (std::string trigName);
    bool Passed      (std::string trigName) { return GetDecision(trigName); }

    //
    //  Sets weights for all configured triggers, which is the average nPass over nToys (involves random numbers if mode == 1)
    //
    void SetWeights(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht=-1);

    //
    //  Return the value set in SetWeights.  (So must call SetWeights before GetWeight)
    //
    float GetWeight (std::string trigName);

    // 
    //  For doing global run counting (Eg: in rate prediction)
    //
    void Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht=-1);

    void dumpResults();

  private:

    std::map<std::string, HLTHtEmulator*>  m_PFHt;
    std::map<std::string, HLTJetEmulator*> m_PFJet;
    std::map<std::string, TrigEmulator*>   m_emulatedTrigMenu;
    std::map<std::string, bool>            m_emulatedDecisions;
    std::map<std::string, float>           m_emulatedWeights;

    std::string m_name;
    int m_nToys;
    
    int m_mode; // 0 - flat cut / 1 - use turn on
    bool m_debug;


    //
    // PF Jet Config
    //
    std::vector<std::string> m_PFJet_trigNames = {"15", "30", "40",  "45",   "50",  "60",   "75",   "80",   "90",   "100",   "105",  "110",   "120",  "140",  "200",  "260",  "320",   "400", "450",  "500","550"};
    std::vector<float> m_PFJet_trigThresholds  = { 15,   30,   40. ,  45,     50,    60.,    75.,    80.,    90.,    100.,    105.,   110.,    120.,   140.,   200.,   260.,   320.,    400.,   450.,   500.,  550.};
    // From Fits to Data 2018
    std::vector<float> m_PFJet_p0              = {0.04, 0.04, 0.188,  0.211, 0.204,  0.188,  0.172,  0.169,  0.168,  0.158 ,  0.159,  0.157,   0.151,  0.144,  0.116,  0.149,  0.082,  0.100,   0.086,  0.085,  0.087};
    std::vector<float> m_PFJet_p1              = {-310, -310, 42.5,   50.0,  56.0,   67.3,   83.8,   89.2,   100.0,  110.9,   116.3,  121.6,   132.0,  153.2,  215.2,  277.1,  336.5,  411.5,  462.3,  505.5,  559.1};


    //
    // PF Ht Config
    //
    std::vector<std::string> m_PFHt_trigNames = {"none", "180",   "250",    "370",    "430",   "510",   "590",    "680",   "780",    "890",   "1050"};
    std::vector<float> m_PFHt_trigThresholds  = {0,    180. ,    250.     ,370.,     430.,    510.,    590.,     680.,    780.,     890.,    1050.};
    std::vector<float> m_PFHt_p0              = {10,  0.0319,  0.0343,  0.0334,  0.0347,  0.0334,   0.0313,   0.0294,  0.0283,  0.0268,     0.0273};
    std::vector<float> m_PFHt_p1              = {-300, 213.0,    294.1 ,  422.1 ,   483.4 ,   566.9 ,  650.0 ,    743.7 , 847.4 ,  956.5 ,    1113.};

  };


}

#endif // TrigEmulator_H

