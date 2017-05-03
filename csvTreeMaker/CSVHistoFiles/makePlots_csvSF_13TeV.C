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

void makePlots_csvSF_13TeV( TString inputFileName  = "infile.root", bool isHF = true, bool isCSV = true, TString dirPostFix = "", bool compareIterations = false ) {
  compareIterations = true;

  TString taggerName = "CSVv2";
  if(!isCSV) taggerName = "cMVAv2";

  TH1::SetDefaultSumw2();

  TFile *histFile = TFile::Open(inputFileName);

  if( compareIterations ) dirPostFix = dirPostFix + "Comparison";
  TString dirprefix = taggerName +"_SFPlots_2017_3_30_13TeV" + dirPostFix + "/";

  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);

  // single jet specific plots
  int nPt = 5;//6;
  int nEta = 1;
  TString flavor = "HF";
  if ( !isHF ){
    nPt = 4; nEta = 3;
    flavor = "LF";
  }

  //////
  TH1D* h_Data_jet_csv[nPt][nEta];
  TH1D* h_MC_b_jet_csv[nPt][nEta];
  TH1D* h_MC_nonb_jet_csv[nPt][nEta];

  /////
  // int nCSVBins = 18; //Number of bins 
  // // double xBins_hf[19] = {-0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};
  // double xBins_hf[19] = {-0.04, 0.0, 0.3025, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 0.906, 0.922, 0.938, 0.954, 0.970, 0.976, 0.982, 0.988, 0.994, 1.01};

  int nCSVBins = 22; //Number of bins 
  // double xBins_hf[19] = {-10.0, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};
  double xBins_hf[23] = {-0.04, 0.0, 0.101, 0.202, 0.303, 0.404, 0.505, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 0.906, 0.922, 0.938, 0.954, 0.970, 0.976, 0.982, 0.988, 0.994, 1.01};
  double xBins_hf_cMVA[23] = {-1.01, -0.9525, -0.905, -0.8575, -0.81, -0.7625, -0.715, -0.565, -0.415, -0.265, -0.115, 0.035, 0.185, 0.323, 0.461, 0.599, 0.737, 0.875, 0.902, 0.929, 0.956, 0.983, 1.01};

  if(!isHF) nCSVBins = 21;
  // double xBins_lf[22] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  double xBins_lf[22] = {-0.04, 0.0, 0.101, 0.202, 0.303, 0.404, 0.505, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 0.917, 0.944, 0.970, 0.975, 0.980, 0.985, 0.990, 0.995, 1.01};
  double xBins_lf_cMVA[22] = {-1.01, -0.9525, -0.905, -0.8575, -0.81, -0.7625, -0.715, -0.565, -0.415, -0.265, -0.115, 0.035, 0.185, 0.415, 0.645, 0.875, 0.8975, 0.92, 0.9425, 0.965, 0.9875, 1.01};

  //TCanvas* c1 = new TCanvas("c1","c1",600,500);
  TCanvas* c1 = new TCanvas("c1","c1");

  c1->SetTopMargin(0.08);
  c1->SetRightMargin(0.08);

  // gPad->SetTickx(1);
  // gPad->SetTicky(1);

  TString flavor_file = flavor;
  flavor_file.ToLower();

  ///
  // TString lumiinfo = "36.8 fb^{-1} (13 TeV, 25ns)";
  // TLatex LumiInfoLatex(0.65, 0.93, lumiinfo);
  TString lumiinfo = "36 fb^{-1}, #sqrt{s} = 13 TeV, 2016";
  TLatex LumiInfoLatex(0.62, 0.93, lumiinfo);
  LumiInfoLatex.SetNDC(); LumiInfoLatex.SetTextFont(42);
  LumiInfoLatex.SetTextSize(0.04);

  //TString cmsinfo =   "CMS Preliminary";
  TString cmsinfo =   "CMS";
  // TLatex CMSInfoLatex(0.155, 0.93, cmsinfo);
  TLatex CMSInfoLatex(0.155, 0.85, cmsinfo);
  CMSInfoLatex.SetNDC(); CMSInfoLatex.SetTextFont(42);
  CMSInfoLatex.SetTextFont(61);
  CMSInfoLatex.SetTextSize(0.055); //SBOUTLE

  std::string publishinfo =   "Preliminary"; //DPUIGH
  // TLatex PublishInfoLatex(0.25, 0.93, publishinfo.c_str()); //SBOUTLE
  TLatex PublishInfoLatex(0.155, 0.80, publishinfo.c_str()); //SBOUTLE
  PublishInfoLatex.SetNDC();
  PublishInfoLatex.SetTextFont(52);
  PublishInfoLatex.SetTextSize(0.045); //SBOUTLE
  ///

  std::vector<TString> label_ptbin;
  std::vector<TString> label_etabin;

  TString plotName;

  int iHist = -1;
  int minPt = 0;
  for ( int iPt=minPt; iPt<nPt; iPt++){
    for ( int iEta=0; iEta<nEta; iEta++){
      iHist++;
      if(isHF){
	if( iPt==0 )      label_ptbin.push_back("20 < p_{T} < 30 GeV");
	else if( iPt==1 ) label_ptbin.push_back("30 < p_{T} < 40 GeV");
	else if( iPt==2 ) label_ptbin.push_back("40 < p_{T} < 60 GeV");
	else if( iPt==3 ) label_ptbin.push_back("60 < p_{T} < 100 GeV");
	else if( iPt==4 ) label_ptbin.push_back("p_{T} > 100 GeV");
	
	label_etabin.push_back("|#eta| < 2.4");
      }
      else{
	if( iPt==0 )      label_ptbin.push_back("20 < p_{T} < 30 GeV");
	else if( iPt==1 ) label_ptbin.push_back("30 < p_{T} < 40 GeV");
	else if( iPt==2 ) label_ptbin.push_back("40 < p_{T} < 60 GeV");
	else if( iPt==3 ) label_ptbin.push_back("p_{T} > 60 GeV");
	
	if( iEta==0 ) label_etabin.push_back("|#eta| < 0.8");
	else if( iEta==1 ) label_etabin.push_back("0.8 < |#eta| < 1.6");
	else if( iEta==2 ) label_etabin.push_back("1.6 < |#eta| < 2.4");
      }

    if(!compareIterations){
      TString h_Data_Name = Form("h_csv_Data_Pt%i_Eta%i",iPt,iEta);
      TString h_b_Name = Form("h_csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);
      TString h_nonb_Name = Form("h_csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);

      
      h_Data_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_Data_Name.Data());
      h_MC_b_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_b_Name.Data());
      h_MC_nonb_jet_csv[iPt][iEta] = (TH1D*)histFile->Get(h_nonb_Name.Data());

      // rebin
      TH1D* h_csv_data = NULL;
      TH1D* h_csv_mc_b = NULL;
      TH1D* h_csv_mc_nonb = NULL;

      h_csv_data = (TH1D*)h_Data_jet_csv[iPt][iEta];
      h_csv_mc_b = (TH1D*)h_MC_b_jet_csv[iPt][iEta];
      h_csv_mc_nonb = (TH1D*)h_MC_nonb_jet_csv[iPt][iEta];

      // if( isHF ){
      // 	h_csv_data = (TH1D*)h_Data_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_hf );
      // 	h_csv_mc_b = (TH1D*)h_MC_b_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_hf );
      // 	h_csv_mc_nonb = (TH1D*)h_MC_nonb_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_hf );
      // }
      // else{
      // 	h_csv_data = (TH1D*)h_Data_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_lf );
      // 	h_csv_mc_b = (TH1D*)h_MC_b_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_lf );
      // 	h_csv_mc_nonb = (TH1D*)h_MC_nonb_jet_csv[iPt][iEta]->Rebin( nCSVBins, Form("csv_Data_Pt%i_Eta%i_temp",iPt,iEta), xBins_lf );
      // }

      // //// first and last bin; underflow/overflow
      // h_csv_data->SetBinContent(1,h_Data_jet_csv[iPt][iEta]->GetBinContent(1));
      // h_csv_data->SetBinError(1,h_Data_jet_csv[iPt][iEta]->GetBinError(1));

      // h_csv_mc_b->SetBinContent(1,h_MC_b_jet_csv[iPt][iEta]->GetBinContent(1));
      // h_csv_mc_b->SetBinError(1,h_MC_b_jet_csv[iPt][iEta]->GetBinError(1));

      // h_csv_mc_nonb->SetBinContent(1,h_MC_nonb_jet_csv[iPt][iEta]->GetBinContent(1));
      // h_csv_mc_nonb->SetBinError(1,h_MC_nonb_jet_csv[iPt][iEta]->GetBinError(1));

      // h_csv_data->SetBinContent(nCSVBins,h_Data_jet_csv[iPt][iEta]->GetBinContent(nCSVBins) + h_Data_jet_csv[iPt][iEta]->GetBinContent(nCSVBins+1));
      // h_csv_data->SetBinError(nCSVBins,sqrt(pow(h_Data_jet_csv[iPt][iEta]->GetBinError(nCSVBins),2) + pow(h_Data_jet_csv[iPt][iEta]->GetBinError(nCSVBins+1),2)));

      // h_csv_mc_b->SetBinContent(nCSVBins,h_MC_b_jet_csv[iPt][iEta]->GetBinContent(nCSVBins) + h_MC_b_jet_csv[iPt][iEta]->GetBinContent(nCSVBins+1));
      // h_csv_mc_b->SetBinError(nCSVBins,sqrt(pow(h_MC_b_jet_csv[iPt][iEta]->GetBinError(nCSVBins),2) + pow(h_MC_b_jet_csv[iPt][iEta]->GetBinError(nCSVBins+1),2)));

      // h_csv_mc_nonb->SetBinContent(nCSVBins,h_MC_nonb_jet_csv[iPt][iEta]->GetBinContent(nCSVBins) + h_MC_nonb_jet_csv[iPt][iEta]->GetBinContent(nCSVBins+1));
      // h_csv_mc_nonb->SetBinError(nCSVBins,sqrt(pow(h_MC_nonb_jet_csv[iPt][iEta]->GetBinError(nCSVBins),2) + pow(h_MC_nonb_jet_csv[iPt][iEta]->GetBinError(nCSVBins+1),2)));

      // //// normalize MC to data
      // double norm_mc_b = h_csv_mc_b->Integral();
      // double norm_mc_nonb = h_csv_mc_nonb->Integral();
      // h_csv_mc_b->Scale(h_csv_data->Integral() / (norm_mc_b + norm_mc_nonb));
      // h_csv_mc_nonb->Scale(h_csv_data->Integral() / (norm_mc_b + norm_mc_nonb));

      ////
      h_csv_data->SetStats(0);
      //h_csv_data->GetXaxis()->SetRangeUser(0.0001, 1.001);

      h_csv_data->SetTitle(";"+taggerName+" Discriminator;Jets / bin");
      //      h_csv_data->SetTitle(";"+taggerName);

      h_csv_data->SetMarkerStyle(20);

      h_csv_mc_b->SetFillColor(kRed);
      h_csv_mc_nonb->SetFillColor(kBlue);
      // h_csv_mc_nonb->SetFillColor(kGreen+1);

      h_csv_mc_b->SetLineColor(kRed);
      h_csv_mc_nonb->SetLineColor(kBlue);
      // h_csv_mc_nonb->SetLineColor(kGreen+1);

      h_csv_data->SetLineWidth(2);
      h_csv_mc_b->SetLineWidth(2);
      h_csv_mc_nonb->SetLineWidth(2);


      ///
    TString ptselectioninfo = label_ptbin[iHist];

    TLatex PTSELECTIONInfoLatex(0.57, 0.85, ptselectioninfo);
    PTSELECTIONInfoLatex.SetNDC();
    PTSELECTIONInfoLatex.SetTextFont(42);
    PTSELECTIONInfoLatex.SetTextSize(0.04);

    TString etaselectioninfo = label_etabin[iHist];

    TLatex ETASELECTIONInfoLatex(0.57, 0.8, etaselectioninfo);
    ETASELECTIONInfoLatex.SetNDC();
    ETASELECTIONInfoLatex.SetTextFont(42);
    ETASELECTIONInfoLatex.SetTextSize(0.04);

      ///
      // TLegend *legend = new TLegend(0.14,0.85,0.9,0.9);
      TLegend *legend = new TLegend(0.57,0.57,0.87,0.77);

      legend->SetFillColor(kWhite);
      legend->SetLineColor(kWhite);
      legend->SetShadowColor(kWhite);
      legend->SetTextFont(42);
      legend->SetTextSize(0.05);

      legend->SetNColumns(1);

      legend->AddEntry(h_csv_data," Data","pe");
      if( isHF ){
	legend->AddEntry(h_csv_mc_b," b","f");
	legend->AddEntry(h_csv_mc_nonb," udsg + c","f");
      }
      else{
	legend->AddEntry(h_csv_mc_b," b + c","f");
	legend->AddEntry(h_csv_mc_nonb," udsg","f");
      }

      TH1D* h_diff = (TH1D*) h_csv_data->Clone( Form("csv_diff_Pt%i_Eta%i",iPt,iEta) );

      THStack *hs = new THStack("hs","");
      if( isHF ){
	hs->Add(h_csv_mc_b);
	hs->Add(h_csv_mc_nonb);

	h_diff->Add(h_csv_mc_nonb,-1);
      }
      else{
	hs->Add(h_csv_mc_nonb);
	hs->Add(h_csv_mc_b);

	h_diff->Add(h_csv_mc_b,-1);
      }

      TH1D* h_ratio = (TH1D*) h_diff->Clone( Form("csv_ratio_Pt%i_Eta%i",iPt,iEta) );
      if( isHF ) h_ratio->Divide( h_csv_mc_b );
      else       h_ratio->Divide( h_csv_mc_nonb );
      

      TString title    = Form("%s SF Pt%i Eta%i",flavor.Data(),iPt,iEta);

      TLatex BinInfoLatex(0.6, 0.81, title.Data());//(0.6, 0.86, title.Data());
	
      BinInfoLatex.SetNDC();
      BinInfoLatex.SetTextFont(42);
      BinInfoLatex.SetTextSize(0.04);

      h_csv_data->GetYaxis()->SetTitleOffset(1.2);
      // h_csv_data->GetXaxis()->SetRangeUser(-0.041,0.9489);
      h_csv_data->SetMaximum(1.3*TMath::Max(h_csv_data->GetMaximum(), hs->GetMaximum()));
      h_csv_data->Draw("pe1");
      hs->Draw("histsame");
      h_csv_data->Draw("pe1same");
      
      legend->Draw();
      //      BinInfoLatex.Draw();

      LumiInfoLatex.Draw();
      CMSInfoLatex.Draw();
      PublishInfoLatex.Draw();
      ETASELECTIONInfoLatex.Draw();
      PTSELECTIONInfoLatex.Draw();

      c1->RedrawAxis();

      plotName = dirprefix + Form("%s_%s_SF_Pt%i_Eta%i_astack",taggerName.Data(),flavor.Data(),iPt,iEta) + ".pdf";

      c1->Print(plotName.Data());



      // TLegend *legend_diff = new TLegend(0.14,0.85,0.9,0.9);//new TLegend(0.14,0.93,0.9,0.98);
      TLegend *legend_diff = new TLegend(0.57,0.57,0.87,0.77);
      legend_diff->SetFillColor(kWhite);
      legend_diff->SetLineColor(kWhite);
      legend_diff->SetShadowColor(kWhite);
      legend_diff->SetTextFont(42);
      legend_diff->SetTextSize(0.05);

      legend_diff->SetNColumns(1);

      if( isHF ){
	legend_diff->AddEntry(h_diff,"(Data - c, udsg)","pe");
	legend_diff->AddEntry(h_csv_mc_b,"b","l");
	h_diff->SetMaximum(1.3*TMath::Max(h_diff->GetMaximum(), h_csv_mc_b->GetMaximum()));
      }
      else{
	legend_diff->AddEntry(h_diff,"(Data - b, c)","pe");
	legend_diff->AddEntry(h_csv_mc_nonb,"udsg","l");
	h_diff->SetMaximum(1.3*TMath::Max(h_diff->GetMaximum(), h_csv_mc_nonb->GetMaximum()));
      }

      h_diff->GetYaxis()->SetTitleOffset(1.2);
      // h_diff->GetXaxis()->SetRangeUser(-0.041,0.9489);
      h_diff->Draw("pe1");
      if( isHF ) h_csv_mc_b->Draw("pe1same");
      else       h_csv_mc_nonb->Draw("pe1same");
      h_diff->Draw("pe1same");

      legend_diff->Draw();
      //      BinInfoLatex.Draw();

      c1->RedrawAxis();

      LumiInfoLatex.Draw();
      CMSInfoLatex.Draw();
      PublishInfoLatex.Draw();
      ETASELECTIONInfoLatex.Draw();
      PTSELECTIONInfoLatex.Draw();

      plotName = dirprefix + Form("%s_%s_SF_Pt%i_Eta%i_bdiff",taggerName.Data(),flavor.Data(),iPt,iEta) + ".pdf";

      c1->Print(plotName.Data());



      h_ratio->GetYaxis()->SetRangeUser(0.,2.);
      h_ratio->SetTitle(";"+taggerName+" Discriminator;Data/MC SF");
      h_ratio->Draw("pe1");

      //      BinInfoLatex.Draw();

      c1->RedrawAxis();

      LumiInfoLatex.Draw();
      CMSInfoLatex.Draw();
      PublishInfoLatex.Draw();
      ETASELECTIONInfoLatex.Draw();
      PTSELECTIONInfoLatex.Draw();

      plotName = dirprefix + Form("%s_%s_SF_Pt%i_Eta%i_cratio",taggerName.Data(),flavor.Data(),iPt,iEta) + ".png";

      c1->Print(plotName.Data());

    }
      if( compareIterations ){
	TFile *fitFile_iter0 = TFile::Open("../macros/csv_rwt_fit_" + flavor_file + "_v2_final_2017_3_15alt.root");
	TFile *fitFile_iter1 = TFile::Open("../macros/csv_rwt_fit_" + flavor_file + "_v2_final_2017_3_15default.root");
	TFile *fitFile_iter2 = TFile::Open("../macros/csv_rwt_fit_" + flavor_file + "_v2_final_2017_3_29test.root");

	// TFile *fitFile_iter0 = TFile::Open("../data/csv_rwt_fit_" + flavor_file + "_v2.root");
	// TFile *fitFile_iter1 = TFile::Open("../macros/BCDF/csv_rwt_fit_" + flavor_file + "_v2.root");
	// TFile *fitFile_iter2 = TFile::Open("../macros/GH/csv_rwt_fit_" + flavor_file + "_v2.root");



	TString h_iter_Data_Name = Form("h_csv_Data_Pt%i_Eta%i",iPt,iEta);
	TString h_iter_b_Name = Form("h_csv_MC_bjets_Pt%i_Eta%i",iPt,iEta);
	TString h_iter_nonb_Name = Form("h_csv_MC_nonbjets_Pt%i_Eta%i",iPt,iEta);
	TString h_iter_csv_ratio_Name = Form("temp_csv_ratio_Pt%i_Eta%i",iPt,iEta);
 
	TH1D* h_iter_Data_jet_csv_iter0 = (TH1D*)fitFile_iter0->Get(h_iter_Data_Name.Data())->Clone(h_iter_Data_Name+"v0");
	TH1D* h_iter_MC_b_jet_csv_iter0 = (TH1D*)fitFile_iter0->Get(h_iter_b_Name.Data())->Clone(h_iter_b_Name+"v0");
	TH1D* h_iter_MC_nonb_jet_csv_iter0 = (TH1D*)fitFile_iter0->Get(h_iter_nonb_Name.Data())->Clone(h_iter_nonb_Name+"v0");
	
	TH1D* h_iter_Data_jet_csv_iter1 = (TH1D*)fitFile_iter1->Get(h_iter_Data_Name.Data())->Clone(h_iter_Data_Name+"v1");
	TH1D* h_iter_MC_b_jet_csv_iter1 = (TH1D*)fitFile_iter1->Get(h_iter_b_Name.Data())->Clone(h_iter_b_Name+"v1");
	TH1D* h_iter_MC_nonb_jet_csv_iter1 = (TH1D*)fitFile_iter1->Get(h_iter_nonb_Name.Data())->Clone(h_iter_nonb_Name+"v1");

	TH1D* h_iter_Data_jet_csv_iter2 = (TH1D*)fitFile_iter2->Get(h_iter_Data_Name.Data())->Clone(h_iter_Data_Name+"v2");
	TH1D* h_iter_MC_b_jet_csv_iter2 = (TH1D*)fitFile_iter2->Get(h_iter_b_Name.Data())->Clone(h_iter_b_Name+"v2");
	TH1D* h_iter_MC_nonb_jet_csv_iter2 = (TH1D*)fitFile_iter2->Get(h_iter_nonb_Name.Data())->Clone(h_iter_nonb_Name+"v2");

	TH1D* h_iter0 = (TH1D*)h_iter_Data_jet_csv_iter0->Clone(h_iter_csv_ratio_Name+"v0");
	TH1D* h_iter1 = (TH1D*)h_iter_Data_jet_csv_iter1->Clone(h_iter_csv_ratio_Name+"v1");
	TH1D* h_iter2 = (TH1D*)h_iter_Data_jet_csv_iter2->Clone(h_iter_csv_ratio_Name+"v2");
	if( isHF ){
	  h_iter0->Add(h_iter_MC_nonb_jet_csv_iter0, -1);
	  h_iter0->Divide(h_iter_MC_b_jet_csv_iter0);

	  h_iter1->Add(h_iter_MC_nonb_jet_csv_iter1, -1);
	  h_iter1->Divide(h_iter_MC_b_jet_csv_iter1);

	  h_iter2->Add(h_iter_MC_nonb_jet_csv_iter2, -1);
	  h_iter2->Divide(h_iter_MC_b_jet_csv_iter2);
	}
	else {
	  h_iter0->Add(h_iter_MC_b_jet_csv_iter0, -1);
	  h_iter0->Divide(h_iter_MC_nonb_jet_csv_iter0);

	  h_iter1->Add(h_iter_MC_b_jet_csv_iter1, -1);
	  h_iter1->Divide(h_iter_MC_nonb_jet_csv_iter1);

	  h_iter2->Add(h_iter_MC_b_jet_csv_iter2, -1);
	  h_iter2->Divide(h_iter_MC_nonb_jet_csv_iter2);
	}


	TString iter_fit_histo_name = Form("csv_ratio_Pt%d_Eta%d_final",iPt,iEta);

	//TString iter_histo_name = Form("h_csv_ratio_%d",iHist);
	TH1D* h_fit_iter0 = (TH1D*)fitFile_iter0->Get(iter_fit_histo_name)->Clone(iter_fit_histo_name+"v0");
	TH1D* h_fit_iter1 = (TH1D*)fitFile_iter1->Get(iter_fit_histo_name)->Clone(iter_fit_histo_name+"v1");
	TH1D* h_fit_iter2 = (TH1D*)fitFile_iter2->Get(iter_fit_histo_name)->Clone(iter_fit_histo_name+"v2");

	// ///-----------
	// double lumi1 =20.236;
	// double lumi2 =16.578;
	// double lumi3 = lumi1 + lumi2;

	// TH1D* h_iter1_tmp = (TH1D*)h_iter1->Clone();
	// TH1D* h_iter2_tmp = (TH1D*)h_iter2->Clone();
	// h_iter1_tmp->Scale(lumi1/lumi3);
	// h_iter2_tmp->Scale(lumi2/lumi3);
	// h_iter1_tmp->Add(h_iter2_tmp);

	// TH1D* h_fit_iter1_tmp = (TH1D*)h_fit_iter1->Clone();
	// TH1D* h_fit_iter2_tmp = (TH1D*)h_fit_iter2->Clone();
	// h_fit_iter1_tmp->Scale(lumi1/lumi3);
	// h_fit_iter2_tmp->Scale(lumi2/lumi3);
	// h_fit_iter1_tmp->Add(h_fit_iter2_tmp);

	///-----------

	h_fit_iter0->SetLineColor(kRed);
	h_fit_iter1->SetLineColor(kGreen+1);
	h_fit_iter2->SetLineColor(kBlue);

	h_fit_iter0->SetLineWidth(2);
	h_fit_iter1->SetLineWidth(2);
	h_fit_iter2->SetLineWidth(2);

	h_iter0->SetLineColor(kRed);
	h_iter1->SetLineColor(kGreen+1);
	h_iter2->SetLineColor(kBlue);

	h_iter0->SetLineWidth(2);
	h_iter1->SetLineWidth(2);
	h_iter2->SetLineWidth(2);

	h_fit_iter0->SetStats(0);
	h_fit_iter0->GetYaxis()->SetRangeUser(0.,2.);
	h_fit_iter0->GetXaxis()->SetRangeUser(-0.04,0.9535); /// change HF range
	if(isHF) h_fit_iter0->GetXaxis()->SetRangeUser(0.5426,1.01); /// change HF range

	h_fit_iter0->SetTitle(";"+taggerName+";Data/MC SF");

	h_fit_iter0->Draw("hist");
	h_iter0->Draw("pe1same");
	// h_fit_iter1->Draw("histsame");
	// h_iter1->Draw("pe1same");
	h_fit_iter2->Draw("histsame");
	h_iter2->Draw("pe1same");

	// h_fit_iter1_tmp->Draw("histsame");
	// h_iter1_tmp->Draw("pe1same");

	c1->RedrawAxis();

	///-------
      TString title    = Form("%s SF Pt%i Eta%i",flavor.Data(),iPt,iEta);

      TLatex BinInfoLatex(0.6, 0.86, title.Data());
	
      BinInfoLatex.SetNDC();
      BinInfoLatex.SetTextFont(42);
      BinInfoLatex.SetTextSize(0.04);

	///-------

	TLegend *legend_iter = new TLegend(0.14,0.93,0.9,0.98);

	legend_iter->SetFillColor(kWhite);
	legend_iter->SetLineColor(kWhite);
	legend_iter->SetShadowColor(kWhite);
	legend_iter->SetTextFont(42);
	legend_iter->SetTextSize(0.05);

	legend_iter->SetNColumns(3);

	legend_iter->AddEntry(h_iter0,"alt","f");
	// legend_iter->AddEntry(h_iter1_tmp,"lumi weighted","f");
	// legend_iter->AddEntry(h_iter1,"default","f");
	legend_iter->AddEntry(h_iter2,"test","f");

	// legend_iter->AddEntry(h_iter0,"Iter0","f");
	// legend_iter->AddEntry(h_iter1,"Iter1","f");
	// legend_iter->AddEntry(h_iter2,"Iter2","f");

	legend_iter->Draw();
	BinInfoLatex.Draw();

	c1->RedrawAxis();

	plotName = dirprefix + Form("%s_%s_SF_Pt%i_Eta%i_diters",taggerName.Data(),flavor.Data(),iPt,iEta) + ".png";

	c1->Print(plotName.Data());
      }
    }
  }



  std::cout << "Done." << std::endl;

}
