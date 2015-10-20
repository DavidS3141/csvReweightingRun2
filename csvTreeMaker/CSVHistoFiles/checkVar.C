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
void checkVar( bool isHF = true, TString dirPostFix = "" ) {


  TString dirprefix = "varImages" + dirPostFix + "/";

  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);


  /////
  // int nCSVBins = 18; //Number of bins 
  // double xBins_hf[19] = {-0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};


  // if(!isHF) nCSVBins = 21;
  // double xBins_lf[22] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  vector<TString> var_list;
  var_list.push_back("first_jet_pt");
  var_list.push_back("first_jet_eta");
  var_list.push_back("first_jet_csv");
  var_list.push_back("first_jet_flavour");
  var_list.push_back("first_jet_partonflavour");


  var_list.push_back("second_jet_pt");
  var_list.push_back("second_jet_eta");
  var_list.push_back("second_jet_csv");
  var_list.push_back("second_jet_flavour");
  var_list.push_back("second_jet_partonflavour");

  for (int i=0; i< var_list.size(); i++){
    TString varName = var_list[i];

  // TCanvas* c1 = new TCanvas("c1","c1",600,500);

  // c1->SetTopMargin(0.08);
  // c1->SetRightMargin(0.08);


  TString flavor_file = (isHF) ? "hf" : "lf"; 
  flavor_file.ToLower();

  TFile *fileTTJets = TFile::Open("csv_rwt_" + flavor_file + "_TTJets_v0_histo.root");
  TFile *fileZJets = TFile::Open("csv_rwt_" + flavor_file + "_DYJetsToLL_v0_histo.root");
  TFile *fileData1 = TFile::Open("csv_rwt_" + flavor_file + "_DoubleEG_v0_histo.root");
  TFile *fileData2 = TFile::Open("csv_rwt_" + flavor_file + "_DoubleMuon_v0_histo.root");
  TFile *fileData3 = TFile::Open("csv_rwt_" + flavor_file + "_MuonEG_v0_histo.root");


  TString plotName = dirprefix + varName + flavor_file + ".png";

  TString h_var_Name = Form("h_%s",varName.Data());
 
  TH1D* h_var_ttjets = (TH1D*)fileTTJets->Get(h_var_Name.Data())->Clone(h_var_Name+"_ttjets");
  std::cout << "number of ttjets: " << h_var_ttjets->Integral() << std::endl;
  TH1D* h_var_zjets = (TH1D*)fileZJets->Get(h_var_Name.Data())->Clone(h_var_Name+"_zjets");
  std::cout << "number of zjets: " << h_var_zjets->Integral() << std::endl;
  TH1D* h_var_2 = (TH1D*)fileData1->Get(h_var_Name.Data())->Clone(h_var_Name+"_DoubleEG");
  TH1D* h_var_3 = (TH1D*)fileData2->Get(h_var_Name.Data())->Clone(h_var_Name+"_DoubleMuon");
  TH1D* h_var_4 = (TH1D*)fileData3->Get(h_var_Name.Data())->Clone(h_var_Name+"_MuonEG");

  TH1D* h_var_data = (TH1D*)h_var_2->Clone(h_var_Name+"_data");
  h_var_data->Add(h_var_3);
  h_var_data->Add(h_var_4);

  std::cout << "number of data: " << h_var_data->Integral() << std::endl;
  ////
  if( !varName.Contains ("flavour") ){
    h_var_data->Rebin(4);
    h_var_ttjets->Rebin(4);
    h_var_zjets->Rebin(4);
  }
  h_var_data->SetStats(0);

  h_var_data->SetMarkerStyle(20);
  
  h_var_ttjets->SetFillColor(kRed);
  h_var_zjets->SetFillColor(kGreen+1);
  
  h_var_ttjets->SetLineColor(kRed);
  h_var_zjets->SetLineColor(kGreen+1);
  
  h_var_data->SetLineWidth(2);
  h_var_ttjets->SetLineWidth(2);
  h_var_zjets->SetLineWidth(2);

  //
  THStack *hs = new THStack("hs","");
  hs->Add(h_var_ttjets);
  hs->Add(h_var_zjets);

  
  TH1D* h_var_mc = (TH1D*)h_var_ttjets->Clone(h_var_Name+"_mc");
  h_var_mc->Add(h_var_zjets);

  TH1D* myRatio = (TH1D*)h_var_data->Clone(h_var_Name+"_ratio");
  myRatio->Divide(h_var_mc);

  ////
  TLegend *legend = new TLegend(0.14,0.93,0.9,0.98);
  
  legend->SetFillColor(kWhite);
  legend->SetLineColor(kWhite);
  legend->SetShadowColor(kWhite);
  legend->SetTextFont(42);
  legend->SetTextSize(0.05);
  
  legend->SetNColumns(3);
  
  legend->AddEntry(h_var_ttjets,"ttjets","l");
  legend->AddEntry(h_var_zjets,"zjets","l");
  legend->AddEntry(h_var_data,"data","l");
  

  // TString title    = varName;
  
  // TLatex BinInfoLatex(0.6, 0.86, title.Data());
	
  // BinInfoLatex.SetNDC();
  // BinInfoLatex.SetTextFont(42);
  // BinInfoLatex.SetTextSize(0.04);

    //Hack to get it plotted with ratio plot
    TCanvas* myC = new TCanvas("myC", "myC", 600,700);
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);
    Float_t small = 1.e-5;
    myC->Divide(1,2,small,small);
    const float padding=1e-5; const float ydivide=0.3;
    myC->GetPad(1)->SetPad( padding, ydivide + padding , 1-padding, 1-padding);
    myC->GetPad(2)->SetPad( padding, padding, 1-padding, ydivide-padding);
    myC->GetPad(1)->SetRightMargin(.05);
    myC->GetPad(2)->SetRightMargin(.05);
    myC->GetPad(1)->SetBottomMargin(.4);
    myC->GetPad(2)->SetBottomMargin(.4);
    myC->GetPad(1)->Modified();
    myC->GetPad(2)->Modified();
    myC->cd(1);
    gPad->SetBottomMargin(small);
    gPad->Modified();


    myC->cd(1);
  h_var_data->Draw("pe1");
  hs->Draw("histsame");
  h_var_data->Draw("pe1same");
  
  legend->Draw();
  // BinInfoLatex.Draw();
  
  //-----------
  myC->cd(2);
    gPad->SetTopMargin(small);
    gPad->SetTickx();
    gPad->Modified();

  double ratioMax = 1.5;
  double ratioMin = 0.5;
  
  // double xmin = h_data_all->GetBinLowEdge(1);
  // double xmax = h_data_all->GetBinLowEdge(nbins) + h_data_all->GetBinWidth(nbins);
  
  
  // TH1D* myRatio = new TH1D("ratio", "", nbins, xmin, xmax );

  myRatio->SetStats(0);
  // myRatio->Sumw2();
  myRatio->SetLineColor(kBlack);
  myRatio->SetMarkerColor(kBlack);
  //myRatio->Divide(hist[bin_one],hist[bin_two]);
  
  myRatio->SetMinimum(ratioMin);
  myRatio->SetMaximum(ratioMax);
  myRatio->GetYaxis()->SetNdivisions(50000+204);
  myRatio->GetYaxis()->SetLabelSize(0.1); //make y label bigger
  myRatio->GetXaxis()->SetLabelSize(0.1); //make y label bigger
  myRatio->GetXaxis()->SetTitleOffset(1.1);
  myRatio->GetXaxis()->SetTitle(h_var_data->GetXaxis()->GetTitle()); //make y label bigger
  myRatio->GetXaxis()->SetLabelSize(0.12);
  myRatio->GetXaxis()->SetLabelOffset(0.04);
  myRatio->GetXaxis()->SetTitleSize(0.12);
  myRatio->GetYaxis()->SetTitle("Data/MC");
  myRatio->GetYaxis()->SetTitleSize(0.09);
  myRatio->GetYaxis()->SetTitleOffset(.55);
  myRatio->GetYaxis()->CenterTitle();

  myRatio->Draw("pe1");
  
  
  TLine* myLine;
  myLine = new TLine(myRatio->GetXaxis()->GetXmin(), 1, myRatio->GetXaxis()->GetXmax(), 1);
  myLine->Draw();

  ///---------
  myC->RedrawAxis();

      // plotName = dirprefix + varName + ".png";

  myC->Print(plotName.Data());

  ///
  delete myC;

  std::cout << "Done." << std::endl;

  }
}
