#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulatorTool.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"
#include <iostream>
#include <assert.h>
#include <array>

using namespace TriggerEmulator;

using std::cout;  using std::endl;
using std::vector;  using std::string; using std::array;

TrigEmulatorTool::TrigEmulatorTool(string name, int mode, unsigned int nToys, string year, bool debug) :  m_name(name),  m_nToys(nToys), m_debug(debug)
{

  if(year == "2018"){
    config2018();
  }else if(year == "2017"){
    config2017();
  }else if(year == "2016"){
    config2016();
  }else{
    cout << "TrigEmulatorTool::ERROR year has to be 2018, 2017 or 2016. Not " << year << endl;
  }

  //
  //  Emulated Jet Thresholds Triggers
  // 
  if(debug) cout << "TrigEmulatorTool::Making Jet Thresholds " << endl;
  for (auto const& jc : JetConfig){
    m_Jet.insert(std::make_pair(jc.first,new HLTJetEmulator(jc.second.at(0),jc.second.at(1),debug)));
  }

  //
  //  Emulated HT Thresholds
  //
  if(debug) cout << "TrigEmulatorTool::Making Ht Thresholds " << endl;
  for (auto const& hc : HTConfig){
    m_Ht.insert(std::make_pair(hc.first,new HLTHtEmulator(hc.second.at(0),hc.second.at(1),debug)));
  }

  //
  //  Emulated BTag Thresholds
  //
  if(debug) cout << "TrigEmulatorTool::Making BTag Thresholds " << endl;
  for (auto const& bc : BTagConfig){
    m_BTag.insert(std::make_pair(bc.first,new HLTBTagEmulator(bc.second.at(0),bc.second.at(1),debug)));
  }


  if(debug) cout << "TrigEmulatorTool::Leaving " << endl;
}


// Just HT 
void TrigEmulatorTool::AddTrig(string trigName,  const vector<hTTurnOn>& HTNames){
			       
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Ht Only " << trigName << endl;
  return AddTrig(trigName, HTNames, vector<jetTurnOn>(), vector<unsigned int>(), vector<bTagTurnOn>(), vector<unsigned int>());
}


// jets
void TrigEmulatorTool::AddTrig(string trigName,  
			       const vector<jetTurnOn>&  JetNames, const vector<unsigned int>& JetMults){
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Jets Only " << trigName << endl;
  return AddTrig(trigName, vector<hTTurnOn>(), JetNames, JetMults, vector<bTagTurnOn>(), vector<unsigned int>());
}

// HT and Jets
void TrigEmulatorTool::AddTrig(string trigName,  const vector<hTTurnOn>& HTNames, 
			       const vector<jetTurnOn>&  JetNames, const vector<unsigned int>& JetMults
			       ){
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Ht and Jets " << trigName << endl;
  return AddTrig(trigName, HTNames, JetNames, JetMults, vector<bTagTurnOn>(), vector<unsigned int>());
}

// jets and btags
void TrigEmulatorTool::AddTrig(string trigName,  
			       const vector<jetTurnOn>&  JetNames, const vector<unsigned int>& JetMults, 
			       const vector<bTagTurnOn>&  TagNames, const vector<unsigned int>& TagMults){
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Jet and btags " << trigName << endl;
  return AddTrig(trigName, vector<hTTurnOn>(), JetNames, JetMults, TagNames, TagMults);
}



void TrigEmulatorTool::AddTrig(string trigName,  const vector<hTTurnOn>& HTNames, 
			       const vector<jetTurnOn>&  JetNames, const vector<unsigned int>& JetMults,
			       const vector<bTagTurnOn>&  TagNames, const vector<unsigned int>& TagMults
			       ){
  if(m_debug) cout << "TrigEmulatorTool::AddTrig Enter " << trigName << endl;
  if(m_emulatedTrigMenu.find(trigName) != m_emulatedTrigMenu.end()){
    cout << "TrigEmulatorTool::AddTrig ERROR " << trigName << " already included " << endl;
    return;
  }
  if(m_debug) cout << "TrigEmulatorTool::AddTrig ADding trig " << trigName << endl;
  assert(JetNames.size() == JetMults.size());
  assert(TagNames.size() == TagMults.size());

  //
  // Config The Ht
  //
  vector<HLTHtEmulator*> HTCuts; 
  for(const hTTurnOn& HTName : HTNames){
    if(m_Ht.find(HTName) == m_Ht.end()){
      cout << "TrigEmulatorTool::AddTrig ERROR Ht " << HTName << " not defined " << endl;
      assert(false);
    }  
    HTCuts.push_back(m_Ht[HTName]);
  }

  //
  // Config The Jets
  //
  if(m_debug) cout << "TrigEmulatorTool::AddTrig making vetor of  HLTJetEmulators " << endl;
  vector<HLTJetEmulator*> JetPtCuts; 
  for(const jetTurnOn& jetName : JetNames){
    if(m_Jet.find(jetName) == m_Jet.end()){
      cout << "TrigEmulatorTool::AddTrig ERROR Jet " << jetName << " not defined " << endl;
      assert(false);
    }  
    JetPtCuts.push_back(m_Jet[jetName]);
  }

  // 
  // Config the BTags
  //
  vector<HLTBTagEmulator*> BTagPtCuts; 
  for(const bTagTurnOn& btagName : TagNames){
    if(m_BTag.find(btagName) == m_BTag.end()){
      cout << "TrigEmulatorTool::AddTrig ERROR BTag " << btagName << " not defined " << endl;
      assert(false);
    }  
    BTagPtCuts.push_back(m_BTag[btagName]);
  }

  if(m_debug) cout << "TrigEmulatorTool::AddTrig inserting  " << endl;
  m_emulatedTrigMenu .insert(std::make_pair(trigName, new TrigEmulator(trigName, HTCuts, JetPtCuts,  JetMults, BTagPtCuts, TagMults, m_nToys)));
  m_emulatedDecisions.insert(std::make_pair(trigName, false));
  m_emulatedWeights  .insert(std::make_pair(trigName, 0.0));
  
}

void TrigEmulatorTool::SetDecisions(const  vector<float>& offline_jet_pts, const vector<float>& offline_btagged_jet_pts, float ht){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){
    if(trigIt->second->passTrig(offline_jet_pts, offline_btagged_jet_pts, ht)){
      m_emulatedDecisions[trigIt->first] = true;
    }else{
      m_emulatedDecisions[trigIt->first] = false;
    }
    
  }
  return;
}



bool TrigEmulatorTool::GetDecision(string trigName){
  if(m_emulatedDecisions.find(trigName) == m_emulatedDecisions.end()){
    cout << "TrigEmulatorTool::GetDecisions ERROR " << trigName << " not defined. Returning false " << endl;
    return false;
  }
  return m_emulatedDecisions[trigName];
}


void TrigEmulatorTool::SetWeights(const vector<float>& offline_jet_pts, const vector<float>& offline_btagged_jet_pts, float ht){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){
    m_emulatedWeights[trigIt->first] = trigIt->second->calcWeight(offline_jet_pts, offline_btagged_jet_pts, ht);
  }
  return;
}


float TrigEmulatorTool::GetWeight(string trigName){
  if(m_emulatedWeights.find(trigName) == m_emulatedWeights.end()){
    cout << "TrigEmulatorTool::GetWeight ERROR " << trigName << " not defined. Returning 0.0 " << endl;
    return 0.0;
  }
  return m_emulatedWeights[trigName];
}

float TrigEmulatorTool::GetWeightOR(const vector<float>& offline_jet_pts, const vector<float>& offline_btagged_jet_pts, float ht){
  
  unsigned int nPass  = 0;

  for(unsigned int iToy = 0; iToy < m_nToys; ++iToy){
    
    bool passAny = false;

    for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){

      // 
      // Count all events
      //
      if(trigIt->second->passTrig(offline_jet_pts, offline_btagged_jet_pts, ht, iToy))
	passAny = true;
	
    }

    if(passAny)
      ++nPass;
  }
  
  return float(nPass)/m_nToys;
}



void TrigEmulatorTool::Fill(const vector<float>& offline_jet_pts, const vector<float>& offline_btagged_jet_pts, float ht){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt)
    trigIt->second->Fill(offline_jet_pts, offline_btagged_jet_pts, ht);
  return;
}


void TrigEmulatorTool::dumpResults(){
  for (auto trigIt=m_emulatedTrigMenu.begin(); trigIt!=m_emulatedTrigMenu.end(); ++trigIt){
    std::cout << trigIt->first << " ";
    trigIt->second->dumpResults();
  }

  return;
}





void TrigEmulatorTool::config2018(){

  cout << "TrigEmulatorTool::configuring for 2018 " << endl;

  std::string fileName2018 = "histMC_2018_All_fittedTurnOns.root";

  //
  //  Jet Emulator
  //
  JetConfig = {
    {jetTurnOn::PF30,                   {{"pt_PF30_TurnOn",                     fileName2018}} },
    {jetTurnOn::PF40,                   {{"pt_PF40_TurnOn",                     fileName2018}} },
    {jetTurnOn::PF45,                   {{"pt_PF45_TurnOn",                     fileName2018}} },
    {jetTurnOn::PF60,                   {{"pt_PF60_TurnOn",                     fileName2018}} },
    {jetTurnOn::PF75,                   {{"pt_PF75_TurnOn"        ,             fileName2018}} },
    {jetTurnOn::L1112,                  {{"pt_2b116_L1ORAll_TurnOn",            fileName2018}} },
    {jetTurnOn::Calo100,                {{"pt_Calo100_TurnOn"     ,             fileName2018}} },   
    {jetTurnOn::PF116,                  {{"pt_PFJets116_TurnOn"   ,             fileName2018}} },   
    {jetTurnOn::PF116Dr,                {{"pt_PFJets116Dr_TurnOn" ,             fileName2018}} },
    {jetTurnOn::L1112TandP,             {{"pt_L1112TandP_TurnOn",               fileName2018}} },
    {jetTurnOn::L1112TandPDenMatch,     {{"pt_L1112TandPDenMatch_TurnOn",       fileName2018}} },
    {jetTurnOn::Calo100DenMatch,        {{"pt_Calo100DenMatch_TurnOn"     ,     fileName2018}} },   
    {jetTurnOn::PF116DenMatch,          {{"pt_PFJets116DenMatch_TurnOn"   ,     fileName2018}} },   
    {jetTurnOn::PF116DrDenMatch,        {{"pt_PFJets116DrDenMatch_TurnOn" ,     fileName2018}} },
    {jetTurnOn::PF30DenMatch,           {{"pt_PF30DenMatch_TurnOn",             fileName2018}} },
    {jetTurnOn::PF40DenMatch,           {{"pt_PF40DenMatch_TurnOn",             fileName2018}} },
    {jetTurnOn::PF45DenMatch,           {{"pt_PF45DenMatch_TurnOn",             fileName2018}} },
    {jetTurnOn::PF60DenMatch,           {{"pt_PF60DenMatch_TurnOn",             fileName2018}} },
    {jetTurnOn::PF75DenMatch,           {{"pt_PF75DenMatch_TurnOn",             fileName2018}} },
  };

  //
  //  BTag Emulator
  //
  BTagConfig = {
    {bTagTurnOn::PFDeepCSVMatchBtag,                 {{"pt_PFDeepCSVMatchBtag_TurnOn",               fileName2018}} },
    {bTagTurnOn::CaloDeepCSV0p7MatchBtag,            {{"pt_CaloDeepCSV0p7MatchBtag_TurnOn",          fileName2018}} },
    {bTagTurnOn::PFDeepCSVMatchBtagDenMatch,         {{"pt_PFDeepCSVMatchBtagDenMatch_TurnOn",       fileName2018}} },
    {bTagTurnOn::CaloDeepCSV0p7MatchBtagDenMatch,    {{"pt_CaloDeepCSV0p7MatchBtagDenMatch_TurnOn",  fileName2018}} },
  };

  //
  // Ht Config
  //
  HTConfig = {
    {hTTurnOn::L1ORAll_Ht330_4j_3b,            {{"hT30TurnOn_L1ORAll_L1UnprescaledPassNJet_Ht330_4j_3b",      fileName2018}} },
    {hTTurnOn::L1ORAll_2b116,                  {{"hT30_lTurnOn_L1ORAll_L1UnprescaledPassNJet_2b116",          fileName2018}} },
    {hTTurnOn::CaloHt320,                      {{"hT30TurnOn_CaloHt320_L1UnprescaledPassNJet_Ht330_4j_3b",    fileName2018}} },
    {hTTurnOn::PFHt330,                        {{"hT30TurnOn_PFHt330_L1UnprescaledPassNJet_Ht330_4j_3b",      fileName2018}} },
  };


}


void TrigEmulatorTool::config2017(){

  cout << "TrigEmulatorTool::configuring for 2017 " << endl;
  std::string fileName2017 = "histMC_2017_All_fittedTurnOns.root";

  //
  //  Jet Emulator
  //
  JetConfig = {
    {jetTurnOn::L1100TandP,              {{"pt_L1100TandP_TurnOn",              fileName2017}} },
    {jetTurnOn::L1100TandPDenMatch,      {{"pt_L1100TandPDenMatch_TurnOn",      fileName2017}} }, 
    {jetTurnOn::Calo100DenMatch,         {{"pt_Calo100DenMatch_TurnOn"     ,    fileName2017}} },   
    {jetTurnOn::PF100DenMatch,           {{"pt_PFJets100DenMatch_TurnOn"   ,    fileName2017}} },   
    {jetTurnOn::PF100DrDenMatch,         {{"pt_PFJets100DrDenMatch_TurnOn" ,    fileName2017}} },
    {jetTurnOn::PF30DenMatch,            {{"pt_PF30DenMatch_TurnOn",            fileName2017}} },
    {jetTurnOn::PF40DenMatch,            {{"pt_PF40DenMatch_TurnOn",            fileName2017}} },
    {jetTurnOn::PF45DenMatch,            {{"pt_PF45DenMatch_TurnOn",            fileName2017}} },
    {jetTurnOn::PF60DenMatch,            {{"pt_PF60DenMatch_TurnOn",            fileName2017}} },
    {jetTurnOn::PF75DenMatch,            {{"pt_PF75DenMatch_TurnOn",            fileName2017}} },
  };


  //
  //  BTag Emulator
  //
  BTagConfig = {
    {bTagTurnOn::PFDeepCSVMatchBtag,                 {{"pt_PFDeepCSVMatchBtag_TurnOn",              fileName2017}} },
    {bTagTurnOn::CaloDeepCSV0p7MatchBtag,            {{"pt_CaloDeepCSV0p7MatchBtag_TurnOn",         fileName2017}} },
    {bTagTurnOn::PFDeepCSVMatchBtagDenMatch,         {{"pt_PFDeepCSVMatchBtagDenMatch_TurnOn",      fileName2017}} },
    {bTagTurnOn::CaloDeepCSV0p7MatchBtagDenMatch,    {{"pt_CaloDeepCSV0p7MatchBtagDenMatch_TurnOn", fileName2017}} },
  };

  //
  // Ht Config
  //
  HTConfig = {
    {hTTurnOn::L1ORAll_Ht300_4j_3b,       {{"hT30TurnOn_L1ORAll_L1UnprescaledPassNJet_Ht300_4j_3b",      fileName2017}} },
    {hTTurnOn::L1ORAll_2b100,             {{"hT30_lTurnOn_L1ORAll_L1UnprescaledPassNJet_2b100",          fileName2017}} },
    {hTTurnOn::CaloHt300,                 {{"hT30TurnOn_CaloHt300_L1UnprescaledPassNJet_Ht300_4j_3b",    fileName2017}} },
    {hTTurnOn::PFHt300,                   {{"hT30TurnOn_PFHt300_L1UnprescaledPassNJet_Ht300_4j_3b",      fileName2017}} },
  };

}



void TrigEmulatorTool::config2016(){

  cout << "TrigEmulatorTool::configuring for 2016 " << endl;
  std::string fileName2016 = "histMC_2016_All_fittedTurnOns.root";

  //
  //  Jet Emulator
  //
  JetConfig = {
    {jetTurnOn::L1100TandP,            {{"pt_L1100TandP_TurnOn",              fileName2016}} },
    {jetTurnOn::L1100TandPDenMatch,    {{"pt_L1100TandPDenMatch_TurnOn",      fileName2016}} },
    {jetTurnOn::Calo30,                {{"pt_Calo30_TurnOn"     ,             fileName2016}} },
    {jetTurnOn::Calo45,                {{"pt_Calo45_TurnOn"     ,             fileName2016}} },
    {jetTurnOn::Calo90DenMatch,        {{"pt_Calo90DenMatch_TurnOn"     ,     fileName2016}} },   
    {jetTurnOn::Calo100DenMatch,       {{"pt_Calo100DenMatch_TurnOn"     ,    fileName2016}} },   
    {jetTurnOn::PF30DenMatch,          {{"pt_PF30DenMatch_TurnOn",            fileName2016}} },
    {jetTurnOn::PF45DenMatch,          {{"pt_PF45DenMatch_TurnOn",            fileName2016}} },
    {jetTurnOn::PF90DenMatch,          {{"pt_PF90DenMatch_TurnOn"   ,         fileName2016}} },   
    {jetTurnOn::PF100DenMatch,         {{"pt_PFJets100DenMatch_TurnOn"   ,    fileName2016}} },   
    {jetTurnOn::PF100DrDenMatch,       {{"pt_PFJets100DrDenMatch_TurnOn" ,    fileName2016}} },
  };

  //
  //  BTag Emulator
  //
  BTagConfig = {
    {bTagTurnOn::CaloCSVMatchBtag,             {{"pt_CaloCSVMatchBtag_TurnOn",             fileName2016}} },
    {bTagTurnOn::CaloCSVMatchBtagDenMatch,     {{"pt_CaloCSVMatchBtagDenMatch_TurnOn",     fileName2016}} },
    {bTagTurnOn::CaloCSV0p84MatchBtag,         {{"pt_CaloCSV0p84MatchBtag_TurnOn",         fileName2016}} },
    {bTagTurnOn::CaloCSV0p84MatchBtagDenMatch, {{"pt_CaloCSV0p84MatchBtagDenMatch_TurnOn", fileName2016}} },
  };

  //
  // HT Config
  //
  HTConfig = {
    {hTTurnOn::L1ORAll_4j_3b,            {{"hT30TurnOn_L1ORAll_L1UnprescaledPassNJet_4j_3b",      fileName2016}} },
    {hTTurnOn::L1ORAll_2b100,            {{"hT30_lTurnOn_L1ORAll_L1UnprescaledPassNJet_2b100",      fileName2016}} },
    {hTTurnOn::L1ORAll_2j_2j_3b,         {{"hT30TurnOn_L1ORAll_L1UnprescaledPassNJet_2j_2j_3b",      fileName2016}} },
  };



}


