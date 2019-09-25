#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulator.h"
#include <iostream>

using namespace TriggerEmulator;
using std::cout; using std::endl;
TrigEmulator::TrigEmulator(std::string name, 
			   HLTHtEmulator* htThreshold, 
			   std::vector<HLTJetEmulator* > jetThresholds, std::vector<unsigned int> jetMultiplicities, 
			   HLTBTagEmulator* bTagOpPoint, unsigned int bTagMultiplicities,
			   int nToys){
  m_trigName = name;
  m_htThreshold = htThreshold;
  m_jetThresholds = jetThresholds;
  m_jetMultiplicities = jetMultiplicities;
  m_bTagOpPoint = bTagOpPoint;
  m_bTagMultiplicities = bTagMultiplicities;
  m_nToys = nToys;
}

bool TrigEmulator::passTrig(std::vector<float> offline_jet_pts, std::vector<float> offline_btagged_jet_pts, float ht, float seedOffset){
  //std::cout << "TrigEmulator:: " << m_trigName << " ht " << ht << std::endl;

  //
  //  Ht Cut 
  //
  if(ht > 0 && m_htThreshold){
    if(!m_htThreshold->passHt(ht, seedOffset)) {
      //std::cout << "Fail ht  " << std::endl;
      return false;
    }
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
    for(float jet_pt: offline_jet_pts){
      if(HLTJet->passJet(jet_pt,seedOffset)) ++nJetsPassed;
    }
      
    //
    //  Impose trigger cut
    //
    if(nJetsPassed < m_jetMultiplicities.at(iThres)){
      return false;
    }
  }

  //
  //  Apply BTag Operating Points
  //
  unsigned int nJetsPassBTag = 0;
      
  //
  // Count passing jets
  //
  for(float bjet_pt: offline_btagged_jet_pts){
    if(m_bTagOpPoint->passJet(bjet_pt,seedOffset)) ++nJetsPassBTag;
  }
      
  //
  //  Impose trigger cut
  //
  if(nJetsPassBTag < m_bTagMultiplicities){
    return false;
  }

  return true;
}

float TrigEmulator::calcWeight(std::vector<float> offline_jet_pts, std::vector<float> offline_btagged_jet_pts, float ht){

  unsigned int nPass  = 0;
  
  for(unsigned int iToy = 0; iToy < m_nToys; ++iToy){

    // 
    // Count all events
    //
    if(passTrig(offline_jet_pts, offline_btagged_jet_pts, ht, iToy))
      ++nPass;
  }
  //cout << "TrigEmulator::calcWeight is " << float(nPass)/m_nToys << endl;
  return float(nPass)/m_nToys;
}



void TrigEmulator::Fill(std::vector<float> offline_jet_pts, std::vector<float> offline_btagged_jet_pts, float ht){

  for(unsigned int iToy = 0; iToy < m_nToys; ++iToy){

    // 
    // Count all events
    //
    ++m_nTotal;
    if(passTrig(offline_jet_pts, offline_btagged_jet_pts, ht, iToy))
      ++m_nPass;
  }

  return;
}

void TrigEmulator::dumpResults(){
  std::cout << "eff_rel = " << float(m_nPass)/m_nTotal << "    (" << m_nPass << ")"<< std::endl;
}

