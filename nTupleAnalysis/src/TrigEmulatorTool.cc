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

}


void TrigEmulatorTool::AddTrig(std::string trigName,  const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult){
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Enter " << trigName << endl;
  if(m_emulatedTrigMenu.find(trigName) != m_emulatedTrigMenu.end()){
    cout << "TrigEmulatorTool::AddTrig ERROR " << trigName << " already included " << endl;
    return;
  }
  if(m_debug) cout << "TrigEmulatorTool::AddTrig ADding trig " << trigName << endl;
  assert(PFJetName.size() == PFJetMult.size());



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
  m_emulatedTrigMenu.insert(std::make_pair(trigName, new TrigEmulator(trigName, thisPFJets,  PFJetMult, m_nToys)));
}


void TrigEmulatorTool::Fill(std::vector<nTupleAnalysis::jetPtr> offline_jets){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt)
    trigIt->second->Fill(offline_jets);
  //dafsd
  return;
}


void TrigEmulatorTool::dumpResults(){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){
    std::cout << trigIt->first << " ";
    trigIt->second->dumpResults();
  }

  return;
}
