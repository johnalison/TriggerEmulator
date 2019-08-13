// -*- C++ -*-
#if !defined(TrigEmulator_H)
#define TrigEmulator_H

#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTHtEmulator.h"
#include <string>
#include "nTupleAnalysis/baseClasses/interface/jetData.h"

namespace TriggerEmulator {

  //
  // Trigger Emulation
  //
  class TrigEmulator{

  public:

    TrigEmulator(std::string name, HLTHtEmulator* htThreshold, std::vector<HLTJetEmulator* > jetThresholds, std::vector<unsigned int> jetMultiplicities, int nToys=1000);

    void Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht=-1);

    void dumpResults();

  private:
    unsigned int m_nToys;

    std::string m_trigName;
    std::string m_trigSeed;
    HLTHtEmulator* m_htThreshold;
    std::vector<HLTJetEmulator* > m_jetThresholds;
    std::vector<unsigned int>     m_jetMultiplicities;

    // counters
    unsigned int m_nTotal = 0;
    unsigned int m_nPass  = 0;

  };

}

#endif // TrigEmulator_H
