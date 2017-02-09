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
void sfCompare_RunDependent(bool isCSV = 1, bool isHF = true, TString dirPostFix = "" ) {

  TString BTagger = "csv";
  if (!isCSV) BTagger = "cMVA";

  TH1::SetDefaultSumw2();

  TString dirprefix = "SFCompare_1stFeb_varImages_" + BTagger + dirPostFix + "/";

  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);


  /////
  // int nCSVBins = 18; //Number of bins 
  // double xBins_hf[19] = {-0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};


  // if(!isHF) nCSVBins = 21;
  // double xBins_lf[22] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  vector<TString> var_list;
  // var_list.push_back("probe_jet_csv");
  var_list.push_back("nJets");
  var_list.push_back("nTags");
  var_list.push_back("all_jet_csv");

  // var_list.push_back("probe_jet_csv_noSF");
  // var_list.push_back("nJets_noSF");
  // var_list.push_back("nTags_noSF");
  // var_list.push_back("all_jet_csv_noSF");

  // // var_list.push_back("nJets30");
  // // var_list.push_back("nJets30_noSF");

  // var_list.push_back("probe_jet_pt");
  var_list.push_back("all_jet_pt");

  var_list.push_back("first_jet_pt");
  var_list.push_back("first_jet_eta");
  var_list.push_back("first_jet_csv");
  var_list.push_back("first_jet_flavour");
  // // // var_list.push_back("first_jet_partonflavour");


  var_list.push_back("second_jet_pt");
  var_list.push_back("second_jet_eta");
  var_list.push_back("second_jet_csv");
  var_list.push_back("second_jet_flavour");
  // // // var_list.push_back("second_jet_partonflavour");

  var_list.push_back("met_pt");
  var_list.push_back("mht_pt");
  var_list.push_back("dr_leplep");
  var_list.push_back("mass_leplep");

  var_list.push_back("1stlep_pt");
  var_list.push_back("2ndlep_pt");
  var_list.push_back("lep_pt");


  // var_list.push_back("numPV");




  TString dirStr = "";//"histoFiles_tHFmLF/"; /// directory change   _ge2jNoTagCut
  TString flavor_file = (isHF) ? "hf" : "lf"; 
  flavor_file.ToLower();


  TString end_str1 = "MC_v5_histo_All_BtoF.root"; //// file name change
  TString end_str2 = "MC_v5_histo_All_GH.root"; //// file name change
  TString end_str3 = "MC_v5_histo_All_full.root"; //// file name change
  TFile *fileData1 = TFile::Open(dirStr+BTagger+"_rwt_" + flavor_file + "_" + end_str1);
  TFile *fileData2 = TFile::Open(dirStr+BTagger+"_rwt_" + flavor_file + "_" + end_str2);
  TFile *fileData3 = TFile::Open(dirStr+BTagger+"_rwt_" + flavor_file + "_" + end_str3);


  ////
  TString lumiinfo = "36.81 fb^{-1} (13 TeV)";
  TLatex LumiInfoLatex(0.70, 0.91, lumiinfo);
  LumiInfoLatex.SetNDC(); LumiInfoLatex.SetTextFont(42);
  LumiInfoLatex.SetTextSize(0.04);

  //TString cmsinfo =   "CMS Preliminary";
  TString cmsinfo =   "CMS";
  TLatex CMSInfoLatex(0.155, 0.91, cmsinfo);
  CMSInfoLatex.SetNDC(); CMSInfoLatex.SetTextFont(42);
  CMSInfoLatex.SetTextFont(61);
  CMSInfoLatex.SetTextSize(0.055); //SBOUTLE

  std::string publishinfo =   "Preliminary"; //DPUIGH
  TLatex PublishInfoLatex(0.25, 0.91, publishinfo.c_str()); //SBOUTLE
  PublishInfoLatex.SetNDC();
  PublishInfoLatex.SetTextFont(52);
  PublishInfoLatex.SetTextSize(0.045); //SBOUTLE
  ///

  //--
  double lumi1 =20.236;
  double lumi2 =16.578;
  double lumi3 = lumi1 + lumi2;

  ///////
  for (int i=0; i< var_list.size(); i++){
    TString varName = var_list[i];

  TString plotName = dirprefix + BTagger +"_"+ varName + flavor_file + ".png";
  // TString plotName = dirprefix + BTagger +"_"+ varName + flavor_file + ".pdf";

  TString h_var_Name = Form("h_%s",varName.Data());

  // TH1D* h_var_ttjets = (TH1D*)fileTTJets->Get(h_var_Name.Data())->Clone(h_var_Name+"_ttjets");
  // int nBins = h_var_ttjets->GetNbinsX();
  // double norm_ttjets = h_var_ttjets->Integral( 0, 1+nBins );

  // double norm_mc = norm_ttjets + norm_zjets + norm_tW + norm_WW + norm_lowMasszjets;


  TH1D* h_var_1 = (TH1D*)fileData1->Get(h_var_Name.Data())->Clone(h_var_Name+"_BtoF");
  TH1D* h_var_2 = (TH1D*)fileData2->Get(h_var_Name.Data())->Clone(h_var_Name+"_GH");
  TH1D* h_var_3 = (TH1D*)fileData3->Get(h_var_Name.Data())->Clone(h_var_Name+"_full");
  int nBins = h_var_1->GetNbinsX();

  // style format
  h_var_3->SetStats(0);

  h_var_3->SetMarkerStyle(20);
  

  h_var_1->SetFillColor(kRed);
  h_var_2->SetFillColor(kGreen+3);
  
  h_var_1->SetLineColor(kRed);
  h_var_2->SetLineColor(kGreen+3);
  
  h_var_1->SetLineWidth(2);
  h_var_2->SetLineWidth(2);
  h_var_3->SetLineWidth(2);

  //// rebin
  if( !varName.Contains ("flavour") && !varName.Contains ("nJets") && !varName.Contains ("nTags") 
      && !varName.Contains ("mass_leplep") ){ //&& !varName.Contains ("lep_pt") ){
    h_var_1->Rebin(4);
    h_var_2->Rebin(4);
    h_var_3->Rebin(4);

    if(varName.Contains ("csv") && !isCSV){
      h_var_1->Rebin(2);
      h_var_2->Rebin(2);
      h_var_3->Rebin(2);
    }
  }


  ////
  TLegend *legend = new TLegend(0.14,0.78,0.9,0.88);//new TLegend(0.14,0.93,0.9,0.98);
  
  legend->SetFillColor(kWhite);
  legend->SetLineColor(kWhite);
  legend->SetShadowColor(kWhite);
  legend->SetTextFont(42);
  legend->SetTextSize(0.05);
  
  legend->SetNColumns(3);
  
  legend->AddEntry(h_var_1,"BtoF","f");
  legend->AddEntry(h_var_2,"GH","f");
  // legend->AddEntry(h_var_lowMasszjets,"lowM zjets","f");
  // legend->AddEntry(h_var_tW,"tW","f");
  // legend->AddEntry(h_var_WW,"WW","f");

  // legend->AddEntry(h_var_data,"data","le");
  legend->AddEntry(h_var_3,"full","le");
  

  // TString title    = varName;
  
  // TLatex BinInfoLatex(0.6, 0.86, title.Data());
	
  // BinInfoLatex.SetNDC();
  // BinInfoLatex.SetTextFont(42);
  // BinInfoLatex.SetTextSize(0.04);


  ///--
  if( varName.Contains ("jet_pt") || varName.Contains ("nJets") || varName.Contains ("nTags") 
      || varName.Contains ("jet_csv") ){ 

    TCanvas* c1 = new TCanvas("c1","c1");//,600,500);
    
    c1->SetTopMargin(0.08);
    c1->SetRightMargin(0.08);
    
    c1->cd();
    h_var_3->SetMaximum( 1.3*h_var_3->GetMaximum() );
    h_var_3->Draw("pe1");
    h_var_1->Draw("pe1same");
    h_var_2->Draw("pe1same");
    legend->Draw();
    
    TString compare_plotName = dirprefix + "compare_" + BTagger +"_"+ varName + flavor_file + ".png";
    c1->Print(compare_plotName);
    delete c1;

  }

  ////
  h_var_1->Scale(lumi1/lumi3);
  h_var_2->Scale(lumi2/lumi3);




  //
  THStack *hs = new THStack("hs","");
  hs->Add(h_var_1);
  hs->Add(h_var_2);

  
  TH1D* h_var_avg = (TH1D*)h_var_1->Clone(h_var_Name+"_avg");
  h_var_avg->Add(h_var_2);

  TH1D* myRatio = (TH1D*)h_var_3->Clone(h_var_Name+"_ratio");
  myRatio->Divide(h_var_avg);


  //---
  double norm_avg = h_var_avg->Integral( 0, 1+nBins );
  double norm_full = h_var_3->Integral( 0, 1+nBins );

  if (varName.Contains ("csv") ){
    std::cout << "---------------------------------: " << std::endl;

    // std::cout << "number of ttjets: " << norm_ttjets << "; fraction is " << norm_ttjets/norm_mc << std::endl;
    std::cout << "---number of lumi averaged: " << norm_avg << std::endl;
    
    std::cout << "number of integrated: " << norm_full << ";  integrated/averaged ratio is " << norm_full/norm_avg << std::endl;
    std::cout << "---------------------------------: " << std::endl;
  }


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
    if(varName.Contains ("csv") && !isCSV) gPad-> SetLogy();
    gPad->SetBottomMargin(small);
    gPad->Modified();


    myC->cd(1);
    h_var_3->SetMaximum(1.3*TMath::Max(h_var_3->GetMaximum(), hs->GetMaximum()));

    h_var_3->Draw("pe1");
    hs->Draw("histsame");
    h_var_3->Draw("pe1same");
  
  legend->Draw();

      LumiInfoLatex.Draw();
      CMSInfoLatex.Draw();
      PublishInfoLatex.Draw();

  gPad->RedrawAxis();
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
  myRatio->GetXaxis()->SetTitle(h_var_3->GetXaxis()->GetTitle()); //make y label bigger
  myRatio->GetXaxis()->SetLabelSize(0.12);
  myRatio->GetXaxis()->SetLabelOffset(0.04);
  myRatio->GetXaxis()->SetTitleSize(0.12);
  //  myRatio->GetYaxis()->SetTitle("Data/MC");
  myRatio->GetYaxis()->SetTitle("ratio");
  myRatio->GetYaxis()->SetTitleSize(0.09);
  myRatio->GetYaxis()->SetTitleOffset(.55);
  myRatio->GetYaxis()->CenterTitle();
  TString newXTitle = myRatio->GetXaxis()->GetTitle();
  if(varName.Contains("csv"))   {
    newXTitle += "v2";
    myRatio->GetXaxis()->SetTitle(newXTitle);
  }
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

  //////// PU reweighting
  // if (varName.Contains ("numPV")){

  //   TFile histofile("PileUPReweighting.root","recreate");
  //   histofile.cd();

  //   h_var_2->SetLineColor(kBlue);
  //   h_var_3->SetLineColor(kOrange-3);
  //   // h_var_4->SetLineColor(kMagenta);

  //   h_var_ttjets->Write();
  //   h_var_zjets->Write();
  //   h_var_tW->Write();
  //   h_var_2->Write();
  //   h_var_3->Write();
  //   h_var_4->Write();

  //   ///    
  //   h_var_ttjets->Scale(1./h_var_ttjets->Integral(0, 1+nBins));
  //   h_var_zjets->Scale(1./h_var_zjets->Integral(0, 1+nBins));
  //   h_var_tW->Scale(1./h_var_tW->Integral(0, 1+nBins));

  //   h_var_2->Scale(1./h_var_2->Integral(0, 1+nBins));
  //   h_var_3->Scale(1./h_var_3->Integral(0, 1+nBins));
  //   h_var_4->Scale(1./h_var_4->Integral(0, 1+nBins));

  //   TCanvas* myCan = new TCanvas("myCan", "myCan", 600,500);
  //   myCan->cd();

  //   h_var_2->Draw();
  //   h_var_3->Draw("same");
  //   h_var_4->Draw("same");

  //   h_var_ttjets->Draw("same");
  //   h_var_zjets->Draw("same");
  //   h_var_tW->Draw("same");

  //   myCan->Print("numPV_compare.png");

  //   ///
  //   h_var_mc->Scale(1./h_var_mc->Integral(0, 1+nBins));
  //   h_var_data->Scale(1./h_var_data->Integral(0, 1+nBins));

  //   TH1D* myPU = (TH1D*)h_var_data->Clone(h_var_Name+"_PUratio");
  //   myPU->Divide(h_var_mc);


  //   myPU->Write();

  // }


  std::cout << "Done." << std::endl;

  }
}
