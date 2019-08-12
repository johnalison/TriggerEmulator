// -*- C++ -*-
#if !defined(TrigEmulatorTool_H)
#define TrigEmulatorTool_H

#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"
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

    void Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets);

    void dumpResults();

  private:

    std::map<std::string, HLTJetEmulator*> m_PFJet;
    std::map<std::string, TrigEmulator*>   m_emulatedTrigMenu;


    std::string m_name;
    int m_nToys;
    
    int m_mode; // 0 - flat cut / 1 - use turn on
    bool m_debug;

    std::vector<std::string> m_PFJet_trigNames = {"15", "30", "40",  "45",   "50",  "60",   "75",   "80",   "90",   "100",   "105",  "110",   "120",  "140",  "200",  "260",  "320",   "400", "450",  "500","550"};
    std::vector<float> m_PFJet_trigThresholds  = { 15,   30,   40. ,  45,     50,    60.,    75.,    80.,    90.,    100.,    105.,   110.,    120.,   140.,   200.,   260.,   320.,    400.,   450.,   500.,  550.};
    // From Fits to Data 2018
    std::vector<float> m_PFJet_p0              = {0.04, 0.04, 0.188,  0.211, 0.204,  0.188,  0.172,  0.169,  0.168,  0.158 ,  0.159,  0.157,   0.151,  0.144,  0.116,  0.149,  0.082,  0.100,   0.086,  0.085,  0.087};
    std::vector<float> m_PFJet_p1              = {-310, -310, 42.5,   50.0,  56.0,   67.3,   83.8,   89.2,   100.0,  110.9,   116.3,  121.6,   132.0,  153.2,  215.2,  277.1,  336.5,  411.5,  462.3,  505.5,  559.1};



  };


}

#endif // TrigEmulator_H
