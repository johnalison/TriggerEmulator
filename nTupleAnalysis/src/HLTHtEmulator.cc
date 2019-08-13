#include "TriggerEmulator/nTupleAnalysis/interface/HLTHtEmulator.h"

#include <TRandom3.h>
#include <TF1.h>


using namespace TriggerEmulator;

HLTHtEmulator::HLTHtEmulator(std::string name, float p0, float p1, float htCut, int mode){
  m_name = name;
  m_rand = new TRandom3();
  
  m_sigmoid = new TF1(("func"+name).c_str(),"(1.0/(1+ TMath::Exp(-[0]*(x-[1]))))", -301, 14000);
  m_sigmoid->SetParameters(p0, p1);
  
  m_mode = mode;
  m_htCut = htCut;
}

bool HLTHtEmulator::passHt(float ht){
  if(m_mode == 0){
    if(ht > m_htCut) {
      return true;
    }
    return false;
  }

  float probPassTrig = m_sigmoid->Eval(ht);
  if(probPassTrig > m_rand->Rndm())
    return true;
  return false;
}
