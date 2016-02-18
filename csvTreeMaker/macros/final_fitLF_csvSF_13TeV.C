#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVirtualFitter.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"

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

#include "TMatrixDSym.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

#include "TCanvas.h"

#include "Math/MinimizerOptions.h"

//______________________________________________________________________________
void final_fitLF_csvSF_13TeV( TString inputFileName  = "infile.root", 
			      TString inputFileName_JESUp  = "infile.root", 
			      TString inputFileName_JESDown  = "infile.root", 
			      int iterNum=0, TString dirPostFix = "" ){

  TH1::SetDefaultSumw2();

  TString dirprefix = "Images/Images_2016_02_15_fitLF_csvSF_13TeV" + dirPostFix + "/";

  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);


  TFile *file = TFile::Open(inputFileName);
  TFile *file_JESUp = TFile::Open(inputFileName_JESUp);
  TFile *file_JESDown = TFile::Open(inputFileName_JESDown);

  std::cout << " ###===> iteration version " << iterNum << std::endl;


  std::string histofilename = Form("csv_rwt_fit_lf_v%d_final_2016_02_15" + dirPostFix + ".root",iterNum) ;
  TFile histofile(histofilename.c_str(),"recreate");
  histofile.cd();



  TString lumiinfo = "2.6 fb^{-1} (13 TeV, 25 ns)";
  TLatex LumiInfoLatex(0.67, 0.91, lumiinfo);
  LumiInfoLatex.SetNDC(); LumiInfoLatex.SetTextFont(42);
  LumiInfoLatex.SetTextSize(0.04);

  //TString cmsinfo =   "CMS Preliminary";
  TString cmsinfo =   "CMS";
  // TLatex CMSInfoLatex(0.155, 0.91, cmsinfo);
  TLatex CMSInfoLatex(0.155, 0.83, cmsinfo);
  CMSInfoLatex.SetNDC(); CMSInfoLatex.SetTextFont(42);
  CMSInfoLatex.SetTextFont(61);
  CMSInfoLatex.SetTextSize(0.055); //SBOUTLE

  std::string publishinfo =   "Preliminary"; //DPUIGH
  // TLatex PublishInfoLatex(0.25, 0.91, publishinfo.c_str()); //SBOUTLE
  TLatex PublishInfoLatex(0.155, 0.77, publishinfo.c_str()); //SBOUTLE
  PublishInfoLatex.SetNDC();
  PublishInfoLatex.SetTextFont(52);
  PublishInfoLatex.SetTextSize(0.045); //SBOUTLE


  double useUp = 1.2;
  double useDown = 0.8;

  // int ncsvbins = 17;//21;//17;
  // double csvbins[] = { -10.0, 0.0, 0.040, 0.080, 0.120, 0.160, 0.200, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.010 };
  // double csvbins_new[] = { -0.04, 0.0, 0.040, 0.080, 0.120, 0.160, 0.200, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.010 };
  // double csvbins[] = {-10.0, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  // double csvbins_new[] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.915, 0.932, 0.949, 0.966, 0.983, 1.01};
  // same as last round
  /// --- 8TeV
  // // int ncsvbins = 17;//21;//17;
  // // double csvbins[] = {-10.0, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.01};
  // // double csvbins_new[] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.752, 0.825, 0.898, 0.949, 1.01};
  // // Rebin for stats
  // int ncsvbins = 15;//21;//17;
  // double csvbins[] = {-10.0, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.825, 0.949, 1.01};
  // double csvbins_new[] = {-0.04, 0.0, 0.04, 0.08, 0.12, 0.16, 0.2, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.825, 0.949, 1.01};

  ///------------ 13TeV
  //int ncsvbins = 13;//21;//17;
  //double csvbins[] = {-10.0, 0.0, 0.101, 0.202, 0.303, 0.404, 0.505, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 1.01};
  //double csvbins_new[] = {-0.04, 0.0, 0.101, 0.202, 0.303, 0.404, 0.505, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 1.01};

  int ncsvbins = 17;
  double csvbins[] = {-10.0, 0.0, 0.08, 0.16, 0.24, 0.32, 0.40, 0.460, 0.528, 0.596, 0.664, 0.732, 0.800, 0.845, 0.890, 0.935, 0.968, 1.01};
  double csvbins_new[] = {-0.04, 0.0, 0.08, 0.16, 0.24, 0.32, 0.40, 0.460, 0.528, 0.596, 0.664, 0.732, 0.800, 0.845, 0.890, 0.935, 0.968, 1.01};


  std::vector<TString> bin_name;
  std::vector<TString> hist_name;
  std::vector<TString> data_hist_name;
  std::vector<TString> mc_b_hist_name;
  std::vector<TString> mc_nonb_hist_name;

  std::vector<TString> label_ptbin;
  std::vector<TString> label_etabin;

  int maxPt  = 4;
  int maxEta = 3;

  TString prefix_hist = ( iterNum < 3 ) ? "" : "h_"; 

  for( int iPt=0; iPt<maxPt; iPt++ ){
    for( int iEta=0; iEta<maxEta; iEta++ ){
      hist_name.push_back( Form("csv_ratio_Pt%d_Eta%d", iPt, iEta) );

      bin_name.push_back( Form("Pt%d_Eta%d", iPt, iEta) );

      data_hist_name.push_back( Form("%scsv_Data_Pt%d_Eta%d", prefix_hist.Data(), iPt, iEta) );
      mc_b_hist_name.push_back( Form("%scsv_MC_bjets_Pt%d_Eta%d", prefix_hist.Data(), iPt, iEta) );
      mc_nonb_hist_name.push_back( Form("%scsv_MC_nonbjets_Pt%d_Eta%d", prefix_hist.Data(), iPt, iEta) );

      if( iPt==0 )      label_ptbin.push_back("20 < p_{T} < 30 GeV");
      else if( iPt==1 ) label_ptbin.push_back("30 < p_{T} < 40 GeV");
      else if( iPt==2 ) label_ptbin.push_back("40 < p_{T} < 60 GeV");
      else if( iPt==3 ) label_ptbin.push_back("p_{T} > 60 GeV");

      if( iEta==0 ) label_etabin.push_back("|#eta| < 0.8");
      else if( iEta==1 ) label_etabin.push_back("0.8 < |#eta| < 1.6");
      else if( iEta==2 ) label_etabin.push_back("1.6 < |#eta| < 2.4");
    }
  }

  int NumHists_normal = int( hist_name.size() );
  int numHists = NumHists_normal+2;

  // ofstream fit_result_file[NumHists_normal];
  ofstream *fit_result_file = new ofstream[NumHists_normal];

  TH1D* h_csv_ratio[numHists];
  TH1D* h_csv_ratio_HF[numHists];
  TH1D* h_csv_ratio_HFUp[numHists];
  TH1D* h_csv_ratio_HFDown[numHists];
  TH1D* h_csv_ratio_JESUp[numHists];
  TH1D* h_csv_ratio_JESDown[numHists];
  TH1D* h_csv_ratio_Stats1Up[numHists];
  TH1D* h_csv_ratio_Stats1Down[numHists];
  TH1D* h_csv_ratio_Stats2Up[numHists];
  TH1D* h_csv_ratio_Stats2Down[numHists];

  TH1D* h_csv_data[NumHists_normal];
  TH1D* h_csv_mc_b[NumHists_normal];
  TH1D* h_csv_mc_nonb[NumHists_normal];

  TH1D* h_csv_mc_b_JESUp[NumHists_normal];
  TH1D* h_csv_mc_nonb_JESUp[NumHists_normal];
  TH1D* h_csv_mc_b_JESDown[NumHists_normal];
  TH1D* h_csv_mc_nonb_JESDown[NumHists_normal];


  TH1D* h_csv_data_all = NULL;
  TH1D* h_csv_mc_b_all = NULL;
  TH1D* h_csv_mc_nonb_all = NULL;

  TH1D* h_csv_mc_b_all_JESUp = NULL;
  TH1D* h_csv_mc_nonb_all_JESUp = NULL;

  TH1D* h_csv_mc_b_all_JESDown = NULL;
  TH1D* h_csv_mc_nonb_all_JESDown = NULL;

  for( int iHist=0; iHist<NumHists_normal; iHist++ ){
    TH1D* h_csv_data_temp0 = (TH1D*)file->Get( data_hist_name[iHist] )->Clone( Form("h_%s_temp0",data_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_b_temp0 = (TH1D*)file->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_temp0 = (TH1D*)file->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_nonb_hist_name[iHist].Data()) );

    TH1D* h_csv_data_temp0_rebin = (TH1D*)h_csv_data_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",data_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_b_temp0_rebin = (TH1D*)h_csv_mc_b_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_nonb_temp0_rebin = (TH1D*)h_csv_mc_nonb_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);

    // JES up/down
    TH1D* h_csv_mc_b_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_nonb_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_b_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_nonb_hist_name[iHist].Data()) );

    TH1D* h_csv_mc_b_JESUp_temp0_rebin = (TH1D*)h_csv_mc_b_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_nonb_JESUp_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_b_JESDown_temp0_rebin = (TH1D*)h_csv_mc_b_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_mc_nonb_JESDown_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);


    h_csv_data[iHist] = new TH1D( Form("h_%s",data_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_b[iHist] = new TH1D( Form("h_%s",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb[iHist] = new TH1D( Form("h_%s",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );

    h_csv_mc_b_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_b_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    h_csv_mc_nonb_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );

    for( int iBin=0; iBin<ncsvbins; iBin++ ){
      h_csv_data[iHist]->SetBinContent(iBin+1, h_csv_data_temp0_rebin->GetBinContent(iBin+1));
      h_csv_data[iHist]->SetBinError(iBin+1, h_csv_data_temp0_rebin->GetBinError(iBin+1));

      h_csv_mc_b[iHist]->SetBinContent(iBin+1, h_csv_mc_b_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_b[iHist]->SetBinError(iBin+1, h_csv_mc_b_temp0_rebin->GetBinError(iBin+1));

      h_csv_mc_nonb[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_nonb[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_temp0_rebin->GetBinError(iBin+1));


      h_csv_mc_b_JESUp[iHist]->SetBinContent(iBin+1, h_csv_mc_b_JESUp_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_b_JESUp[iHist]->SetBinError(iBin+1, h_csv_mc_b_JESUp_temp0_rebin->GetBinError(iBin+1));
      h_csv_mc_nonb_JESUp[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_JESUp_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_nonb_JESUp[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_JESUp_temp0_rebin->GetBinError(iBin+1));

      h_csv_mc_b_JESDown[iHist]->SetBinContent(iBin+1, h_csv_mc_b_JESDown_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_b_JESDown[iHist]->SetBinError(iBin+1, h_csv_mc_b_JESDown_temp0_rebin->GetBinError(iBin+1));
      h_csv_mc_nonb_JESDown[iHist]->SetBinContent(iBin+1, h_csv_mc_nonb_JESDown_temp0_rebin->GetBinContent(iBin+1));
      h_csv_mc_nonb_JESDown[iHist]->SetBinError(iBin+1, h_csv_mc_nonb_JESDown_temp0_rebin->GetBinError(iBin+1));
    }

    if( iHist==0 ){
      h_csv_data_all = (TH1D*)h_csv_data[iHist]->Clone("h_csv_data_all");
      h_csv_mc_b_all = (TH1D*)h_csv_mc_b[iHist]->Clone("h_csv_mc_b_all");
      h_csv_mc_nonb_all = (TH1D*)h_csv_mc_nonb[iHist]->Clone("h_csv_mc_nonb_all");

      h_csv_mc_b_all_JESUp = (TH1D*)h_csv_mc_b_JESUp[iHist]->Clone("h_csv_mc_b_all_JESUp");
      h_csv_mc_nonb_all_JESUp = (TH1D*)h_csv_mc_nonb_JESUp[iHist]->Clone("h_csv_mc_nonb_all_JESUp");
      h_csv_mc_b_all_JESDown = (TH1D*)h_csv_mc_b_JESDown[iHist]->Clone("h_csv_mc_b_all_JESDown");
      h_csv_mc_nonb_all_JESDown = (TH1D*)h_csv_mc_nonb_JESDown[iHist]->Clone("h_csv_mc_nonb_all_JESDown");
    }
    else{
      h_csv_data_all->Add(h_csv_data[iHist]);
      h_csv_mc_b_all->Add(h_csv_mc_b[iHist]);
      h_csv_mc_nonb_all->Add(h_csv_mc_nonb[iHist]);

      h_csv_mc_b_all_JESUp->Add(h_csv_mc_b_JESUp[iHist]);
      h_csv_mc_nonb_all_JESUp->Add(h_csv_mc_nonb_JESUp[iHist]);
      h_csv_mc_b_all_JESDown->Add(h_csv_mc_b_JESDown[iHist]);
      h_csv_mc_nonb_all_JESDown->Add(h_csv_mc_nonb_JESDown[iHist]);
    }


    h_csv_ratio[iHist]        = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_%d",iHist));
    h_csv_ratio_HFUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_HFUp_%d",iHist));
    h_csv_ratio_HFDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_HFDown_%d",iHist));

    h_csv_ratio_JESUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESUp_%d",iHist));
    h_csv_ratio_JESDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESDown_%d",iHist));

    ///////
    TH1D* h_csv_mc_b_temp0_HFUp = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_HFUp_%d",iHist));
    TH1D* h_csv_mc_b_temp0_HFDown = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_HFDown_%d",iHist));

    TH1D* h_csv_mc_nonb_temp0_HFUp = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_HFUp_%d",iHist));
    TH1D* h_csv_mc_nonb_temp0_HFDown = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_HFDown_%d",iHist));


    ////////////////
    double data_integral = h_csv_data[iHist]->Integral();
    double mc_b_integral = h_csv_mc_b[iHist]->Integral();
    double mc_nonb_integral = h_csv_mc_nonb[iHist]->Integral();

    h_csv_mc_b[iHist]->Scale( data_integral / ( mc_b_integral + mc_nonb_integral ) );
    h_csv_mc_nonb[iHist]->Scale( data_integral / ( mc_b_integral + mc_nonb_integral ) );

    /////

    double mc_b_temp0_HFUp_integral = useUp*h_csv_mc_b[iHist]->Integral();
    double mc_b_temp0_HFDown_integral = useDown*h_csv_mc_b[iHist]->Integral();

    h_csv_mc_b_temp0_HFUp->Scale( data_integral / ( mc_b_temp0_HFUp_integral + mc_nonb_integral ) );
    h_csv_mc_b_temp0_HFDown->Scale( data_integral / ( mc_b_temp0_HFDown_integral + mc_nonb_integral ) );

    h_csv_mc_nonb_temp0_HFUp->Scale( data_integral / ( mc_b_temp0_HFUp_integral + mc_nonb_integral ) );
    h_csv_mc_nonb_temp0_HFDown->Scale( data_integral / ( mc_b_temp0_HFDown_integral + mc_nonb_integral ) );

    /////

    double mc_b_JESUp_integral = h_csv_mc_b_JESUp[iHist]->Integral();
    double mc_nonb_JESUp_integral = h_csv_mc_nonb_JESUp[iHist]->Integral();

    double mc_b_JESDown_integral = h_csv_mc_b_JESDown[iHist]->Integral();
    double mc_nonb_JESDown_integral = h_csv_mc_nonb_JESDown[iHist]->Integral();

    h_csv_mc_b_JESUp[iHist]->Scale( data_integral / ( mc_b_JESUp_integral + mc_nonb_JESUp_integral ) );
    h_csv_mc_b_JESDown[iHist]->Scale( data_integral / ( mc_b_JESDown_integral + mc_nonb_JESDown_integral ) );

    h_csv_mc_nonb_JESUp[iHist]->Scale( data_integral / ( mc_b_JESUp_integral + mc_nonb_JESUp_integral ) );
    h_csv_mc_nonb_JESDown[iHist]->Scale( data_integral / ( mc_b_JESDown_integral + mc_nonb_JESDown_integral ) );

    ///////

    h_csv_ratio[iHist]->Add(h_csv_mc_b[iHist],-1);
    h_csv_ratio_HFUp[iHist]->Add(h_csv_mc_b_temp0_HFUp,-useUp);
    h_csv_ratio_HFDown[iHist]->Add(h_csv_mc_b_temp0_HFDown,-useDown);
    h_csv_ratio_JESUp[iHist]->Add(h_csv_mc_b_JESUp[iHist],-1);
    h_csv_ratio_JESDown[iHist]->Add(h_csv_mc_b_JESDown[iHist],-1);

    h_csv_ratio[iHist]->Divide(h_csv_mc_nonb[iHist]);
    h_csv_ratio_HFUp[iHist]->Divide(h_csv_mc_nonb_temp0_HFUp);
    h_csv_ratio_HFDown[iHist]->Divide(h_csv_mc_nonb_temp0_HFDown);
    h_csv_ratio_JESUp[iHist]->Divide(h_csv_mc_nonb_JESUp[iHist]);
    h_csv_ratio_JESDown[iHist]->Divide(h_csv_mc_nonb_JESDown[iHist]);



    /// NEW stats 1 2 up down
    h_csv_ratio_Stats1Up[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_Stats1Up_%d",iHist));
    h_csv_ratio_Stats1Down[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_Stats1Down_%d",iHist));

    h_csv_ratio_Stats2Up[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_Stats2Up_%d",iHist));
    h_csv_ratio_Stats2Down[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_Stats2Down_%d",iHist));

    int Nbins = h_csv_data[iHist]->GetNbinsX();
    for( int iBin=0; iBin<Nbins; iBin++ ){
      double center = h_csv_data[iHist]->GetBinCenter(iBin+1);
      double content = h_csv_data[iHist]->GetBinContent(iBin+1);

      double delta = h_csv_data[iHist]->GetBinError(iBin+1);
      double stat1Up   = content + delta * ( 1 - 2*center );
      double stat1Down = content + delta * ( 2*center - 1 );

      double stat2Up   = content + delta * ( 1 - 6*center*(1-center) );
      double stat2Down = content - delta * ( 1 - 6*center*(1-center) );

      h_csv_ratio_Stats1Up[iHist]->SetBinContent(iBin+1,stat1Up);
      h_csv_ratio_Stats1Down[iHist]->SetBinContent(iBin+1,stat1Down);

      h_csv_ratio_Stats2Up[iHist]->SetBinContent(iBin+1,stat2Up);
      h_csv_ratio_Stats2Down[iHist]->SetBinContent(iBin+1,stat2Down);
    }

    h_csv_ratio_Stats1Up[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats1Up[iHist]->Integral() );
    h_csv_ratio_Stats1Down[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats1Down[iHist]->Integral() );
    h_csv_ratio_Stats2Up[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats2Up[iHist]->Integral() );
    h_csv_ratio_Stats2Down[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats2Down[iHist]->Integral() );

    h_csv_ratio_Stats1Up[iHist]->Add(h_csv_mc_b[iHist],-1);
    h_csv_ratio_Stats1Down[iHist]->Add(h_csv_mc_b[iHist],-1);
    h_csv_ratio_Stats2Up[iHist]->Add(h_csv_mc_b[iHist],-1);
    h_csv_ratio_Stats2Down[iHist]->Add(h_csv_mc_b[iHist],-1);

    h_csv_ratio_Stats1Up[iHist]->Divide(h_csv_mc_nonb[iHist]);
    h_csv_ratio_Stats1Down[iHist]->Divide(h_csv_mc_nonb[iHist]);
    h_csv_ratio_Stats2Up[iHist]->Divide(h_csv_mc_nonb[iHist]);
    h_csv_ratio_Stats2Down[iHist]->Divide(h_csv_mc_nonb[iHist]);

  }


  TH1D* h_csv_ratio_all = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_temp");
  TH1D* h_csv_ratio_all_HFUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_HFUp_temp");
  TH1D* h_csv_ratio_all_HFDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_HFDown_temp");
  TH1D* h_csv_ratio_all_JESUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESUp_temp");
  TH1D* h_csv_ratio_all_JESDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESDown_temp");
  TH1D* h_csv_ratio_cumulative = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative");

  int nBins = h_csv_ratio_cumulative->GetNbinsX();

  TH1D* h_csv_mc_b_all_HFUp = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_HFUp");
  TH1D* h_csv_mc_b_all_HFDown = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_HFDown");

  TH1D* h_csv_mc_nonb_all_HFUp = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_HFUp");
  TH1D* h_csv_mc_nonb_all_HFDown = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_HFDown");


  h_csv_mc_b_all_HFUp->Scale( h_csv_ratio_all->Integral() / (useUp*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );
  h_csv_mc_b_all_HFDown->Scale( h_csv_ratio_all->Integral() / (useDown*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );

  h_csv_mc_nonb_all_HFUp->Scale( h_csv_ratio_all->Integral() / (useUp*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );
  h_csv_mc_nonb_all_HFDown->Scale( h_csv_ratio_all->Integral() / (useDown*h_csv_mc_b_all->Integral() +  h_csv_mc_nonb_all->Integral()) );

  h_csv_ratio_all->Add(h_csv_mc_b_all,-1);
  h_csv_ratio_all_HFUp->Add(h_csv_mc_b_all_HFUp,-useUp);
  h_csv_ratio_all_HFDown->Add(h_csv_mc_b_all_HFDown,-useDown);
  h_csv_ratio_all_JESUp->Add(h_csv_mc_b_all_JESUp,-1);
  h_csv_ratio_all_JESDown->Add(h_csv_mc_b_all_JESDown,-1);

  for( int iBin=0; iBin<nBins; iBin++ ) h_csv_ratio_cumulative->SetBinContent( iBin+1, h_csv_ratio_all->Integral(iBin+1,nBins) );

  h_csv_ratio_all->Divide(h_csv_mc_nonb_all);
  h_csv_ratio_all_HFUp->Divide(h_csv_mc_nonb_all_HFUp);
  h_csv_ratio_all_HFDown->Divide(h_csv_mc_nonb_all_HFDown);
  h_csv_ratio_all_JESUp->Divide(h_csv_mc_nonb_all_JESUp);
  h_csv_ratio_all_JESDown->Divide(h_csv_mc_nonb_all_JESDown);



  TH1D* h_mc_nonb_cumulative = (TH1D*)h_csv_mc_nonb_all->Clone("h_mc_nonb_cumulative");
  TH1D* h_mc_b_cumulative = (TH1D*)h_csv_mc_b_all->Clone("h_mc_b_cumulative");
  for( int iBin=0; iBin<nBins; iBin++ ){
    h_mc_nonb_cumulative->SetBinContent( iBin+1, h_csv_mc_nonb_all->Integral(iBin+1,nBins) );
    h_mc_b_cumulative->SetBinContent( iBin+1, h_csv_mc_b_all->Integral(iBin+1,nBins) );
  }

  
  // for( int iBin=0; iBin<nBins; iBin++ ) printf("\t iBin = %d, numerator = %4.2f, denominator = %4.2f, ratio = %4.2f \n", iBin+1, 
  // 					       h_csv_ratio_cumulative->GetBinContent(iBin+1), h_mc_nonb_cumulative->GetBinContent(iBin+1),
  // 					       h_csv_ratio_cumulative->GetBinContent(iBin+1)/h_mc_nonb_cumulative->GetBinContent(iBin+1) );


  h_csv_ratio_cumulative->Divide(h_mc_nonb_cumulative);


  // for( int iBin=0; iBin<nBins; iBin++ ) printf("\t iBin = %d, cumulative ratio = %4.2f \n ", iBin+1, h_csv_ratio_cumulative->GetBinContent(iBin+1));
  // for( int iBin=0; iBin<nBins; iBin++ ) printf("\t iBin = %d, ratio = %4.2f \n ", iBin+1, h_csv_ratio_all->GetBinContent(iBin+1));

  hist_name.push_back("csv_ratio_all");
  hist_name.push_back("csv_ratio_cumulative_all");


  h_csv_ratio[numHists-2] = (TH1D*)h_csv_ratio_all->Clone( Form("h_%s",hist_name[numHists-2].Data()) );

  h_csv_ratio_HFUp[numHists-2] = (TH1D*)h_csv_ratio_all_HFUp->Clone( Form("h_%s_HFUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_HFDown[numHists-2] = (TH1D*)h_csv_ratio_all_HFDown->Clone( Form("h_%s_HFDown",hist_name[numHists-2].Data()) );

  h_csv_ratio_JESUp[numHists-2] = (TH1D*)h_csv_ratio_all_JESUp->Clone( Form("h_%s_JESUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_JESDown[numHists-2] = (TH1D*)h_csv_ratio_all_JESDown->Clone( Form("h_%s_JESDown",hist_name[numHists-2].Data()) );


  h_csv_ratio[numHists-1] = (TH1D*)h_csv_ratio_cumulative->Clone( Form("h_%s",hist_name[numHists-1].Data()) );
  h_csv_ratio[numHists-1]->SetMarkerStyle(20);




  // for( int iHist=0; iHist<numHists-1; iHist++ ){
  //   h_csv_ratio_Stats1Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Up",hist_name[numHists-2].Data()) );
  //   h_csv_ratio_Stats1Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Down",hist_name[numHists-2].Data()) );
  //   h_csv_ratio_Stats2Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Up",hist_name[numHists-2].Data()) );
  //   h_csv_ratio_Stats2Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Down",hist_name[numHists-2].Data()) );

  //   h_csv_ratio_HF[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_HF",hist_name[numHists-2].Data()) );

  //   for( int iBin=0; iBin<nBins; iBin++ ){
  //     double center = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
  //     double content = h_csv_ratio[iHist]->GetBinContent(iBin+1);
  //     double hfUp = fabs( content - h_csv_ratio_HFUp[iHist]->GetBinContent(iBin+1) );
  //     double hfDown = fabs( content - h_csv_ratio_HFDown[iHist]->GetBinContent(iBin+1) );

  //     double delta = h_csv_ratio[iHist]->GetBinError(iBin+1);
  //     double stat1Up   = content + delta * ( 1 - 2*center );
  //     double stat1Down = content + delta * ( 2*center - 1 );

  //     double stat2Up   = content + delta * ( 1 - 6*center*(1-center) );
  //     double stat2Down = content - delta * ( 1 - 6*center*(1-center) );

  //     double hfUnc = 0.5 * ( hfUp + hfDown );
  //     h_csv_ratio_HF[iHist]->SetBinError(iBin+1,hfUnc);

  //     h_csv_ratio_Stats1Up[iHist]->SetBinContent(iBin+1,stat1Up);
  //     h_csv_ratio_Stats1Down[iHist]->SetBinContent(iBin+1,stat1Down);

  //     h_csv_ratio_Stats2Up[iHist]->SetBinContent(iBin+1,stat2Up);
  //     h_csv_ratio_Stats2Down[iHist]->SetBinContent(iBin+1,stat2Down);
  //   }
  // }




  //TF1* btagSF = new TF1("btagSF","0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396",0.,1.0);

  //// pol5
  //TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x",0,0.95 );

  //// pol6
  //TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x",0,0.95 );
  TF1* f0 = new TF1("f0","[0] + x*([1] + x*([2] + x*([3] + x*([4] + x*([5] + x*[6])))))",0,0.95 );

  //// pol7
  //TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x + [7]*x*x*x*x*x*x",0,0.95 );

  //TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3])",0,0.9 );
  //TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3]) + [4]*x + [5]*x*x + [6]*x*x*x + [7]*x*x*x*x + [8]*x*x*x*x*x",0,0.95 );
  //f0->SetParLimits(8,0,100);


  //TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3])",0,0.679 );
  f0->SetLineColor(kBlack);


  TF1* f0_HFUp = (TF1*)f0->Clone("f0_HFUp");
  TF1* f0_HFDown = (TF1*)f0->Clone("f0_HFDown");

  TF1* f0_JESUp = (TF1*)f0->Clone("f0_JESUp");
  TF1* f0_JESDown = (TF1*)f0->Clone("f0_JESDown");

  TF1* f0_Stats1Up = (TF1*)f0->Clone("f0_Stats1Up");
  TF1* f0_Stats1Down = (TF1*)f0->Clone("f0_Stats1Down");

  TF1* f0_Stats2Up = (TF1*)f0->Clone("f0_Stats2Up");
  TF1* f0_Stats2Down = (TF1*)f0->Clone("f0_Stats2Down");

  f0_JESUp->SetLineColor(kRed);
  f0_JESDown->SetLineColor(kBlue);

  f0_HFUp->SetLineColor(kGreen+3);
  f0_HFDown->SetLineColor(kGreen+3);

  f0_Stats1Up->SetLineColor(kCyan+2);
  f0_Stats1Down->SetLineColor(kCyan+2);


  int nPars = f0->GetNpar();

  int n = 100000;
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(n);


  TH1D* h_csv_ratio_final[numHists];
  TH1D* h_csv_ratio_final_HF[numHists];
  TH1D* h_csv_ratio_final_HFUp[numHists];
  TH1D* h_csv_ratio_final_HFDown[numHists];
  TH1D* h_csv_ratio_final_JESUp[numHists];
  TH1D* h_csv_ratio_final_JESDown[numHists];
  TH1D* h_csv_ratio_final_Stats1Up[numHists];
  TH1D* h_csv_ratio_final_Stats1Down[numHists];
  TH1D* h_csv_ratio_final_Stats2Up[numHists];
  TH1D* h_csv_ratio_final_Stats2Down[numHists];


  double par_vals[numHists][nPars];

  double fit_chi2[numHists];
  int    fit_ndof[numHists];
  double fit_prob[numHists];

  double fit_chi2_HFUp[numHists];
  int    fit_ndof_HFUp[numHists];
  double fit_prob_HFUp[numHists];

  double fit_chi2_HFDown[numHists];
  int    fit_ndof_HFDown[numHists];
  double fit_prob_HFDown[numHists];

  double fit_chi2_JESUp[numHists];
  int    fit_ndof_JESUp[numHists];
  double fit_prob_JESUp[numHists];

  double fit_chi2_JESDown[numHists];
  int    fit_ndof_JESDown[numHists];
  double fit_prob_JESDown[numHists];

  double fit_chi2_Stats1Up[numHists];
  int    fit_ndof_Stats1Up[numHists];
  double fit_prob_Stats1Up[numHists];

  double fit_chi2_Stats1Down[numHists];
  int    fit_ndof_Stats1Down[numHists];
  double fit_prob_Stats1Down[numHists];

  double fit_chi2_Stats2Up[numHists];
  int    fit_ndof_Stats2Up[numHists];
  double fit_prob_Stats2Up[numHists];

  double fit_chi2_Stats2Down[numHists];
  int    fit_ndof_Stats2Down[numHists];
  double fit_prob_Stats2Down[numHists];

  int NumFinalBins = 1000;

  TCanvas *c1 = new TCanvas("c1");
  gPad->SetRightMargin(.05);

  for( int iHist=0; iHist<numHists-1; iHist++ ){
    // delete DELETE
    if( iHist>numHists-3 ) continue;

    h_csv_ratio_final[iHist] = new TH1D( Form("h_%s_final",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_JESUp[iHist] = new TH1D( Form("h_%s_final_JESUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_JESDown[iHist] = new TH1D( Form("h_%s_final_JESDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_HFUp[iHist] = new TH1D( Form("h_%s_final_HFUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_HFDown[iHist] = new TH1D( Form("h_%s_final_HFDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats1Up[iHist] = new TH1D( Form("h_%s_final_Stats1Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats1Down[iHist] = new TH1D( Form("h_%s_final_Stats1Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats2Up[iHist] = new TH1D( Form("h_%s_final_Stats2Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );
    h_csv_ratio_final_Stats2Down[iHist] = new TH1D( Form("h_%s_final_Stats2Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, -0.04, 1.01 );

    for( int iPar=0; iPar<nPars; iPar++ ) f0->SetParameter(iPar,1.);

    TFitResultPtr r = h_csv_ratio[iHist]->Fit(f0,"+mrN0QS");

    fit_chi2[iHist] = f0->GetChisquare();
    fit_ndof[iHist] = f0->GetNDF();
    fit_prob[iHist] = f0->GetProb();


    int Nbins = 10000;
    //Create a histogram to hold the confidence intervals
    TH1D *hint = new TH1D("hint", "Fitted gaussian with .95 conf.band", Nbins, 0, 1.);
    (TVirtualFitter::GetFitter())->GetConfidenceIntervals(hint,0.682689292);
    //Now the "hint" histogram has the fitted function values as the 
    //bin contents and the confidence intervals as bin errors
    hint->SetStats(kFALSE);
    hint->SetFillColor(kYellow);

    TMatrixDSym cov = r->GetCovarianceMatrix();  //  to access the covariance matrix
    Double_t chi2   = r->Chi2(); // to retrieve the fit chi2
    Double_t ndof   = r->Ndf();  // to retrieve the fit NDf
    Double_t par0   = r->Parameter(0); // retrieve the value for the parameter 0
    Double_t err0   = r->ParError(0); // retrieve the error for the parameter 0
    //r->Print("V");     // print full information of fit including covariance matrix

    if( iHist!=numHists-1 ){
      TFitResultPtr r_HFUp = h_csv_ratio_HFUp[iHist]->Fit(f0_HFUp,"+mrNQ0S");
      fit_chi2_HFUp[iHist] = f0_HFUp->GetChisquare();
      fit_ndof_HFUp[iHist] = f0_HFUp->GetNDF();
      fit_prob_HFUp[iHist] = f0_HFUp->GetProb();

      TFitResultPtr r_HFDown = h_csv_ratio_HFDown[iHist]->Fit(f0_HFDown,"+mrNQ0S");
      fit_chi2_HFDown[iHist] = f0_HFDown->GetChisquare();
      fit_ndof_HFDown[iHist] = f0_HFDown->GetNDF();
      fit_prob_HFDown[iHist] = f0_HFDown->GetProb();

      TFitResultPtr r_JESUp = h_csv_ratio_JESUp[iHist]->Fit(f0_JESUp,"+mrNQ0S");
      fit_chi2_JESUp[iHist] = f0_JESUp->GetChisquare();
      fit_ndof_JESUp[iHist] = f0_JESUp->GetNDF();
      fit_prob_JESUp[iHist] = f0_JESUp->GetProb();

      TFitResultPtr r_JESDown = h_csv_ratio_JESDown[iHist]->Fit(f0_JESDown,"+mrNQ0S");
      fit_chi2_JESDown[iHist] = f0_JESDown->GetChisquare();
      fit_ndof_JESDown[iHist] = f0_JESDown->GetNDF();
      fit_prob_JESDown[iHist] = f0_JESDown->GetProb();

      TFitResultPtr r_Stats1Up = h_csv_ratio_Stats1Up[iHist]->Fit(f0_Stats1Up,"+mrNQ0S");
      fit_chi2_Stats1Up[iHist] = f0_Stats1Up->GetChisquare();
      fit_ndof_Stats1Up[iHist] = f0_Stats1Up->GetNDF();
      fit_prob_Stats1Up[iHist] = f0_Stats1Up->GetProb();

      TFitResultPtr r_Stats1Down = h_csv_ratio_Stats1Down[iHist]->Fit(f0_Stats1Down,"+mrNQ0S");
      fit_chi2_Stats1Down[iHist] = f0_Stats1Down->GetChisquare();
      fit_ndof_Stats1Down[iHist] = f0_Stats1Down->GetNDF();
      fit_prob_Stats1Down[iHist] = f0_Stats1Down->GetProb();

      TFitResultPtr r_Stats2Up = h_csv_ratio_Stats2Up[iHist]->Fit(f0_Stats2Up,"+mrNQ0S");
      fit_chi2_Stats2Up[iHist] = f0_Stats2Up->GetChisquare();
      fit_ndof_Stats2Up[iHist] = f0_Stats2Up->GetNDF();
      fit_prob_Stats2Up[iHist] = f0_Stats2Up->GetProb();

      TFitResultPtr r_Stats2Down = h_csv_ratio_Stats2Down[iHist]->Fit(f0_Stats2Down,"+mrNQ0S");
      fit_chi2_Stats2Down[iHist] = f0_Stats2Down->GetChisquare();
      fit_ndof_Stats2Down[iHist] = f0_Stats2Down->GetNDF();
      fit_prob_Stats2Down[iHist] = f0_Stats2Down->GetProb();
    }


    h_csv_ratio_final[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_HFUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_HFDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineWidth(2);


    h_csv_ratio_final[iHist]->SetLineColor(kBlack);
    h_csv_ratio_final_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_final_JESDown[iHist]->SetLineColor(kBlue);
    h_csv_ratio_final_HFUp[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_HFDown[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineColor(kRed-2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineColor(kRed-2);


    double firstPoint = 0.5*(csvbins_new[1] + csvbins_new[2]); //0;
    double lastPoint  = 0.9;
    for( int iBin=0; iBin<n; iBin++ ){
      double center = h_csv_ratio_final[iHist]->GetBinCenter(iBin+1);
      if( center<0 ){
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_HFUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_HFDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
      }
      else if( center>0 && center<firstPoint ){
      	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,f0->Eval( firstPoint ));
      	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,f0_JESUp->Eval( firstPoint ));
      	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,f0_JESDown->Eval( firstPoint ));
      	h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,f0_HFUp->Eval( firstPoint ));
      	h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,f0_HFDown->Eval( firstPoint ));
      	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,f0_Stats1Up->Eval( firstPoint ));
      	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,f0_Stats1Down->Eval( firstPoint ));
      	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,f0_Stats2Up->Eval( firstPoint ));
      	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,f0_Stats2Down->Eval( firstPoint ));
      }
      else if( center>lastPoint ){
      	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,f0->Eval( lastPoint ));
      	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,f0_JESUp->Eval( lastPoint ));
      	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,f0_JESDown->Eval( lastPoint ));
      	h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,f0_HFUp->Eval( lastPoint ));
      	h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,f0_HFDown->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,f0_Stats1Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,f0_Stats1Down->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,f0_Stats2Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,f0_Stats2Down->Eval( lastPoint ));
      }
      else {
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,f0->Eval( center ));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,f0_JESUp->Eval( center ));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,f0_JESDown->Eval( center ));
	h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,f0_HFUp->Eval( center ));
	h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,f0_HFDown->Eval( center ));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,f0_Stats1Up->Eval( center ));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,f0_Stats1Down->Eval( center ));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,f0_Stats2Up->Eval( center ));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,f0_Stats2Down->Eval( center ));
      }
    }


    //////////////
    //////////////

    bool includeCorrectionFactor = !false;
    if( includeCorrectionFactor ){
      if( hist_name[iHist]=="csv_ratio_Pt0_Eta0" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.997050 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.999836 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.992558 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 0.996832 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 0.996076 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.995182 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.998634 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.995495 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.998555 ); 

      	h_csv_ratio[iHist]->Scale( 0.997050 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.999836 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.992558 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 0.996832 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 0.996076 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.995182 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.998634 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.995495 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.998555 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt0_Eta1" ){ 
	h_csv_ratio_final[iHist]->Scale( 1.005871 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 1.006979 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 1.003139 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 1.008865 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 1.006898 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 1.003399 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 1.007968 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 1.003980 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.006749 ); 

      	h_csv_ratio[iHist]->Scale( 1.005871 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 1.006979 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 1.003139 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 1.008865 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 1.006898 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 1.003399 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 1.007968 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 1.003980 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 1.006749 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt0_Eta2" ){ 
	h_csv_ratio_final[iHist]->Scale( 1.006750 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 1.005687 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 1.007100 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 1.006751 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 1.010159 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 1.002933 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 1.010515 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 1.005129 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.008345 ); 

      	h_csv_ratio[iHist]->Scale( 1.006750 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 1.005687 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 1.007100 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 1.006751 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 1.010159 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 1.002933 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 1.010515 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 1.005129 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 1.008345 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt1_Eta0" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.980872 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.988204 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.978004 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 0.987860 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 0.989887 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.977195 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.984518 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.979247 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.982477 ); 

      	h_csv_ratio[iHist]->Scale( 0.980872 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.988204 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.978004 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 0.987860 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 0.989887 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.977195 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.984518 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.979247 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.982477 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt1_Eta1" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.991299 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.992665 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.986839 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 0.997342 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 1.001480 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.986230 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.996424 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.989073 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.993467 ); 

      	h_csv_ratio[iHist]->Scale( 0.991299 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.992665 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.986839 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 0.997342 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 1.001480 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.986230 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.996424 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.989073 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.993467 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt1_Eta2" ){ 
	h_csv_ratio_final[iHist]->Scale( 1.003121 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 1.008096 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.997635 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 1.012836 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 1.014709 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.997198 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 1.009050 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 1.000832 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.005417 ); 

      	h_csv_ratio[iHist]->Scale( 1.003121 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 1.008096 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.997635 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 1.012836 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 1.014709 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.997198 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 1.009050 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 1.000832 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 1.005417 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt2_Eta0" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.991043 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.991960 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.993034 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 1.001645 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 1.005114 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.985811 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.996364 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.988437 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.993656 ); 

      	h_csv_ratio[iHist]->Scale( 0.991043 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.991960 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.993034 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 1.001645 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 1.005114 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.985811 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.996364 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.988437 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.993656 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt2_Eta1" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.978981 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.976599 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.979412 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 0.993288 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 0.992478 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.973485 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.983921 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.975841 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.982100 ); 

      	h_csv_ratio[iHist]->Scale( 0.978981 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.976599 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.979412 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 0.993288 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 0.992478 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.973485 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.983921 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.975841 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.982100 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt2_Eta2" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.994111 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.995555 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.997335 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 1.006864 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 1.006312 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.989901 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.998183 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.992611 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.995555 ); 

      	h_csv_ratio[iHist]->Scale( 0.994111 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.995555 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.997335 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 1.006864 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 1.006312 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.989901 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.998183 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.992611 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.995555 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt3_Eta0" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.972874 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.974001 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.969259 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 0.986257 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 0.989409 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.968452 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.977311 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.971202 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.974525 ); 

      	h_csv_ratio[iHist]->Scale( 0.972874 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.974001 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.969259 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 0.986257 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 0.989409 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.968452 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.977311 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.971202 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.974525 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt3_Eta1" ){ 
	h_csv_ratio_final[iHist]->Scale( 0.979375 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 0.983353 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 0.976933 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 0.986771 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 0.987780 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.974135 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.985024 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.976084 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.982589 ); 

      	h_csv_ratio[iHist]->Scale( 0.979375 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 0.983353 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 0.976933 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 0.986771 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 0.987780 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.974135 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 0.985024 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 0.976084 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 0.982589 ); 
      } 
      if( hist_name[iHist]=="csv_ratio_Pt3_Eta2" ){ 
	h_csv_ratio_final[iHist]->Scale( 1.003502 ); 
	h_csv_ratio_final_JESUp[iHist]->Scale( 1.004260 ); 
	h_csv_ratio_final_JESDown[iHist]->Scale( 1.000385 ); 
	h_csv_ratio_final_HFUp[iHist]->Scale( 1.007816 ); 
	h_csv_ratio_final_HFDown[iHist]->Scale( 1.011638 ); 
	h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.997270 ); 
	h_csv_ratio_final_Stats1Down[iHist]->Scale( 1.010227 ); 
	h_csv_ratio_final_Stats2Up[iHist]->Scale( 1.002912 ); 
	h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.005103 ); 

      	h_csv_ratio[iHist]->Scale( 1.003502 ); 
	h_csv_ratio_JESUp[iHist]->Scale( 1.004260 ); 
	h_csv_ratio_JESDown[iHist]->Scale( 1.000385 ); 
	h_csv_ratio_HFUp[iHist]->Scale( 1.007816 ); 
	h_csv_ratio_HFDown[iHist]->Scale( 1.011638 ); 
	h_csv_ratio_Stats1Up[iHist]->Scale( 0.997270 ); 
	h_csv_ratio_Stats1Down[iHist]->Scale( 1.010227 ); 
	h_csv_ratio_Stats2Up[iHist]->Scale( 1.002912 ); 
	h_csv_ratio_Stats2Down[iHist]->Scale( 1.005103 ); 
      } 
    }


    for( int iPar=0; iPar<nPars; iPar++ ) par_vals[iHist][iPar] = r->Parameter(iPar);

    h_csv_ratio[iHist]->SetMarkerStyle(20);

    h_csv_ratio_final[iHist]->SetTitle( Form("LF %s", hist_name[iHist].Data() ) );
    //h_csv_ratio_final[iHist]->SetTitle("");
    h_csv_ratio_final[iHist]->GetYaxis()->SetTitle("Data/MC scale factor");
    h_csv_ratio_final[iHist]->GetXaxis()->SetTitle("CSVv2 Discriminator");

    h_csv_ratio_final[iHist]->SetStats(0);


    double maxY = 0;
    for( int iBin=0; iBin<h_csv_ratio[iHist]->GetNbinsX()-2; iBin++ ){
      double content = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double err     = h_csv_ratio[iHist]->GetBinError(iBin+1);

      if( maxY<(content+err) ) maxY = content + err;
    }
    maxY *= 1.3;
    //double maxY = 1.3 * h_csv_ratio[iHist]->GetMaximum();
    //double maxY = 1.3 * h_csv_ratio[iHist]->GetBinContent( h_csv_ratio[iHist]->GetNbinsX()-1 );
    maxY = std::min( maxY, 5. );

    h_csv_ratio[iHist]->SetMaximum(maxY);
    h_csv_ratio[iHist]->SetMinimum(0.01);

    h_csv_ratio[iHist]->GetXaxis()->SetRangeUser(-0.041,0.9489);


    h_csv_ratio_final[iHist]->SetMaximum(maxY);
    h_csv_ratio_final[iHist]->SetMinimum(0.01);
    h_csv_ratio_final[iHist]->GetXaxis()->SetRangeUser(-0.041,0.9489);

    TLegend *legend = new TLegend(0.2,0.75,0.77,0.89);

    h_csv_ratio_HFUp[iHist]->SetLineColor(kGreen+1);
    h_csv_ratio_HFDown[iHist]->SetLineColor(kGreen+1);

    h_csv_ratio_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_JESDown[iHist]->SetLineColor(kBlue);

    h_csv_ratio_Stats1Up[iHist]->SetLineColor(kCyan);
    h_csv_ratio_Stats1Down[iHist]->SetLineColor(kCyan);

    h_csv_ratio_Stats2Up[iHist]->SetLineColor(kCyan);
    h_csv_ratio_Stats2Down[iHist]->SetLineColor(kCyan);





    legend->SetFillColor(kWhite);
    legend->SetLineColor(kWhite);
    legend->SetShadowColor(kWhite);
    legend->SetTextFont(42);
    legend->SetTextSize(0.035);

    legend->SetNColumns(3);

    legend->AddEntry(h_csv_ratio[iHist],"LF SF","p");
    legend->AddEntry(h_csv_ratio_final_JESUp[iHist],"JES Up","l");
    legend->AddEntry(h_csv_ratio_final_JESDown[iHist],"JES Down","l");
    legend->AddEntry(h_csv_ratio_final[iHist],"LF Fit","l");
    legend->AddEntry(h_csv_ratio_final_HFUp[iHist],"HF Err","l");
    legend->AddEntry(h_csv_ratio_final_Stats1Up[iHist],"Stats1 Err","l");
    legend->AddEntry(h_csv_ratio_final_Stats2Up[iHist],"Stats2 Err","l");



    //////
    TString ptselectioninfo = label_ptbin[iHist];

    // TLatex PTSELECTIONInfoLatex(0.16, 0.84, ptselectioninfo);
    TLatex PTSELECTIONInfoLatex(0.68, 0.84, ptselectioninfo);
    PTSELECTIONInfoLatex.SetNDC();
    PTSELECTIONInfoLatex.SetTextFont(42);
    PTSELECTIONInfoLatex.SetTextSize(0.04);

    TString etaselectioninfo = label_etabin[iHist];

    // TLatex ETASELECTIONInfoLatex(0.16, 0.78, etaselectioninfo);
    TLatex ETASELECTIONInfoLatex(0.68, 0.78, etaselectioninfo);
    ETASELECTIONInfoLatex.SetNDC();
    ETASELECTIONInfoLatex.SetTextFont(42);
    ETASELECTIONInfoLatex.SetTextSize(0.04);


    ///////


    // TLegend *legend4 = new TLegend(0.2,0.75,0.77,0.89);
    TLegend *legend4 = new TLegend(0.75,0.72,0.87,0.89);
    legend4->SetFillColor(kWhite);
    legend4->SetLineColor(kWhite);
    legend4->SetShadowColor(kWhite);
    legend4->SetTextFont(42);
    legend4->SetTextSize(0.04);

    // legend4->SetNColumns(2);

    legend4->AddEntry(h_csv_ratio[iHist],"LF SF","p");
    legend4->AddEntry(h_csv_ratio_final[iHist],"LF Fit","l");


    h_csv_ratio_final[iHist]->SetTitle("");
    h_csv_ratio_final[iHist]->Draw("pe1");
    h_csv_ratio[iHist]->Draw("pe1same");

    //legend4->Draw();

    LumiInfoLatex.Draw();
    CMSInfoLatex.Draw();
    PublishInfoLatex.Draw();

    PTSELECTIONInfoLatex.Draw();
    ETASELECTIONInfoLatex.Draw();

    TLine *line = new TLine(0, 0.01, 0, maxY);
    line->SetLineStyle(7);

    line->Draw();

    TString img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_only.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_only.pdf";
    c1->Print(img);




    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_JES.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_JES.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_HFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_HFDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_HF.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_HF.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats1.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats1.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats2.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_Stats2.pdf";
    c1->Print(img);


    TH1D* h_ratio_Stats1Up = (TH1D*)h_csv_ratio_Stats1Up[iHist]->Clone("h_ratio_Stats1Up");
    TH1D* h_ratio_Stats1Down = (TH1D*)h_csv_ratio_Stats1Down[iHist]->Clone("h_ratio_Stats1Down");

    h_ratio_Stats1Up->Divide(h_csv_ratio[iHist]);
    h_ratio_Stats1Down->Divide(h_csv_ratio[iHist]);

    TH1D* h_ratio_Stats2Up = (TH1D*)h_csv_ratio_Stats2Up[iHist]->Clone("h_ratio_Stats2Up");
    TH1D* h_ratio_Stats2Down = (TH1D*)h_csv_ratio_Stats2Down[iHist]->Clone("h_ratio_Stats2Down");

    h_ratio_Stats2Up->Divide(h_csv_ratio[iHist]);
    h_ratio_Stats2Down->Divide(h_csv_ratio[iHist]);

    h_ratio_Stats1Up->SetLineColor(kRed);
    h_ratio_Stats1Down->SetLineColor(kBlue);

    h_ratio_Stats2Up->SetLineColor(kMagenta+2);
    h_ratio_Stats2Down->SetLineColor(kGreen+2);


    h_ratio_Stats1Up->SetLineWidth(2);
    h_ratio_Stats1Down->SetLineWidth(2);

    h_ratio_Stats2Up->SetLineWidth(2);
    h_ratio_Stats2Down->SetLineWidth(2);



    TLegend *legend2 = new TLegend(0.2,0.8,0.84,0.89);

    legend2->SetFillColor(kWhite);
    legend2->SetLineColor(kWhite);
    legend2->SetShadowColor(kWhite);
    legend2->SetTextFont(42);
    legend2->SetTextSize(0.035);

    legend2->SetNColumns(2);

    legend2->AddEntry(h_ratio_Stats1Up,"Stats 1 Up","l");
    legend2->AddEntry(h_ratio_Stats1Down,"Stats 1 Down","l");

    h_ratio_Stats1Up->SetTitle(";CSV;Uncertainty/Nominal");
    h_ratio_Stats1Up->SetStats(0);
    h_ratio_Stats1Up->GetYaxis()->SetRangeUser(0.7,1.3);
    h_ratio_Stats1Up->GetXaxis()->SetRangeUser(-0.041,0.9489);

    h_ratio_Stats1Up->Draw("hist");
    h_ratio_Stats1Down->Draw("histsame");
    legend2->Draw();

    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats1.png";
    c1->Print(img);
    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats1.pdf";
    c1->Print(img);



    TLegend *legend3 = new TLegend(0.2,0.8,0.84,0.89);

    legend3->SetFillColor(kWhite);
    legend3->SetLineColor(kWhite);
    legend3->SetShadowColor(kWhite);
    legend3->SetTextFont(42);
    legend3->SetTextSize(0.035);

    legend3->SetNColumns(2);

    legend3->AddEntry(h_ratio_Stats2Up,"Stats 2 Up","l");
    legend3->AddEntry(h_ratio_Stats2Down,"Stats 2 Down","l");

    h_ratio_Stats2Up->SetTitle(";CSV;Uncertainty/Nominal");
    h_ratio_Stats2Up->SetStats(0);
    h_ratio_Stats2Up->GetYaxis()->SetRangeUser(0.7,1.3);
    h_ratio_Stats2Up->GetXaxis()->SetRangeUser(-0.041,0.9489);

    h_ratio_Stats2Up->Draw("hist");
    h_ratio_Stats2Down->Draw("histsame");
    legend3->Draw();

    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats2.png";
    c1->Print(img);
    img = dirprefix + "lfSF_ratio_" + hist_name[iHist] + "_fit_Stats2.pdf";
    c1->Print(img);



    h_csv_ratio_final[iHist]->Draw("hist");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio_final_HFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_HFDown[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_All.png";
    c1->Print(img);
    img = dirprefix + "lfSF_" + hist_name[iHist] + "_fit_All.pdf";
    c1->Print(img);


    // Enforce non-negative weights
    for( int iBin=0; iBin<n; iBin++ ){
      if( h_csv_ratio_final[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_JESUp[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_JESDown[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_HFUp[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_HFUp[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_HFDown[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_HFDown[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats1Up[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats1Down[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats2Up[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats2Down[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,0.);

      if( h_csv_ratio[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_JESUp[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_JESUp[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_JESDown[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_JESDown[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_HFUp[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_HFUp[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_HFDown[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_HFDown[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_Stats1Up[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_Stats1Up[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_Stats1Down[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_Stats1Down[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_Stats2Up[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_Stats2Up[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_Stats2Down[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_Stats2Down[iHist]->SetBinContent(iBin+1,0.);
    }



    //////////////
    /////////////
    
    // double scale_inv = 1.0;
    // if( hist_name[iHist]=="csv_ratio_Pt0_Eta0" )      scale_inv = 9.960210e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt1_Eta0" ) scale_inv = 9.855755e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt2_Eta0" ) scale_inv = 9.810684e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt3_Eta0" ) scale_inv = 9.999028e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt0_Eta1" ) scale_inv = 9.907767e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt1_Eta1" ) scale_inv = 9.892888e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt2_Eta1" ) scale_inv = 1.025452e+00;
    // else if( hist_name[iHist]=="csv_ratio_Pt3_Eta1" ) scale_inv = 9.983725e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt0_Eta2" ) scale_inv = 9.911757e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt1_Eta2" ) scale_inv = 9.927481e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt2_Eta2" ) scale_inv = 9.968454e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt3_Eta2" ) scale_inv = 9.861553e-01;


    // double scale_all = 1.0 / scale_inv;
    
    // h_csv_ratio_final[iHist]->Scale( scale_all );
    // h_csv_ratio_final_JESUp[iHist]->Scale( scale_all );
    // h_csv_ratio_final_JESDown[iHist]->Scale( scale_all );
    // h_csv_ratio_final_HFUp[iHist]->Scale( scale_all );
    // h_csv_ratio_final_HFDown[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats1Up[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats1Down[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats2Up[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats2Down[iHist]->Scale( scale_all );

    
    h_csv_ratio_final[iHist]->Write(Form("%s_final",hist_name[iHist].Data()));
    h_csv_ratio_final_JESUp[iHist]->Write(Form("%s_final_JESUp",hist_name[iHist].Data()));
    h_csv_ratio_final_JESDown[iHist]->Write(Form("%s_final_JESDown",hist_name[iHist].Data()));
    h_csv_ratio_final_HFUp[iHist]->Write(Form("%s_final_HFUp",hist_name[iHist].Data()));
    h_csv_ratio_final_HFDown[iHist]->Write(Form("%s_final_HFDown",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Up[iHist]->Write(Form("%s_final_Stats1Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Down[iHist]->Write(Form("%s_final_Stats1Down",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Up[iHist]->Write(Form("%s_final_Stats2Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Down[iHist]->Write(Form("%s_final_Stats2Down",hist_name[iHist].Data()));


    std::cout << "*********************************************************** " << "\n";
    std::cout << "Bin: " << hist_name[iHist].Data() << "\n";
    std::cout << "\t central = " << f0->GetExpFormula("p") << "\n";
    std::cout << "\t HFUp = " << f0_HFUp->GetExpFormula("p") << "\n";
    std::cout << "\t HFDown = " << f0_HFDown->GetExpFormula("p") << "\n";

    std::cout << "\t JESUp = " << f0_JESUp->GetExpFormula("p") << "\n";
    std::cout << "\t JESDown = " << f0_JESDown->GetExpFormula("p") << "\n";

    std::cout << "\t Stats1Up = " << f0_Stats1Up->GetExpFormula("p") << "\n";
    std::cout << "\t Stats1Down = " << f0_Stats1Down->GetExpFormula("p") << "\n";

    std::cout << "\t Stats2Up = " << f0_Stats2Up->GetExpFormula("p") << "\n";
    std::cout << "\t Stats2Down = " << f0_Stats2Down->GetExpFormula("p") << "\n";
    std::cout << "*********************************************************** " << "\n";


    if( iHist<NumHists_normal ){
      fit_result_file[iHist].open( Form("BTVScaleFactorFitResult/fitResult_LF_csvSF_13TeV_%s.txt", bin_name[iHist].Data()) );
      // fit_result_file[iHist] << "central\t" << f0->GetExpFormula("p") << "\n";
      // fit_result_file[iHist] << "up_hf\t" << f0_HFUp->GetExpFormula("p") << "\n";
      // fit_result_file[iHist] << "down_hf\t" << f0_HFDown->GetExpFormula("p") << "\n";

      // fit_result_file[iHist] << "up_jec\t" << f0_JESUp->GetExpFormula("p") << "\n";
      // fit_result_file[iHist] << "down_jec\t" << f0_JESDown->GetExpFormula("p") << "\n";

      // fit_result_file[iHist] << "up_stats1\t" << f0_Stats1Up->GetExpFormula("p") << "\n";
      // fit_result_file[iHist] << "down_stats1\t" << f0_Stats1Down->GetExpFormula("p") << "\n";

      // fit_result_file[iHist] << "up_stats2\t" << f0_Stats2Up->GetExpFormula("p") << "\n";
      // fit_result_file[iHist] << "down_stats2\t" << f0_Stats2Down->GetExpFormula("p") << "\n";

      TString etaMin = "";
      TString etaMax = "";
      if( hist_name[iHist].Contains("Eta0") ){
	etaMin = "0.0";
	etaMax = "0.8";
      }
      else if( hist_name[iHist].Contains("Eta1") ){
	etaMin = "0.8";
	etaMax = "1.6";
      }
      else if( hist_name[iHist].Contains("Eta2") ){
	etaMin = "1.6";
	etaMax = "2.4";
      }

      TString ptMin = "";
      TString ptMax = "";
      if( hist_name[iHist].Contains("Pt0") ){
	ptMin = "20";
	ptMax = "30";
      }
      else if( hist_name[iHist].Contains("Pt1") ){
	ptMin = "30";
	ptMax = "40";
      }
      else if( hist_name[iHist].Contains("Pt2") ){
	ptMin = "40";
	ptMax = "60";
      }
      else if( hist_name[iHist].Contains("Pt3") ){
	ptMin = "60";
	ptMax = "10000";
      }

      // central
      fit_result_file[iHist] << "3, iterativefit, central, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, central, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, central, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, central, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0->Eval( lastPoint ) << "\"" << "\n";

      // JESUp
      fit_result_file[iHist] << "3, iterativefit, up_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_JESUp[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_JESUp->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_JESUp->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_JESUp->Eval( lastPoint ) << "\"" << "\n";

      // JESDown
      fit_result_file[iHist] << "3, iterativefit, down_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_JESDown[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_JESDown->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_JESDown->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_jes, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_JESDown->Eval( lastPoint ) << "\"" << "\n";


      // HFUp
      fit_result_file[iHist] << "3, iterativefit, up_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_HFUp[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_HFUp->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_HFUp->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_HFUp->Eval( lastPoint ) << "\"" << "\n";

      // HFDown
      fit_result_file[iHist] << "3, iterativefit, down_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_HFDown[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_HFDown->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_HFDown->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_hf, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_HFDown->Eval( lastPoint ) << "\"" << "\n";


      // Stats1Up
      fit_result_file[iHist] << "3, iterativefit, up_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_Stats1Up[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_Stats1Up->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_Stats1Up->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_Stats1Up->Eval( lastPoint ) << "\"" << "\n";

      // Stats1Down
      fit_result_file[iHist] << "3, iterativefit, down_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_Stats1Down[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_Stats1Down->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_Stats1Down->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_lfstats1, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_Stats1Down->Eval( lastPoint ) << "\"" << "\n";


      // Stats2Up
      fit_result_file[iHist] << "3, iterativefit, up_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_Stats2Up[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_Stats2Up->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_Stats2Up->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, up_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_Stats2Up->Eval( lastPoint ) << "\"" << "\n";

      // Stats2Down
      fit_result_file[iHist] << "3, iterativefit, down_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "-15" << ", " << "0" << ", " 
			     << "\"" << h_csv_ratio_Stats2Down[iHist]->GetBinContent(1) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << "0" << ", " << firstPoint << ", " 
			     << "\"" << f0_Stats2Down->Eval( firstPoint ) << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << firstPoint << ", " << lastPoint << ", " 
			     << "\"" << f0_Stats2Down->GetExpFormula("p") << "\"" << "\n";

      fit_result_file[iHist] << "3, iterativefit, down_lfstats2, 2, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << f0_Stats2Down->Eval( lastPoint ) << "\"" << "\n";

      fit_result_file[iHist].close();
    }

    delete hint;
    delete legend;

    delete line;
  }

  for( int iHist=0; iHist<NumHists_normal; iHist++ ){
    fit_result_file[iHist].close();
  }

  // double fit_par_vals[3][3][nPars];
  // for( int iHist=0; iHist<numHists; iHist++ ){
  //   for( int iPar=0; iPar<nPars; iPar++ ){
  //     int iPt = -1;
  //     if( hist_name[iHist].Contains("Pt0") )      iPt = 0;
  //     else if( hist_name[iHist].Contains("Pt1") ) iPt = 1;
  //     else if( hist_name[iHist].Contains("Pt2") ) iPt = 2;
  //     int iEta = -1;
  //     if( hist_name[iHist].Contains("Eta0") )      iEta = 0;
  //     else if( hist_name[iHist].Contains("Eta1") ) iEta = 1;
  //     else if( hist_name[iHist].Contains("Eta2") ) iEta = 2;

  //     fit_par_vals[iPt][iEta][iPar] = par_vals[iHist][iPar];
  //     //std::cout << "fit_par_vals[" << iPt << "][" << iEta << "][" << iPar << "] = " << fit_par_vals[iPt][iEta][iPar] << ";" << std::endl;
  //   }

  //   printf(" %s:\t chi2 = %.2f,\t ndof = %d,\t prob = %.2f \n", hist_name[iHist].Data(), fit_chi2[iHist], fit_ndof[iHist], fit_prob[iHist] );
  //   printf(" \t(JESUp)  :\t chi2 = %.2f,\t ndof = %d,\t prob = %.2f \n", fit_chi2_JESUp[iHist], fit_ndof_JESUp[iHist], fit_prob_JESUp[iHist] );
  //   printf(" \t(JESDown):\t chi2 = %.2f,\t ndof = %d,\t prob = %.2f \n", fit_chi2_JESDown[iHist], fit_ndof_JESDown[iHist], fit_prob_JESDown[iHist] );
  // }


  delete [] fit_result_file;
  std::cout << " Done! " << std::endl;

  histofile.Write();
  histofile.Close();


}

/*
 FCN=4.37714 FROM IMProve     STATUS=RESET          48 CALLS         261 TOTAL
                     EDM=7.07195e-09    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.18246e+00   4.56664e-02   4.56664e-04   4.69042e+02
   2  p1           4.63312e+02   2.03551e+02   2.03551e+00  -4.44797e-01
   3  p2           2.10061e-06   1.49933e-06   1.49933e-08  -7.16288e+07
	 f1->Eval(463.312) = 0.731548
		 f1->GetMinimum(0,1000) = 0.731548
 FCN=0.0571179 FROM IMProve     STATUS=RESET          35 CALLS         207 TOTAL
                     EDM=1.04253e-08    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.21615e+00   4.59417e-02   4.59417e-04   1.71784e+02
   2  p1           3.82802e+02   4.77352e+01   4.77352e-01  -2.76792e-01
   3  p2           2.90873e-06   8.85574e-07   8.85574e-09  -2.20034e+07
 FCN=0.0615925 FROM IMProve     STATUS=RESET          43 CALLS         154 TOTAL
                     EDM=5.13031e-11    STRATEGY= 1  ERROR MATRIX UNCERTAINTY  50.0 per cent
  EXT PARAMETER                APPROXIMATE        STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.14857e+00   4.50183e-02   4.50183e-04   6.00544e+02
   2  p1           6.47355e+02   2.73547e+02   2.73547e+00  -4.90261e-01
   3  p2           1.28422e-06   8.54454e-07   8.54454e-09  -1.81000e+08



 TF1* f0 = new TF1("f0","[0] + [1]*TMath::Log([2]*x + [3]) + [4]*x + [5]*x*x + [6]*x*x*x + [7]*x*x*x*x + [8]*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 8.98,    ndof = 6,       prob = 0.17 
 csv_ratio_Pt0_Eta1:     chi2 = 3.89,    ndof = 6,       prob = 0.69 
 csv_ratio_Pt0_Eta2:     chi2 = 11.51,   ndof = 6,       prob = 0.07 
 csv_ratio_Pt1_Eta0:     chi2 = 5.51,    ndof = 6,       prob = 0.48 
 csv_ratio_Pt1_Eta1:     chi2 = 9.67,    ndof = 6,       prob = 0.14 
 csv_ratio_Pt1_Eta2:     chi2 = 12.62,   ndof = 6,       prob = 0.05 
 csv_ratio_Pt2_Eta0:     chi2 = 5.62,    ndof = 6,       prob = 0.47 
 csv_ratio_Pt2_Eta1:     chi2 = 7.55,    ndof = 6,       prob = 0.27 
 csv_ratio_Pt2_Eta2:     chi2 = 4.37,    ndof = 6,       prob = 0.63 
 csv_ratio_all:  chi2 = 12.21,   ndof = 6,       prob = 0.06 
 csv_ratio_cumulative_all:       chi2 = 24.52,   ndof = 6,       prob = 0.00


 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 8.98,    ndof = 9,       prob = 0.44,    SF (CSV=1) = 2.38 
 csv_ratio_Pt0_Eta1:     chi2 = 3.89,    ndof = 9,       prob = 0.92,    SF (CSV=1) = 3.19 
 csv_ratio_Pt0_Eta2:     chi2 = 12.73,   ndof = 9,       prob = 0.18,    SF (CSV=1) = 1.79 
 csv_ratio_Pt1_Eta0:     chi2 = 5.51,    ndof = 9,       prob = 0.79,    SF (CSV=1) = 1.98 
 csv_ratio_Pt1_Eta1:     chi2 = 9.67,    ndof = 9,       prob = 0.38,    SF (CSV=1) = 0.75 
 csv_ratio_Pt1_Eta2:     chi2 = 12.62,   ndof = 9,       prob = 0.18,    SF (CSV=1) = 1.56 
 csv_ratio_Pt2_Eta0:     chi2 = 5.62,    ndof = 9,       prob = 0.78,    SF (CSV=1) = 5.59 
 csv_ratio_Pt2_Eta1:     chi2 = 7.55,    ndof = 9,       prob = 0.58,    SF (CSV=1) = 2.55 
 csv_ratio_Pt2_Eta2:     chi2 = 4.37,    ndof = 9,       prob = 0.89,    SF (CSV=1) = 3.32 
 csv_ratio_all:  chi2 = 12.21,   ndof = 9,       prob = 0.20,    SF (CSV=1) = 2.67 
 csv_ratio_cumulative_all:       chi2 = 24.52,   ndof = 9,       prob = 0.00,    SF (CSV=1) = 2.67 

 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 6.87,    ndof = 8,       prob = 0.55,    SF (CSV=1) = 3.94 
 csv_ratio_Pt0_Eta1:     chi2 = 3.82,    ndof = 8,       prob = 0.87,    SF (CSV=1) = 2.91 
 csv_ratio_Pt0_Eta2:     chi2 = 10.35,   ndof = 8,       prob = 0.24,    SF (CSV=1) = 0.56 
 csv_ratio_Pt1_Eta0:     chi2 = 3.78,    ndof = 8,       prob = 0.88,    SF (CSV=1) = 4.21 
 csv_ratio_Pt1_Eta1:     chi2 = 8.93,    ndof = 8,       prob = 0.35,    SF (CSV=1) = 2.17 
 csv_ratio_Pt1_Eta2:     chi2 = 10.30,   ndof = 8,       prob = 0.24,    SF (CSV=1) = 3.70 
 csv_ratio_Pt2_Eta0:     chi2 = 3.34,    ndof = 8,       prob = 0.91,    SF (CSV=1) = 8.36 
 csv_ratio_Pt2_Eta1:     chi2 = 7.57,    ndof = 8,       prob = 0.48,    SF (CSV=1) = 2.66 
 csv_ratio_Pt2_Eta2:     chi2 = 3.93,    ndof = 8,       prob = 0.86,    SF (CSV=1) = 4.03 
 csv_ratio_all:  chi2 = 9.51,    ndof = 8,       prob = 0.30,    SF (CSV=1) = 3.38 
 csv_ratio_cumulative_all:       chi2 = 9.84,    ndof = 8,       prob = 0.28,    SF (CSV=1) = 3.63 

 TF1* f0 = new TF1("f0","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x + [7]*x*x*x*x*x*x",0,0.95 );
 csv_ratio_Pt0_Eta0:     chi2 = 6.87,    ndof = 7,       prob = 0.44,    SF (CSV=1) = 3.94 
 csv_ratio_Pt0_Eta1:     chi2 = 3.82,    ndof = 7,       prob = 0.80,    SF (CSV=1) = 2.91 
 csv_ratio_Pt0_Eta2:     chi2 = 10.35,   ndof = 7,       prob = 0.17,    SF (CSV=1) = 0.56 
 csv_ratio_Pt1_Eta0:     chi2 = 3.78,    ndof = 7,       prob = 0.80,    SF (CSV=1) = 4.21 
 csv_ratio_Pt1_Eta1:     chi2 = 8.93,    ndof = 7,       prob = 0.26,    SF (CSV=1) = 2.17 
 csv_ratio_Pt1_Eta2:     chi2 = 10.30,   ndof = 7,       prob = 0.17,    SF (CSV=1) = 3.70 
 csv_ratio_Pt2_Eta0:     chi2 = 3.34,    ndof = 7,       prob = 0.85,    SF (CSV=1) = 8.36 
 csv_ratio_Pt2_Eta1:     chi2 = 7.57,    ndof = 7,       prob = 0.37,    SF (CSV=1) = 2.66 
 csv_ratio_Pt2_Eta2:     chi2 = 3.93,    ndof = 7,       prob = 0.79,    SF (CSV=1) = 4.04 
 csv_ratio_all:  chi2 = 9.51,    ndof = 7,       prob = 0.22,    SF (CSV=1) = 3.38 
 csv_ratio_cumulative_all:       chi2 = 9.84,    ndof = 7,       prob = 0.20,    SF (CSV=1) = 3.63 


 */
