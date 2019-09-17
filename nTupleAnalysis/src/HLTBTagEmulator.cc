#include "TriggerEmulator/nTupleAnalysis/interface/HLTBTagEmulator.h"

#include <TGraphAsymmErrors.h>
#include <TRandom3.h>
#include <TFile.h>
#include <TSystem.h>


using namespace TriggerEmulator;

HLTBTagEmulator::HLTBTagEmulator(std::string tagName, std::string fileName, std::string histName){
  name = tagName;
  m_rand = new TRandom3();
  
  TFile* inputFile = new TFile(gSystem->ExpandPathName("$CMSSW_BASE/src/TriggerEmulator/nTupleAnalysis/data/EffOnlineWrtOffline2017.root"),"READ");
  inputFile->ls();

  TGraphAsymmErrors* relativeEff = (TGraphAsymmErrors*)("Eff_Data_offJetsMedDeepFlav_matchedPFDeepCSV_pt_m");
  
  unsigned int nBins = relativeEff->GetN();
  for(unsigned int iBin = 0; iBin<nBins; ++iBin){
    double eff, pt;
    relativeEff->GetPoint(iBin, pt, eff);
    //float pt_low   = relativeEff->GetErrorXlow(iBin);
    float pt_high  = relativeEff->GetErrorXhigh(iBin);
    float err_low  = relativeEff->GetErrorYlow(iBin);
    float err_high = relativeEff->GetErrorYhigh(iBin);
    float err_ave  = (err_low+err_high)/2;
    float error_total = err_ave;

    //if(sf_tag){
    //  double sf, ptSF;
    //  m_sf_tag->GetPoint(iBin, ptSF, sf);
    //  //float ptSF_low   = m_sf_tag->GetErrorXlow(iBin);
    //  //float ptSF_high  = m_sf_tag->GetErrorXhigh(iBin);
    //  float errSF_low  = m_sf_tag->GetErrorYlow(iBin);
    //  float errSF_high = m_sf_tag->GetErrorYhigh(iBin);
    //  float errSF_ave  = (errSF_low+errSF_high)/2;
    //
    //  eff *= sf;
    //  error_total = sqrt(err_ave*err_ave + errSF_ave*errSF_ave);
    //}

    //cout << "\tiBin " << iBin << " pt: " << pt-pt_low << " - " << pt << " - " << pt+pt_high
    //     << " eff: " << eff << " +/- " << error_total
    //     << endl;
    m_lowBinEdge .push_back(pt-pt_high);
    m_eff        .push_back(eff);
    m_effErr     .push_back(error_total);
  }
  


  inputFile->Close();
  delete inputFile;
}

bool HLTBTagEmulator::passJet(float pt, float smearFactor){

  //thisTagEff = tagEff->Eval(pt)*scaleFactor;
  float eff    = -99;
  //float sf     = -99;
  float effErr = -99;

  for(unsigned int iBin = 0; iBin< m_lowBinEdge.size(); ++iBin){
    if(pt > m_lowBinEdge.at(iBin)){
      eff    = m_eff   .at(iBin);
      effErr = m_effErr.at(iBin);
      break;
    }
  }
  if(eff < 0) {
    eff    = m_eff.back();
    effErr = m_effErr.back();
  }
  assert((eff > 0) && "ERROR eff < 0");

  float thisTagEff = eff + effErr*smearFactor;

  if(thisTagEff > m_rand->Rndm())
    return true;

  return false;
}
