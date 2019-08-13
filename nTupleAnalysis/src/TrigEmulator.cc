#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulator.h"


using namespace TriggerEmulator;

TrigEmulator::TrigEmulator(std::string name, HLTHtEmulator* htThreshold, std::vector<HLTJetEmulator* > jetThresholds, std::vector<unsigned int> jetMultiplicities, int nToys){
  m_trigName = name;
  m_htThreshold = htThreshold;
  m_jetThresholds = jetThresholds;
  m_jetMultiplicities = jetMultiplicities;
  m_nToys = nToys;
}

void TrigEmulator::Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht){
	

  for(unsigned int iToy = 0; iToy < m_nToys; ++iToy){

    // 
    // Count all events
    //
    ++m_nTotal;
    bool passTrig = true;


    //
    //  Ht Cut 
    //
    if(ht > 0 && m_htThreshold){
      if(!m_htThreshold->passHt(ht)) passTrig = false;
    }


    //
    // Loop on all thresholds
    //
    for(unsigned int iThres = 0; iThres < m_jetThresholds.size(); ++iThres){
      HLTJetEmulator* HLTJet = m_jetThresholds.at(iThres);
          
      unsigned int nJetsPassed = 0;
      
      //
      // Count passing jets
      //
      for(const nTupleAnalysis::jetPtr &jet: offline_jets){
	if(HLTJet->passJet(jet->pt)) ++nJetsPassed;
      }
      
      //
      //  Impose trigger cut
      //
      if(nJetsPassed < m_jetMultiplicities.at(iThres))
	passTrig = false;
          
    }
        
    if(passTrig)
      ++m_nPass;
  }

  return;
}

void TrigEmulator::dumpResults(){
  std::cout << "eff_rel = " << float(m_nPass)/m_nTotal << "    (" << m_nPass << ")"<< std::endl;
}

