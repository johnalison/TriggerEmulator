#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulatorTool.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"


using namespace TriggerEmulator;

using std::cout;  using std::endl;

TrigEmulatorTool::TrigEmulatorTool(std::string name, int mode, int nToys, bool debug) :  m_name(name),  m_nToys(nToys), m_mode(mode), m_debug(debug)
{

  //
  //  Emulated Jet Thresholds Triggers
  // 
  for(unsigned int i = 0; i < m_PFJet_trigNames.size(); ++i){
    m_PFJet.insert(std::make_pair(m_PFJet_trigNames.at(i),new HLTJetEmulator("PFJ"+m_PFJet_trigNames.at(i),m_PFJet_p0.at(i), m_PFJet_p1.at(i), m_PFJet_trigThresholds.at(i),m_mode)));
  }

  for(unsigned int i = 0; i < m_PFHt_trigNames.size(); ++i){
    m_PFHt.insert(std::make_pair(m_PFHt_trigNames.at(i),new HLTHtEmulator("PFHt"+m_PFHt_trigNames.at(i),m_PFHt_p0.at(i), m_PFHt_p1.at(i), m_PFHt_trigThresholds.at(i),m_mode)));
  }

}


void TrigEmulatorTool::AddTrig(std::string trigName,  const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult){
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Enter " << trigName << endl;
  return AddTrig(trigName, "none", PFJetName, PFJetMult);
}



void TrigEmulatorTool::AddTrig(std::string trigName,  std::string HTName, const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult){
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Enter " << trigName << endl;
  if(m_emulatedTrigMenu.find(trigName) != m_emulatedTrigMenu.end()){
    cout << "TrigEmulatorTool::AddTrig ERROR " << trigName << " already included " << endl;
    return;
  }
  if(m_debug) cout << "TrigEmulatorTool::AddTrig ADding trig " << trigName << endl;
  assert(PFJetName.size() == PFJetMult.size());

  
  if(m_PFHt.find(HTName) == m_PFHt.end()){
    cout << "TrigEmulatorTool::AddTrig ERROR PFHt " << HTName << " not defined " << endl;
    assert(false);
  }  
  
  HLTHtEmulator* thisPFHt = m_PFHt[HTName];

  if(m_debug) cout << "TrigEmulatorTool::AddTrig making vetor of  HLTJetEmulators " << endl;
  std::vector<HLTJetEmulator*> thisPFJets; 
  for(const std::string& pf : PFJetName){
    if(m_PFJet.find(pf) == m_PFJet.end()){
      cout << "TrigEmulatorTool::AddTrig ERROR PFJet " << pf << " not defined " << endl;
      assert(false);
    }  

    thisPFJets.push_back(m_PFJet[pf]);
  }


  if(m_debug) cout << "TrigEmulatorTool::AddTrig inserting  " << endl;
  m_emulatedTrigMenu .insert(std::make_pair(trigName, new TrigEmulator(trigName, thisPFHt, thisPFJets,  PFJetMult, m_nToys)));
  m_emulatedDecisions.insert(std::make_pair(trigName, false));
  m_emulatedWeights  .insert(std::make_pair(trigName, 0.0));
  
}

void TrigEmulatorTool::SetDecisions(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){
    if(trigIt->second->passTrig(offline_jets, ht)){
      m_emulatedDecisions[trigIt->first] = true;
    }else{
      m_emulatedDecisions[trigIt->first] = false;
    }
    
  }
  return;
}



bool TrigEmulatorTool::GetDecision(std::string trigName){
  if(m_emulatedDecisions.find(trigName) == m_emulatedDecisions.end()){
    cout << "TrigEmulatorTool::GetDecisions ERROR " << trigName << " not defined. Returning false " << endl;
    return false;
  }
  return m_emulatedDecisions[trigName];
}


void TrigEmulatorTool::SetWeights(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){
    m_emulatedWeights[trigIt->first] = trigIt->second->calcWeight(offline_jets, ht);
  }
  return;
}


float TrigEmulatorTool::GetWeight(std::string trigName){
  if(m_emulatedWeights.find(trigName) == m_emulatedWeights.end()){
    cout << "TrigEmulatorTool::GetWeight ERROR " << trigName << " not defined. Returning 0.0 " << endl;
    return 0.0;
  }
  return m_emulatedWeights[trigName];
}


void TrigEmulatorTool::Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets, float ht){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt)
    trigIt->second->Fill(offline_jets, ht);
  return;
}


void TrigEmulatorTool::dumpResults(){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){
    std::cout << trigIt->first << " ";
    trigIt->second->dumpResults();
  }

  return;
}
