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
void pileUp_SFs( TString MCfile = "" ) {
  TH1::SetDefaultSumw2();
  // double lumi = 1546.395;

  //2017 Moriond
  std::vector<double> MCPU = {1.78653e-05 ,2.56602e-05 ,5.27857e-05 ,8.88954e-05 ,0.000109362 ,0.000140973 ,0.000240998 ,0.00071209 ,0.00130121 ,0.00245255 ,0.00502589 ,0.00919534 ,0.0146697 ,0.0204126 ,0.0267586 ,0.0337697 ,0.0401478 ,0.0450159 ,0.0490577 ,0.0524855 ,0.0548159 ,0.0559937 ,0.0554468 ,0.0537687 ,0.0512055 ,0.0476713 ,0.0435312 ,0.0393107 ,0.0349812 ,0.0307413 ,0.0272425 ,0.0237115 ,0.0208329 ,0.0182459 ,0.0160712 ,0.0142498 ,0.012804 ,0.011571 ,0.010547 ,0.00959489 ,0.00891718 ,0.00829292 ,0.0076195 ,0.0069806 ,0.0062025 ,0.00546581 ,0.00484127 ,0.00407168 ,0.00337681 ,0.00269893 ,0.00212473 ,0.00160208 ,0.00117884 ,0.000859662 ,0.000569085 ,0.000365431 ,0.000243565 ,0.00015688 ,9.88128e-05 ,6.53783e-05 ,3.73924e-05 ,2.61382e-05 ,2.0307e-05 ,1.73032e-05 ,1.435e-05 ,1.36486e-05 ,1.35555e-05 ,1.37491e-05 ,1.34255e-05 ,1.33987e-05 ,1.34061e-05 ,1.34211e-05 ,1.34177e-05 ,1.32959e-05 ,1.33287e-05};

  /// 2016 ICHEP
  // std::vector<double> MCPU = {
  // 			  0.000829312873542,
  // 			  0.00124276120498,
  // 			  0.00339329181587,
  // 			  0.00408224735376,
  // 			  0.00383036590008,
  // 			  0.00659159288946,
  // 			  0.00816022734493,
  // 			  0.00943640833116,
  // 			  0.0137777376066,
  // 			  0.017059392038,
  // 			  0.0213193035468,
  // 			  0.0247343174676,
  // 			  0.0280848773878,
  // 			  0.0323308476564,
  // 			  0.0370394341409,
  // 			  0.0456917721191,
  // 			  0.0558762890594,
  // 			  0.0576956187107,
  // 			  0.0625325287017,
  // 			  0.0591603758776,
  // 			  0.0656650815128,
  // 			  0.0678329011676,
  // 			  0.0625142146389,
  // 			  0.0548068448797,
  // 			  0.0503893295063,
  // 			  0.040209818868,
  // 			  0.0374446988111,
  // 			  0.0299661572042,
  // 			  0.0272024759921,
  // 			  0.0219328403791,
  // 			  0.0179586571619,
  // 			  0.0142926728247,
  // 			  0.00839941654725,
  // 			  0.00522366397213,
  // 			  0.00224457976761,
  // 			  0.000779274977993,
  // 			  0.000197066585944,
  // 			  7.16031761328e-05,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0,
  // 			  0.0 };

  int numBin = int(MCPU.size());
  TH1D* h_PU_MC = new TH1D("h_PU_MC","h_PU_MC", numBin, 0, numBin);

  std::cout << "MC PU bin size" << numBin << std::endl;
  for(int iBin=0; iBin<numBin; iBin++ ){
    double iPU = MCPU[iBin];
    h_PU_MC->SetBinContent(iBin+1, iPU);
  }
  ///
  ///////// load root files
  // TFile *fileTTJets1 = TFile::Open(MCfile);

  TFile *fileData1 = TFile::Open("MyDataPileupHistogram.root");


  ////  load TH1F
  TString h_var_Name = "numPVs";

  /// ttjets
  // TH1D* h_var_ttjets = (TH1D*)fileTTJets1->Get("ttHTreeMaker/numPVs")->Clone(h_var_Name+"_ttjets");  ///histo name
  TH1D* h_var_ttjets = (TH1D*)h_PU_MC->Clone(h_var_Name+"_ttjets");  ///histo name
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

    TH1D* h_PUwgt = new TH1D("puWgt","puWgt", numBin, 0, numBin);
    // compute weights
    for(int bin = 1; bin <= nBins; ++bin) {
      double nDataEstimated = h_var_data->GetBinContent(bin);
      double nMC = h_var_ttjets->GetBinContent(bin);
      double weight = nMC>0. ? nDataEstimated/nMC : 0.;
      h_PUwgt->SetBinContent(bin, weight);
    }


    h_PUwgt->Write();

    h_PU_MC->Write();



  std::cout << "Done." << std::endl;


}
