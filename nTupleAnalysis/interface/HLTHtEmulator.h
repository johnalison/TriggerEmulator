// -*- C++ -*-
#if !defined(HLTHtEmulator_H)
#define HLTHtEmulator_H

#include <string>

class TRandom3;
class TF1;

namespace TriggerEmulator {

  class HLTHtEmulator {
      
  public:

    HLTHtEmulator(std::string name, float p0, float p1, float htCut, int mode);

    bool passHt(float ht);

    std::string m_name;

  private:
    TRandom3*   m_rand = nullptr;
    TF1*        m_sigmoid = nullptr;
    int         m_mode            = 0; // Flat Pt Cut 
    float       m_htCut;


  };

}

#endif // HLTHtEmulator_H
