// -*- C++ -*-
#if !defined(HLTBTagEmulator_H)
#define HLTBTagEmulator_H

#include <string>
#include <vector>

class TRandom3;
class TGraphAsymmErrors;


namespace TriggerEmulator {

  class HLTBTagEmulator {
      
  public:

    HLTBTagEmulator(std::string tagName, std::string fileName, std::string histName);

    bool passJet(float pt, float smearFactor = 0.0);

    std::string name;

  private:

    TRandom3*           m_rand        = nullptr;

    // EFficeincies and Uncertianties
    std::vector<double> m_lowBinEdge;
    std::vector<double> m_eff;
    std::vector<double> m_effErr;
  };

}

#endif // HLTBTagEmulator_H