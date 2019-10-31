// -*- C++ -*-
#if !defined(TrigEmulatorTool_H)
#define TrigEmulatorTool_H

#include "TriggerEmulator/nTupleAnalysis/interface/TrigEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTJetEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTBTagEmulator.h"
#include "TriggerEmulator/nTupleAnalysis/interface/HLTHtEmulator.h"
#include <string>
#include <map>


namespace TriggerEmulator {

  //
  // Trigger Emulation
  //
  class TrigEmulatorTool{

  public:

    TrigEmulatorTool(std::string name, int mode = 1, int nToys=1000, bool debug = false);

    // Just jets
    void AddTrig(std::string trigName,  const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult);

    // Just jets and btags
    void AddTrig(std::string trigName,  const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult, std::string PFTagName, unsigned int PFTagMult);

    // HT and jets
    void AddTrig(std::string trigName,  std::string HTName, const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult);

    // HT jet and btags
    void AddTrig(std::string trigName,  std::string HTName, const std::vector<std::string>&  PFJetName, const std::vector<unsigned int>& PFJetMult, std::string PFTagName, unsigned int PFTagMult);

    //
    //  Sets decisions for all configured triggers (involves random numbers if mode == 1)
    //
    void SetDecisions(std::vector<float> offline_jet_pts, std::vector<float> offline_btagged_jet_pts, float ht=-1);

    //
    //  Return the value set in SetDecisions.  (So must call SetDecisions before GetDecision/Passed
    //
    bool GetDecision (std::string trigName);
    bool Passed      (std::string trigName) { return GetDecision(trigName); }

    //
    //  Sets weights for all configured triggers, which is the average nPass over nToys (involves random numbers if mode == 1)
    //
    void SetWeights(std::vector<float> offline_jet_pts, std::vector<float> offline_btagged_jet_pts, float ht=-1);

    //
    //  Return the value set in SetWeights.  (So must call SetWeights before GetWeight)
    //
    float GetWeight (std::string trigName);

    // 
    //  For doing global run counting (Eg: in rate prediction)
    //
    void Fill(std::vector<float> offline_jet_pts, std::vector<float> offline_btagged_jet_pts, float ht=-1);

    void dumpResults();

  private:

    std::map<std::string, HLTHtEmulator*>    m_PFHt;
    std::map<std::string, HLTJetEmulator*>   m_PFJet;
    std::map<std::string, HLTBTagEmulator*>  m_PFBTag;
    std::map<std::string, TrigEmulator*>     m_emulatedTrigMenu;
    std::map<std::string, bool>              m_emulatedDecisions;
    std::map<std::string, float>             m_emulatedWeights;

    std::string m_name;
    int m_nToys;
    
    int m_mode; // 0 - flat cut / 1 - use turn on
    bool m_debug;


    //
    // PF Jet Config
    //
    //std::vector<std::string> m_PFJet_trigNames = {"15", "30", "40",  "40wL1", "45",   "50",  "60",   "75",   "80",   "90",   "100",   "105",  "110",   "120",  "140",  "200",  "260",  "320",   "400", "450",  "500","550"};
    //std::vector<float> m_PFJet_trigThresholds  = { 15,   30,   40. ,  40.,     45,     50,    60.,    75.,    80.,    90.,    100.,    105.,   110.,    120.,   140.,   200.,   260.,   320.,    400.,   450.,   500.,  550.};
    //// From Fits to Data 2018
    //std::vector<float> m_PFJet_p0              = {0.04, 0.04, 0.188,  0.088,   0.211, 0.204,  0.188,  0.172,  0.169,  0.168,  0.158 ,  0.159,  0.157,   0.151,  0.144,  0.116,  0.149,  0.082,  0.100,   0.086,  0.085,  0.087};
    //std::vector<float> m_PFJet_p1              = {-310, -310, 42.5,   47.1,    50.0,  56.0,   67.3,   83.8,   89.2,   100.0,  110.9,   116.3,  121.6,   132.0,  153.2,  215.2,  277.1,  336.5,  411.5,  462.3,  505.5,  559.1};
    std::vector<std::string> m_PFJet_trigNames = {"40",                           "40wL140",                       "45",                            "45wL140",                         "60",                             "60wL155",                        "75",                       "75wL170"};			    
    std::vector<std::string> m_PFJet_fileNames = {"EffOnlineWrtOffline_ZHMC.root","EffOnlineWrtOffline_ZHMC.root", "EffOnlineWrtOffline_ZHMC.root", "EffOnlineWrtOffline_ZHMC.root",   "EffOnlineWrtOffline_ZHMC.root", "EffOnlineWrtOffline_ZHMC.root",   "EffOnlineWrtOffline_ZHMC.root", "EffOnlineWrtOffline_ZHMC.root"};
    std::vector<std::string> m_PFJet_histNames = {"matchedPt_h40_rebin_ratio",    "matchedPt_h40_l40_rebin_ratio", "matchedPt_h45_rebin_ratio",      "matchedPt_h45_l40_rebin_ratio",   "matchedPt_h60_rebin_ratio",    "matchedPt_h60_l55_rebin_ratio",   "matchedPt_h75_rebin_ratio", "matchedPt_h75_l70_rebin_ratio"};


    //
    //  PF BTag Emulator
    //
    std::vector<std::string> m_PFBTag_trigNames = {"none", "2018",                                              "2017"};   
    std::vector<std::string> m_PFBTag_fileNames = {"none", "EffOnlineWrtOffline2018.root",                      "EffOnlineWrtOffline2017.root"};   
    std::vector<std::string> m_PFBTag_histNames = {"none", "Eff_Data_offJetsMedDeepFlav_matchedPFDeepCSV_pt_m", "Eff_Data_offJetsMedDeepFlav_matchedPFCSV_pt_m"};   
    

    //
    // PF Ht Config
    //
    std::vector<std::string> m_PFHt_trigNames = {"none", "180",     "250",  "330ZH", "330ZH_l1",   "330",   "370",    "430",   "510",   "590",    "680",   "780",    "890",   "1050"};
    std::vector<float> m_PFHt_trigThresholds  = {0,       180. ,    250.,   330.,     330.,          330.,     370.,     430.,    510.,    590.,     680.,    780.,     890.,    1050.};
    std::vector<float> m_PFHt_p0              = {10,      0.0319,   0.0343, 0.051,    0.051,         0.03,     0.0334,  0.0347,  0.0334,   0.0313,   0.0294,  0.0283,  0.0268,     0.0273};
    std::vector<float> m_PFHt_p1              = {-300,    213.0,    294.1 , 362.5,    362.5,         401.8,    422.1 ,   483.4 ,   566.9 ,  650.0 ,    743.7 , 847.4 ,  956.5 ,    1113.};
    std::vector<std::string> m_PFHt_fileNames = {"none", "none",    "none", "EffOnlineWrtOffline_ZHMC.root", "EffOnlineWrtOffline_ZHMC.root",   "HT_EffOnlineWrtOffline2018_v3.root", "none", "none",    "none", "none", "none",    "none", "none", "none"};   
    std::vector<std::string> m_PFHt_histNames = {"none", "none",    "none",  "hT30_h330_ratio", "hT30_h330_l320_ratio",   "ht330_sel_3tag_pass_4b_rebin_ratio" , "none", "none",    "none", "none", "none",    "none", "none", "none"};   

  };


}

#endif // TrigEmulator_H

