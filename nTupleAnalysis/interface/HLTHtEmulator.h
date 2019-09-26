// -*- C++ -*-
#if !defined(HLTHtEmulator_H)
#define HLTHtEmulator_H

#include <string>
#include <vector>

class TRandom3;
class TF1;

namespace TriggerEmulator {

  class HLTHtEmulator {
      
  public:

    HLTHtEmulator(std::string name, float p0, float p1, float htCut, int mode, std::string fileName="", std::string histName="" );

    bool passHt(float ht, float seedOffset=1.0, float smearFactor = 0.0);

    std::string m_name;

  private:
    TRandom3*   m_rand = nullptr;
    TF1*        m_sigmoid = nullptr;
    int         m_mode            = 0; // 0 = Flat Pt Cut  /  1 = Sigmoid / 2 = TGraph
    float       m_htCut = -1;

    // EFficeincies and Uncertianties
    //std::vector<double> m_lowBinEdge;
    std::vector<double> m_highBinEdge;
    std::vector<double> m_eff;
    std::vector<double> m_effErr;

  };

}

#endif // HLTHtEmulator_H
