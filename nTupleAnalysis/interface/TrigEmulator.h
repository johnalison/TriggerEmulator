// -*- C++ -*-
#if !defined(TrigEmulator_H)
#define TrigEmulator_H

#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTHtEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTBTagEmulator.h"
#include <string>
#include "nTupleAnalysis/baseClasses/interface/jetData.h"

namespace TriggerEmulator {

  //
  // Trigger Emulation
  //
  class TrigEmulator{

  public:

    TrigEmulator(std::string name, 
		 HLTHtEmulator* htThreshold, 
		 std::vector<HLTJetEmulator* > jetThresholds, std::vector<unsigned int> jetMultiplicities, 
		 HLTBTagEmulator* bTagOpPoint, unsigned int bTagMultiplicities,
		 int nToys=1000);

    //
    //  Determine if a given event fired (invloves random number if mode=1)
    //
    bool passTrig(std::vector<nTupleAnalysis::jetPtr> offline_jets, std::vector<nTupleAnalysis::jetPtr> offline_btagged_jets, float ht=-1, float seedOffset=1.0);

    //
    //  Calculate weight for trigger, average nPass over nToys (involves random numbers if mode == 1)
    //
    float calcWeight(std::vector<nTupleAnalysis::jetPtr> offline_jets, std::vector<nTupleAnalysis::jetPtr> offline_btagged_jets, float ht=-1);

    // 
    //  For doing global run counting (Eg: in rate prediction)
    //
    void Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets, std::vector<nTupleAnalysis::jetPtr> offline_btagged_jets, float ht=-1);

    void dumpResults();

  private:
    unsigned int m_nToys;

    std::string m_trigName;
    std::string m_trigSeed;
    HLTHtEmulator* m_htThreshold;
    std::vector<HLTJetEmulator* > m_jetThresholds;
    std::vector<unsigned int>     m_jetMultiplicities;
    HLTBTagEmulator* m_bTagOpPoint;
    unsigned int     m_bTagMultiplicities;

    // counters
    unsigned int m_nTotal = 0;
    unsigned int m_nPass  = 0;

  };

}

#endif // TrigEmulator_H
