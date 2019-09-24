#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"

#include <TRandom3.h>
#include <TF1.h>


using namespace TriggerEmulator;

HLTJetEmulator::HLTJetEmulator(std::string jetName, float p0, float p1, float ptCut, int jetMode){
  name = jetName;
  rand = new TRandom3();
  
  sigmoid = new TF1(("func"+name).c_str(),"(1.0/(1+ TMath::Exp(-[0]*(x-[1]))))", 0, 14000);
  sigmoid->SetParameters(p0, p1);
  
  mode = jetMode;
  flatPTCut = ptCut;
}

bool HLTJetEmulator::passJet(float pt, float seedOffset){
  if(mode == 0){
    if(pt > flatPTCut) {
      return true;
    }
    return false;
  }

  float probPassTrig = sigmoid->Eval(pt);
  //float thisTagEff = eff + effErr*smearFactor;
  int seed = (int)(pt * seedOffset + pt); 
  //rand->SetSeed(seed);
  if(probPassTrig > rand->Rndm())
    return true;
  return false;
}
