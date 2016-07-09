//Root includes                                   
#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1.h"
#include "TF1.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include "TAxis.h"
#include "TKey.h"
#include "TList.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <cmath> 
#include <iomanip>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sstream>

//void checkVar( bool isHF = true, TString dirPostFix = "", TString varName = "first_jet_pt" ) {
void pileUp_SFs( TString MCfile = "/afs/cern.ch/work/l/lwming/csvRWT13TeV/ttjets_June12th.root" ) {
  TH1::SetDefaultSumw2();
  // double lumi = 1546.395;

  ///////// load root files
  TFile *fileTTJets1 = TFile::Open(MCfile);

  TFile *fileData1 = TFile::Open("MyDataPileupHistogram.root");


  ////  load TH1F
  TString h_var_Name = "numPVs";

  /// ttjets
  TH1D* h_var_ttjets = (TH1D*)fileTTJets1->Get("ttHTreeMaker/numPVs")->Clone(h_var_Name+"_ttjets");  ///histo name
  h_var_ttjets->Scale(1./h_var_ttjets->Integral());

  int nBins = h_var_ttjets->GetNbinsX();
  std::cout<< "total number of bins = " << nBins << std::endl;
  std::cout << "number of ttjets: " << h_var_ttjets->Integral( 0, 1+nBins ) << std::endl;

  TH1D* h_var_data = (TH1D*)fileData1->Get("pileup")->Clone(h_var_Name+"_data");
  h_var_data->Scale(1./h_var_data->Integral());

  std::cout<< "total number of bins = " << int(h_var_data->GetNbinsX()) << std::endl;
  std::cout << "number of data: " << h_var_data->Integral( 0, 1+nBins ) << std::endl;

  
  h_var_ttjets->SetLineColor(kRed);
  h_var_data->SetLineColor(kBlue);
  
  //////// PU reweighting

    TFile histofile("PileUPReweighting.root","recreate");
    histofile.cd();


    h_var_ttjets->Write();
    h_var_data->Write();

    ///
    TCanvas* myCan = new TCanvas("myCan", "myCan", 600,500);
    myCan->cd();

    h_var_data->SetStats(0);
    h_var_data->Draw();
    h_var_ttjets->Draw("same");

    myCan->Print("numPV_compare.png");

    ///
    TH1D* myPU = (TH1D*)h_var_data->Clone(h_var_Name+"_PUratio");
    myPU->Divide(h_var_ttjets);
    myPU->Write();

    TH1D* h_PU = new TH1D("puWgt","puWgt", 50, 0, 50);
    // compute weights
    for(int bin = 1; bin <= nBins; ++bin) {
      double nDataEstimated = h_var_data->GetBinContent(bin);
      double nMC = h_var_ttjets->GetBinContent(bin);
      double weight = nMC>0. ? nDataEstimated/nMC : 0.;
      h_PU->SetBinContent(bin, weight);
    }


    h_PU->Write();




  std::cout << "Done." << std::endl;


}
