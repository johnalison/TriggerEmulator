#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulator.h"


using namespace TriggerEmulator;

TrigEmulator::TrigEmulator(std::string name, std::vector<HLTJetEmulator* > thresholds, std::vector<unsigned int> mult, int nToys){
  m_trigName = name;
  m_jetThresholds = thresholds;
  m_jetMultiplicities = mult;
  m_nToys = nToys;
}

void TrigEmulator::Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets){
	
//  //
//  // Require seed to be fired
//  //
//  if(!event->HLT_PFJet_Results[trigSeed])
//    return;


  for(unsigned int iToy = 0; iToy < m_nToys; ++iToy){

    // 
    // Count all events
    //
    ++m_nTotal;
    bool passTrig = true;

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

