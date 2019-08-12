// -*- C++ -*-
#if !defined(HLTJetEmulator_H)
#define HLTJetEmulator_H

#include <string>

class TRandom3;
class TF1;

namespace TriggerEmulator {

  class HLTJetEmulator {
      
  public:

    HLTJetEmulator(std::string jetName, float p0, float p1, float ptCut, int jetMode);

    bool passJet(float pt);

    std::string name;

  private:
    TRandom3*   rand = nullptr;
    TF1*        sigmoid = nullptr;
    int         mode            = 0; // Flat Pt Cut 
    float       flatPTCut;


  };

}

#endif // HLTJetEmulator_H
