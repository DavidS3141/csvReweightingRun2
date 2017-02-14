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
#include "TMath.h"

#include "Math/MinimizerOptions.h"
#include <TSpline.h>
#include "Math/Interpolator.h"
#define MAXPOINTS 200

//  double csvbins_new[] = { -0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01 };
Double_t fit_csv(Double_t *x, Double_t *par)
{
  Double_t xv =x[0];

  double b0 = 0.331;
  double b1 = 0.7666;

  double f = (xv<b0)*(par[0] + xv*(par[1] + xv*(par[2] + xv*(par[3] + xv*(par[4] + xv*(par[5] + xv*par[6])))))) + 
    (xv>=b0 && xv<b1)*(par[7] + xv*(par[8] + xv*(par[9] + xv*(par[10] + xv*(par[11] + xv*(par[12] + xv*par[13])))))) +
    (xv>=b1)*(par[14] + xv*(par[15] + xv*(par[16] + xv*(par[17] + xv*(par[18] + xv*(par[19] + xv*par[20]))))));

  return f;
}

Double_t fit_csv_1(Double_t *x, Double_t *par)
{
  Double_t xv =x[0];

  double b0 = par[21];
  double b1 = par[22];

  double f = (xv<b0)*(par[0] + xv*(par[1] + xv*(par[2] + xv*(par[3] + xv*(par[4] + xv*(par[5] + xv*par[6])))))) + 
    (xv>=b0 && xv<b1)*(par[7] + xv*(par[8] + xv*(par[9] + xv*(par[10] + xv*(par[11] + xv*(par[12] + xv*par[13])))))) +
    (xv>=b1)*(par[14] + xv*(par[15] + xv*(par[16] + xv*(par[17] + xv*(par[18] + xv*(par[19] + xv*par[20]))))));

  return f;
}

Double_t fit_csv_2(Double_t *x, Double_t *par)
{
  Double_t xv =x[0];

  double b0 = 0.331;
  double b1 = 0.592;
  double b2 = 0.8104;

  double f = (xv<b0)*(par[0] + xv*(par[1] + xv*(par[2] + xv*(par[3] + xv*(par[4] + xv*(par[5] + xv*par[6])))))) + 
    (xv>=b0 && xv<b1)*(par[7] + xv*(par[8] + xv*(par[9] + xv*(par[10] + xv*(par[11] + xv*(par[12] + xv*par[13])))))) +
    (xv>=b1 && xv<b2)*(par[14] + xv*(par[15] + xv*(par[16] + xv*(par[17] + xv*(par[18] + xv*(par[19] + xv*par[20])))))) +
    (xv>=b2)*(par[21] + xv*(par[22] + xv*(par[23] + xv*(par[24] + xv*(par[25] + xv*(par[26] + xv*par[27]))))));

  return f;
}

Double_t fit_csv_3(Double_t *x, Double_t *par)
{
  Double_t xv =x[0];

  //double b0 = par[21];
  //double b1 = par[22];
  double b0 = 0.331;
  double b1 = 0.7666;

  double f = (xv<b0)*(par[0] + par[1]*cos(par[2]*xv + par[3]) + par[4]*xv*cos(par[5]*xv + par[6])) +
    (xv>=b0 && xv<b1)*(par[7] + par[8]*cos(par[9]*xv + par[10]) + par[11]*xv*cos(par[12]*xv + par[13])) +
    (xv>=b1)*(par[14] + par[15]*cos(par[16]*xv + par[17]) + par[18]*xv*cos(par[19]*xv + par[20]));

  return f;
}

Double_t fit_csv_5(Double_t *x, Double_t *par)
{
  Double_t xv =x[0];
  double denom = 1.0;

  int size = 20;//par[0]
  
  double f = 0;

  for( int ipar=0; ipar<size; ipar++ )
    f += par[ipar] * cos(TMath::Pi() * ipar * xv/denom);

  return f;
}


//______________________________________________________________________________
void final_fitHF_btagSF_13TeV( bool isCSV = true,
			       TString inputFileName  = "infile.root", 
			       TString inputFileName_JESUp  = "infile.root", 
			       TString inputFileName_JESDown  = "infile.root", 
			       int iterNum=0,
			       bool includeCorrectionFactor_ = !false,
			       bool includeCharmCorrectionFactor_ = !false,
			       TString dirPostFix = "" ){


  TH1::SetDefaultSumw2();

  TString btagger = ( isCSV ) ? "csv" : "cmva";

  TString dirprefix = "Images/Images_2017_2_10_fitHF_" + btagger + "_13TeV" + dirPostFix + "/";

  
  struct stat st;
  if( stat(dirprefix.Data(),&st) != 0 )  mkdir(dirprefix.Data(),0777);


  TFile *file = TFile::Open(inputFileName);
  TFile *file_JESUp = TFile::Open(inputFileName_JESUp);
  TFile *file_JESDown = TFile::Open(inputFileName_JESDown);

  std::cout << " ###===> iteration version " << iterNum << std::endl;

  bool verbose = false;
  bool makePlots = true;


  std::string histofilename = Form("%s_rwt_fit_hf_v%d_final_2017_2_10" + dirPostFix + ".root",btagger.Data(),iterNum) ;
  TFile histofile(histofilename.c_str(),"recreate");
  histofile.cd();



  // TString lumiinfo = "36.8 fb^{-1} (13 TeV)";
  // TLatex LumiInfoLatex(0.70, 0.91, lumiinfo);
  TString lumiinfo = "36 fb^{-1}, #sqrt{s} = 13 TeV, 2016";
  TLatex LumiInfoLatex(0.65, 0.91, lumiinfo);
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


  double useUp = 1.2;
  double useDown = 0.8;

  // // Original
  // int ncsvbins = 18;
  // double csvbins[] = {-0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};
  // double csvbins_new[] = {-0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};
  // // Rebin for stats
  // int ncsvbins = 14;
  // double csvbins[] = {-10.0, 0.0, 0.244, 0.418, 0.592, 0.679, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};
  // double csvbins_new[] = {-0.04, 0.0, 0.244, 0.418, 0.592, 0.679, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01};
  // Compare to old SFs
  // int ncsvbins = 18; // 8TeV
  // double csvbins[] = { -10.0, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01 };
  // double csvbins_new[] = { -0.04, 0.0, 0.122, 0.244, 0.331, 0.418, 0.505, 0.592, 0.679, 0.7228, 0.7666, 0.8104, 0.8542, 0.898, 0.9184, 0.9388, 0.9592, 0.9796, 1.01 };

  int ncsvbins = 18; //13TeV
  //double csvbins[] = {-10.0, 0.0, 0.303, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 0.906, 0.922, 0.938, 0.954, 0.970, 0.976, 0.982, 0.988, 0.994, 1.01};
  //double csvbins_new[] = { -0.04, 0.0, 0.303, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 0.906, 0.922, 0.938, 0.954, 0.970, 0.976, 0.982, 0.988, 0.994, 1.01};
  // double csvbins[] = {-10.0, 0.0, 0.303, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 0.906, 0.922, 0.938, 0.954, 0.970, 0.976, 0.982, 0.988, 0.994, 1.01};
  // double csvbins_new[] = { -0.04, 0.0, 0.303, 0.605, 0.662, 0.719, 0.776, 0.833, 0.890, 0.906, 0.922, 0.938, 0.954, 0.970, 0.976, 0.982, 0.988, 0.994, 1.01};

  /// ---------13TeV 2016 ICHEP
  // double csvbins[] = {-10.0, 0.0, 0.24, 0.460, 0.528, 0.596, 0.664, 0.732, 0.800, 0.827, 0.854, 0.881, 0.908, 0.935, 0.948, 0.961, 0.974, 0.987, 1.01};
  // double csvbins_new[] = {-0.04, 0.0, 0.24, 0.460, 0.528, 0.596, 0.664, 0.732, 0.800, 0.827, 0.854, 0.881, 0.908, 0.935, 0.948, 0.961, 0.974, 0.987, 1.01};

  // double cMVAbins[] = {-1.01, -0.8575, -0.715, -0.565, -0.415, -0.265, -0.115, 0.035, 0.185, 0.323, 0.461, 0.599, 0.737, 0.875, 0.902, 0.929, 0.956, 0.983, 1.01};
  // double cMVAbins_new[] = {-1.01, -0.8575, -0.715, -0.565, -0.415, -0.265, -0.115, 0.035, 0.185, 0.323, 0.461, 0.599, 0.737, 0.875, 0.902, 0.929, 0.956, 0.983, 1.01};

  /// ---------13TeV 2017 Moriond
  double csvbins[] = {-10.0, 0.0, 0.2712, 0.5426, 0.6036, 0.6648, 0.726, 0.7872, 0.8484, 0.8695, 0.8905, 0.9115, 0.9325, 0.9535, 0.9648, 0.9761, 0.9874, 0.9987, 1.01};
  double csvbins_new[] = {-0.04, 0.0, 0.2712, 0.5426, 0.6036, 0.6648, 0.726, 0.7872, 0.8484, 0.8695, 0.8905, 0.9115, 0.9325, 0.9535, 0.9648, 0.9761, 0.9874, 0.9987, 1.01};

  double cMVAbins[] = {-1.01, -0.7991, -0.5884, -0.4163, -0.2444, -0.0725, 0.0994, 0.2713, 0.4432, 0.5432, 0.6432, 0.7432, 0.8432, 0.9432, 0.9564, 0.9698, 0.9832, 0.9966, 1.01};
  double cMVAbins_new[] = {-1.01, -0.7991, -0.5884, -0.4163, -0.2444, -0.0725, 0.0994, 0.2713, 0.4432, 0.5432, 0.6432, 0.7432, 0.8432, 0.9432, 0.9564, 0.9698, 0.9832, 0.9966, 1.01};


  std::vector<TString> bin_name;
  std::vector<TString> hist_name;
  std::vector<TString> data_hist_name;
  std::vector<TString> mc_b_hist_name;
  std::vector<TString> mc_nonb_hist_name;

  std::vector<TString> label_ptbin;
  std::vector<TString> label_etabin;


  int maxPt  = 5;//6;
  int maxEta = 1;

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
      else if( iPt==3 ) label_ptbin.push_back("60 < p_{T} < 100 GeV");
      else if( iPt==4 ) label_ptbin.push_back("p_{T} > 100 GeV");

      if( true ) label_etabin.push_back("|#eta| < 2.4");
    }
  }


  int NumHists_normal = int( hist_name.size() );
  int numHists = NumHists_normal+2;

  ofstream *fit_result_file = new ofstream[NumHists_normal];

  TH1D* h_csv_ratio[numHists];
  TH1D* h_csv_ratio_LF[numHists];
  TH1D* h_csv_ratio_LFUp[numHists];
  TH1D* h_csv_ratio_LFDown[numHists];
  TH1D* h_csv_ratio_JESUp[numHists];
  TH1D* h_csv_ratio_JESDown[numHists];
  TH1D* h_csv_ratio_Stats1Up[numHists];
  TH1D* h_csv_ratio_Stats1Down[numHists];
  TH1D* h_csv_ratio_Stats2Up[numHists];
  TH1D* h_csv_ratio_Stats2Down[numHists];

  // TH1D* h_csv_ratio_NewStats1Up[numHists];
  // TH1D* h_csv_ratio_NewStats1Down[numHists];
  // TH1D* h_csv_ratio_NewStats2Up[numHists];
  // TH1D* h_csv_ratio_NewStats2Down[numHists];

  TH1D* c_csv_ratio_final[numHists];
  TH1D* c_csv_ratio_final_CUp[numHists];
  TH1D* c_csv_ratio_final_CDown[numHists];

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

  TH1D* h_csv_data_all_JES = NULL;

  TH1D* h_csv_mc_b_all_JESUp = NULL;
  TH1D* h_csv_mc_nonb_all_JESUp = NULL;

  TH1D* h_csv_mc_b_all_JESDown = NULL;
  TH1D* h_csv_mc_nonb_all_JESDown = NULL;


  for( int iHist=0; iHist<NumHists_normal; iHist++ ){

    TH1D* h_csv_data_temp0 = (TH1D*)file->Get( data_hist_name[iHist] )->Clone( Form("h_%s_temp0",data_hist_name[iHist].Data()) );

    TH1D* h_csv_mc_b_temp0 = (TH1D*)file->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_temp0 = (TH1D*)file->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_temp0",mc_nonb_hist_name[iHist].Data()) );

    // TH1D* h_csv_data_temp0_rebin = (TH1D*)h_csv_data_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",data_hist_name[iHist].Data()),csvbins);
    // TH1D* h_csv_mc_b_temp0_rebin = (TH1D*)h_csv_mc_b_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    // TH1D* h_csv_mc_nonb_temp0_rebin = (TH1D*)h_csv_mc_nonb_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);
    TH1D* h_csv_data_temp0_rebin ;
    TH1D* h_csv_mc_b_temp0_rebin ;
    TH1D* h_csv_mc_nonb_temp0_rebin ;

    if(isCSV){
     h_csv_data_temp0_rebin = (TH1D*)h_csv_data_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",data_hist_name[iHist].Data()),csvbins);
     h_csv_mc_b_temp0_rebin = (TH1D*)h_csv_mc_b_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
     h_csv_mc_nonb_temp0_rebin = (TH1D*)h_csv_mc_nonb_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);
    }
    else{
     h_csv_data_temp0_rebin = (TH1D*)h_csv_data_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",data_hist_name[iHist].Data()),cMVAbins);
     h_csv_mc_b_temp0_rebin = (TH1D*)h_csv_mc_b_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_b_hist_name[iHist].Data()),cMVAbins);
     h_csv_mc_nonb_temp0_rebin = (TH1D*)h_csv_mc_nonb_temp0->Rebin(ncsvbins,Form("h_%s_temp0_rebin",mc_nonb_hist_name[iHist].Data()),cMVAbins);
    }
    
    // JES up/down
    TH1D* h_csv_mc_b_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESUp_temp0 = (TH1D*)file_JESUp->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESUp_temp0",mc_nonb_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_b_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_b_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_b_hist_name[iHist].Data()) );
    TH1D* h_csv_mc_nonb_JESDown_temp0 = (TH1D*)file_JESDown->Get( mc_nonb_hist_name[iHist] )->Clone( Form("h_%s_JESDown_temp0",mc_nonb_hist_name[iHist].Data()) );

    // TH1D* h_csv_mc_b_JESUp_temp0_rebin = (TH1D*)h_csv_mc_b_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    // TH1D* h_csv_mc_nonb_JESUp_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);
    // TH1D* h_csv_mc_b_JESDown_temp0_rebin = (TH1D*)h_csv_mc_b_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
    // TH1D* h_csv_mc_nonb_JESDown_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);

    TH1D* h_csv_mc_b_JESUp_temp0_rebin ;
    TH1D* h_csv_mc_nonb_JESUp_temp0_rebin ;
    TH1D* h_csv_mc_b_JESDown_temp0_rebin ;
    TH1D* h_csv_mc_nonb_JESDown_temp0_rebin ;

    if(isCSV){
     h_csv_mc_b_JESUp_temp0_rebin = (TH1D*)h_csv_mc_b_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
     h_csv_mc_nonb_JESUp_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);
     h_csv_mc_b_JESDown_temp0_rebin = (TH1D*)h_csv_mc_b_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_b_hist_name[iHist].Data()),csvbins);
     h_csv_mc_nonb_JESDown_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_nonb_hist_name[iHist].Data()),csvbins);
    }
    else{
     h_csv_mc_b_JESUp_temp0_rebin = (TH1D*)h_csv_mc_b_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_b_hist_name[iHist].Data()),cMVAbins);
     h_csv_mc_nonb_JESUp_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESUp_temp0->Rebin(ncsvbins,Form("h_%s_JESUp_temp0_rebin",mc_nonb_hist_name[iHist].Data()),cMVAbins);
     h_csv_mc_b_JESDown_temp0_rebin = (TH1D*)h_csv_mc_b_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_b_hist_name[iHist].Data()),cMVAbins);
     h_csv_mc_nonb_JESDown_temp0_rebin = (TH1D*)h_csv_mc_nonb_JESDown_temp0->Rebin(ncsvbins,Form("h_%s_JESDown_temp0_rebin",mc_nonb_hist_name[iHist].Data()),cMVAbins);
    }

    // h_csv_data[iHist] = new TH1D( Form("h_%s",data_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    // h_csv_mc_b[iHist] = new TH1D( Form("h_%s",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    // h_csv_mc_nonb[iHist] = new TH1D( Form("h_%s",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );

    // h_csv_mc_b_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    // h_csv_mc_nonb_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    // h_csv_mc_b_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    // h_csv_mc_nonb_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    if(isCSV){
      h_csv_data[iHist] = new TH1D( Form("h_%s",data_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
      h_csv_mc_b[iHist] = new TH1D( Form("h_%s",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
      h_csv_mc_nonb[iHist] = new TH1D( Form("h_%s",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
      
      h_csv_mc_b_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
      h_csv_mc_nonb_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
      h_csv_mc_b_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_b_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
      h_csv_mc_nonb_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_nonb_hist_name[iHist].Data()), ";CSV", ncsvbins, csvbins_new );
    }
    else{
      h_csv_data[iHist] = new TH1D( Form("h_%s",data_hist_name[iHist].Data()), ";cMVA", ncsvbins, cMVAbins_new );
      h_csv_mc_b[iHist] = new TH1D( Form("h_%s",mc_b_hist_name[iHist].Data()), ";cMVA", ncsvbins, cMVAbins_new );
      h_csv_mc_nonb[iHist] = new TH1D( Form("h_%s",mc_nonb_hist_name[iHist].Data()), ";cMVA", ncsvbins, cMVAbins_new );
      
      h_csv_mc_b_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_b_hist_name[iHist].Data()), ";cMVA", ncsvbins, cMVAbins_new );
      h_csv_mc_nonb_JESUp[iHist] = new TH1D( Form("h_%s_JESUp",mc_nonb_hist_name[iHist].Data()), ";cMVA", ncsvbins, cMVAbins_new );
      h_csv_mc_b_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_b_hist_name[iHist].Data()), ";cMVA", ncsvbins, cMVAbins_new );
      h_csv_mc_nonb_JESDown[iHist] = new TH1D( Form("h_%s_JESDown",mc_nonb_hist_name[iHist].Data()), ";cMVA", ncsvbins, cMVAbins_new );
    }

    
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

    if( true ){
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
    }

    h_csv_ratio[iHist]        = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_%d",iHist));
    h_csv_ratio_LFUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_LFUp_%d",iHist));
    h_csv_ratio_LFDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_LFDown_%d",iHist));

    h_csv_ratio_JESUp[iHist]   = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESUp_%d",iHist));
    h_csv_ratio_JESDown[iHist] = (TH1D*)h_csv_data[iHist]->Clone(Form("h_csv_ratio_JESDown_%d",iHist));

    TH1D* h_csv_mc_b_temp0_LFUp = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_LFUp_%d",iHist));
    TH1D* h_csv_mc_b_temp0_LFDown = (TH1D*)h_csv_mc_b[iHist]->Clone(Form("h_csv_mc_b_temp0_LFDown_%d",iHist));

    TH1D* h_csv_mc_nonb_temp0_LFUp = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_LFUp_%d",iHist));
    TH1D* h_csv_mc_nonb_temp0_LFDown = (TH1D*)h_csv_mc_nonb[iHist]->Clone(Form("h_csv_mc_nonb_temp0_LFDown_%d",iHist));


    ////////////////
    double data_integral = h_csv_data[iHist]->Integral();
    double mc_b_integral = h_csv_mc_b[iHist]->Integral();
    double mc_nonb_integral = h_csv_mc_nonb[iHist]->Integral();

    h_csv_mc_b[iHist]->Scale( data_integral / ( mc_b_integral + mc_nonb_integral ) );
    h_csv_mc_nonb[iHist]->Scale( data_integral / ( mc_b_integral + mc_nonb_integral ) );

    /////

    double mc_nonb_temp0_LFUp_integral = useUp*h_csv_mc_nonb[iHist]->Integral();
    double mc_nonb_temp0_LFDown_integral = useDown*h_csv_mc_nonb[iHist]->Integral();

    h_csv_mc_b_temp0_LFUp->Scale( data_integral / ( mc_b_integral + mc_nonb_temp0_LFUp_integral ) );
    h_csv_mc_b_temp0_LFDown->Scale( data_integral / ( mc_b_integral + mc_nonb_temp0_LFDown_integral ) );

    h_csv_mc_nonb_temp0_LFUp->Scale( data_integral / ( mc_b_integral + mc_nonb_temp0_LFUp_integral ) );
    h_csv_mc_nonb_temp0_LFDown->Scale( data_integral / ( mc_b_integral + mc_nonb_temp0_LFDown_integral ) );

    /////

    double mc_b_JESUp_integral = h_csv_mc_b_JESUp[iHist]->Integral();
    double mc_nonb_JESUp_integral = h_csv_mc_nonb_JESUp[iHist]->Integral();

    double mc_b_JESDown_integral = h_csv_mc_b_JESDown[iHist]->Integral();
    double mc_nonb_JESDown_integral = h_csv_mc_nonb_JESDown[iHist]->Integral();

    h_csv_mc_b_JESUp[iHist]->Scale( data_integral / ( mc_b_JESUp_integral + mc_nonb_JESUp_integral ) );
    h_csv_mc_b_JESDown[iHist]->Scale( data_integral / ( mc_b_JESDown_integral + mc_nonb_JESDown_integral ) );

    h_csv_mc_nonb_JESUp[iHist]->Scale( data_integral / ( mc_b_JESUp_integral + mc_nonb_JESUp_integral ) );
    h_csv_mc_nonb_JESDown[iHist]->Scale( data_integral / ( mc_b_JESDown_integral + mc_nonb_JESDown_integral ) );

    ////////////////

    ///////
    h_csv_ratio[iHist]->Add(h_csv_mc_nonb[iHist],-1);
    h_csv_ratio_LFUp[iHist]->Add(h_csv_mc_nonb_temp0_LFUp,-useUp);
    h_csv_ratio_LFDown[iHist]->Add(h_csv_mc_nonb_temp0_LFDown,-useDown);
    h_csv_ratio_JESUp[iHist]->Add(h_csv_mc_nonb_JESUp[iHist],-1);
    h_csv_ratio_JESDown[iHist]->Add(h_csv_mc_nonb_JESDown[iHist],-1);

    h_csv_ratio[iHist]->Divide(h_csv_mc_b[iHist]);
    h_csv_ratio_LFUp[iHist]->Divide(h_csv_mc_b_temp0_LFUp);
    h_csv_ratio_LFDown[iHist]->Divide(h_csv_mc_b_temp0_LFDown);
    h_csv_ratio_JESUp[iHist]->Divide(h_csv_mc_b_JESUp[iHist]);
    h_csv_ratio_JESDown[iHist]->Divide(h_csv_mc_b_JESDown[iHist]);


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
      if( !isCSV ){
	stat1Up   = content + delta * ( -center );
	stat1Down = content + delta * ( center );
      }
      
      double stat2Up   = content + delta * ( 1 - 6*center*(1-center) );
      double stat2Down = content - delta * ( 1 - 6*center*(1-center) );
      if( !isCSV ){
	stat2Up   = content + delta * 0.25 * ( 1 + 3*center*center );
	stat2Down = content - delta * 0.25 * ( 1 + 3*center*center );
      }
      
      h_csv_ratio_Stats1Up[iHist]->SetBinContent(iBin+1,stat1Up);
      h_csv_ratio_Stats1Down[iHist]->SetBinContent(iBin+1,stat1Down);

      h_csv_ratio_Stats2Up[iHist]->SetBinContent(iBin+1,stat2Up);
      h_csv_ratio_Stats2Down[iHist]->SetBinContent(iBin+1,stat2Down);
    }


    h_csv_ratio_Stats1Up[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats1Up[iHist]->Integral() );
    h_csv_ratio_Stats1Down[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats1Down[iHist]->Integral() );
    h_csv_ratio_Stats2Up[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats2Up[iHist]->Integral() );
    h_csv_ratio_Stats2Down[iHist]->Scale( h_csv_data[iHist]->Integral() / h_csv_ratio_Stats2Down[iHist]->Integral() );

    h_csv_ratio_Stats1Up[iHist]->Add(h_csv_mc_nonb[iHist],-1);
    h_csv_ratio_Stats1Down[iHist]->Add(h_csv_mc_nonb[iHist],-1);
    h_csv_ratio_Stats2Up[iHist]->Add(h_csv_mc_nonb[iHist],-1);
    h_csv_ratio_Stats2Down[iHist]->Add(h_csv_mc_nonb[iHist],-1);

    h_csv_ratio_Stats1Up[iHist]->Divide(h_csv_mc_b[iHist]);
    h_csv_ratio_Stats1Down[iHist]->Divide(h_csv_mc_b[iHist]);
    h_csv_ratio_Stats2Up[iHist]->Divide(h_csv_mc_b[iHist]);
    h_csv_ratio_Stats2Down[iHist]->Divide(h_csv_mc_b[iHist]);
  }


  TH1D* h_csv_ratio_all = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_temp");
  TH1D* h_csv_ratio_all_LFUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_LFUp_temp");
  TH1D* h_csv_ratio_all_LFDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_LFDown_temp");

  TH1D* h_csv_ratio_all_JESUp   = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESUp_temp");
  TH1D* h_csv_ratio_all_JESDown = (TH1D*)h_csv_data_all->Clone("h_csv_ratio_all_JESDown_temp");

  TH1D* h_csv_ratio_cumulative = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative");
  TH1D* h_csv_ratio_cumulative_LFUp = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_LFUp");
  TH1D* h_csv_ratio_cumulative_LFDown = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_LFDown");
  TH1D* h_csv_ratio_cumulative_JESUp = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_JESUp");
  TH1D* h_csv_ratio_cumulative_JESDown = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_JESDown");
  TH1D* h_csv_ratio_cumulative_Stats1Up = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats1Up");
  TH1D* h_csv_ratio_cumulative_Stats1Down = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats1Down");
  TH1D* h_csv_ratio_cumulative_Stats2Up = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats2Up");
  TH1D* h_csv_ratio_cumulative_Stats2Down = (TH1D*)h_csv_ratio_all->Clone("h_csv_ratio_cumulative_Stats2Down");

  int nBins = h_csv_ratio_cumulative->GetNbinsX();


  double data_all_integral = h_csv_ratio_all->Integral();
  double mc_b_all_integral = h_csv_mc_b_all->Integral();
  double mc_nonb_all_integral = h_csv_mc_nonb_all->Integral();

  h_csv_mc_b_all->Scale( data_all_integral / ( mc_b_all_integral + mc_nonb_all_integral ) );
  h_csv_mc_nonb_all->Scale( data_all_integral / ( mc_b_all_integral + mc_nonb_all_integral ) );


  TH1D* h_csv_mc_b_all_LFUp = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_LFUp");
  TH1D* h_csv_mc_b_all_LFDown = (TH1D*)h_csv_mc_b_all->Clone("h_csv_mc_b_all_LFDown");

  TH1D* h_csv_mc_nonb_all_LFUp = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_LFUp");
  TH1D* h_csv_mc_nonb_all_LFDown = (TH1D*)h_csv_mc_nonb_all->Clone("h_csv_mc_nonb_all_LFDown");


  double mc_nonb_all_integral_LFUp = useUp*h_csv_mc_nonb_all->Integral();
  double mc_nonb_all_integral_LFDown = useDown*h_csv_mc_nonb_all->Integral();


  h_csv_mc_b_all_LFUp->Scale( data_all_integral / (mc_b_all_integral + mc_nonb_all_integral_LFUp) );
  h_csv_mc_b_all_LFDown->Scale( data_all_integral / (mc_b_all_integral + mc_nonb_all_integral_LFDown) );

  h_csv_mc_nonb_all_LFUp->Scale( data_all_integral / (mc_b_all_integral + mc_nonb_all_integral_LFUp) );
  h_csv_mc_nonb_all_LFDown->Scale( data_all_integral / (mc_b_all_integral + mc_nonb_all_integral_LFDown) );


  double mc_b_all_integral_JESUp = h_csv_mc_b_all_JESUp->Integral();
  double mc_nonb_all_integral_JESUp = h_csv_mc_nonb_all_JESUp->Integral();

  double mc_b_all_integral_JESDown = h_csv_mc_b_all_JESDown->Integral();
  double mc_nonb_all_integral_JESDown = h_csv_mc_nonb_all_JESDown->Integral();

  h_csv_mc_b_all_JESUp->Scale( data_all_integral / (mc_b_all_integral_JESUp + mc_nonb_all_integral_JESUp) );
  h_csv_mc_b_all_JESDown->Scale( data_all_integral / (mc_b_all_integral_JESDown + mc_nonb_all_integral_JESDown) );

  h_csv_mc_nonb_all_JESUp->Scale( data_all_integral / (mc_b_all_integral_JESUp + mc_nonb_all_integral_JESUp) );
  h_csv_mc_nonb_all_JESDown->Scale( data_all_integral / (mc_b_all_integral_JESDown + mc_nonb_all_integral_JESDown) );


  h_csv_ratio_all->Add(h_csv_mc_nonb_all,-1);
  h_csv_ratio_all_LFUp->Add(h_csv_mc_nonb_all_LFUp,-useUp);
  h_csv_ratio_all_LFDown->Add(h_csv_mc_nonb_all_LFDown,-useDown);
  h_csv_ratio_all_JESUp->Add(h_csv_mc_nonb_all_JESUp,-1);
  h_csv_ratio_all_JESDown->Add(h_csv_mc_nonb_all_JESDown,-1);


  for( int iBin=0; iBin<nBins; iBin++ ){
    h_csv_ratio_cumulative->SetBinContent( iBin+1, h_csv_ratio_all->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_LFUp->SetBinContent( iBin+1, h_csv_ratio_all_LFUp->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_LFDown->SetBinContent( iBin+1, h_csv_ratio_all_LFDown->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_JESUp->SetBinContent( iBin+1, h_csv_ratio_all_JESUp->Integral(iBin+1,nBins) );
    h_csv_ratio_cumulative_JESDown->SetBinContent( iBin+1, h_csv_ratio_all_JESDown->Integral(iBin+1,nBins) );
  }


  h_csv_ratio_all->Divide(h_csv_mc_b_all);
  h_csv_ratio_all_LFUp->Divide(h_csv_mc_b_all_LFUp);
  h_csv_ratio_all_LFDown->Divide(h_csv_mc_b_all_LFDown);
  h_csv_ratio_all_JESUp->Divide(h_csv_mc_b_all_JESUp);
  h_csv_ratio_all_JESDown->Divide(h_csv_mc_b_all_JESDown);



  TH1D* h_mc_nonb_cumulative = (TH1D*)h_csv_mc_nonb_all->Clone("h_mc_nonb_cumulative");
  TH1D* h_mc_b_cumulative = (TH1D*)h_csv_mc_b_all->Clone("h_mc_b_cumulative");
  TH1D* h_mc_b_cumulative_LFUp = (TH1D*)h_csv_mc_b_all_LFUp->Clone("h_mc_b_cumulative_LFUp");
  TH1D* h_mc_b_cumulative_LFDown = (TH1D*)h_csv_mc_b_all_LFDown->Clone("h_mc_b_cumulative_LFDown");
  TH1D* h_mc_b_cumulative_JESUp = (TH1D*)h_csv_mc_b_all_JESUp->Clone("h_mc_b_cumulative_JESUp");
  TH1D* h_mc_b_cumulative_JESDown = (TH1D*)h_csv_mc_b_all_JESDown->Clone("h_mc_b_cumulative_JESDown");

  for( int iBin=0; iBin<nBins; iBin++ ){
    h_mc_nonb_cumulative->SetBinContent( iBin+1, h_csv_mc_nonb_all->Integral(iBin+1,nBins) );

    h_mc_b_cumulative->SetBinContent( iBin+1, h_csv_mc_b_all->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_LFUp->SetBinContent( iBin+1, h_csv_mc_b_all_LFUp->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_LFDown->SetBinContent( iBin+1, h_csv_mc_b_all_LFDown->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_JESUp->SetBinContent( iBin+1, h_csv_mc_b_all_JESUp->Integral(iBin+1,nBins) );
    h_mc_b_cumulative_JESDown->SetBinContent( iBin+1, h_csv_mc_b_all_JESDown->Integral(iBin+1,nBins) );
  }

  h_csv_ratio_cumulative->Divide(h_mc_b_cumulative);
  h_csv_ratio_cumulative_LFUp->Divide(h_mc_b_cumulative_LFUp);
  h_csv_ratio_cumulative_LFDown->Divide(h_mc_b_cumulative_LFDown);
  h_csv_ratio_cumulative_JESUp->Divide(h_mc_b_cumulative_JESUp);
  h_csv_ratio_cumulative_JESDown->Divide(h_mc_b_cumulative_JESDown);


  if( false ){
    for( int iBin=0; iBin<nBins; iBin++ ){
      printf("\t iBin=%d,\t D=%.0f,\t LF=%.0f,\t HF=%.0f,\t iD=%.0f,\t iLF=%.0f,\t iHF=%.0f,\t iLFUp=%.0f,\t iHFUp=%.0f,\t iLFDown=%.0f,\t iHFDown=%.0f,\t (D-LF)/HF=%.2f,\t i(D-LF)/HF=%.3f,\t LFUp=%.3f,\t LFDown=%.3f,\t cum=%.2f,\t cumLFUp=%.2f,\t cumLFDown=%.2f \n", 
	     iBin, h_csv_data_all->GetBinContent(iBin+1), h_csv_mc_nonb_all->GetBinContent(iBin+1), h_csv_mc_b_all->GetBinContent(iBin+1),
	     h_csv_data_all->Integral(iBin+1,nBins), h_csv_mc_nonb_all->Integral(iBin+1,nBins), h_csv_mc_b_all->Integral(iBin+1,nBins),
	     h_csv_mc_nonb_all_LFUp->Integral(iBin+1,nBins), h_csv_mc_b_all_LFUp->Integral(iBin+1,nBins),
	     h_csv_mc_nonb_all_LFDown->Integral(iBin+1,nBins), h_csv_mc_b_all_LFDown->Integral(iBin+1,nBins),
	     (h_csv_data_all->GetBinContent(iBin+1) - h_csv_mc_nonb_all->GetBinContent(iBin+1))/h_csv_mc_b_all->GetBinContent(iBin+1),
	     (h_csv_data_all->Integral(iBin+1,nBins) - h_csv_mc_nonb_all->Integral(iBin+1,nBins))/h_csv_mc_b_all->Integral(iBin+1,nBins),
	     (h_csv_data_all->Integral(iBin+1,nBins) - h_csv_mc_nonb_all_LFUp->Integral(iBin+1,nBins))/h_csv_mc_b_all_LFUp->Integral(iBin+1,nBins),
	     (h_csv_data_all->Integral(iBin+1,nBins) - h_csv_mc_nonb_all_LFDown->Integral(iBin+1,nBins))/h_csv_mc_b_all_LFDown->Integral(iBin+1,nBins),
	     h_csv_ratio_cumulative->GetBinContent(iBin+1), h_csv_ratio_cumulative_LFUp->GetBinContent(iBin+1), h_csv_ratio_cumulative_LFDown->GetBinContent(iBin+1) );

    }
  }

  int binCSVL = h_csv_ratio_cumulative->FindBin(0.605);
  int binCSVM = h_csv_ratio_cumulative->FindBin(0.89);
  int binCSVT = h_csv_ratio_cumulative->FindBin(0.97);

  double csvL = h_csv_ratio_cumulative->GetBinContent(binCSVL);
  double csvL_LFUp = h_csv_ratio_cumulative_LFUp->GetBinContent(binCSVL) - csvL;
  double csvL_JESUp = h_csv_ratio_cumulative_JESUp->GetBinContent(binCSVL) - csvL;
  double csvL_LFDown = h_csv_ratio_cumulative_LFDown->GetBinContent(binCSVL) - csvL;
  double csvL_JESDown = h_csv_ratio_cumulative_JESDown->GetBinContent(binCSVL) - csvL;

  double csvL_Up = sqrt( csvL_LFUp*csvL_LFUp + csvL_JESUp*csvL_JESUp );
  double csvL_Down = sqrt( csvL_LFDown*csvL_LFDown + csvL_JESDown*csvL_JESDown );

  double csvL_Err = 0.5 * (csvL_Up + csvL_Down);

  printf(" CSVL: SFb = %.3f +/- %.3f \n", csvL, csvL_Err );

  double csvM = h_csv_ratio_cumulative->GetBinContent(binCSVM);
  double csvM_LFUp = h_csv_ratio_cumulative_LFUp->GetBinContent(binCSVM) - csvM;
  double csvM_JESUp = h_csv_ratio_cumulative_JESUp->GetBinContent(binCSVM) - csvM;
  double csvM_LFDown = h_csv_ratio_cumulative_LFDown->GetBinContent(binCSVM) - csvM;
  double csvM_JESDown = h_csv_ratio_cumulative_JESDown->GetBinContent(binCSVM) - csvM;

  double csvM_Up = sqrt( csvM_LFUp*csvM_LFUp + csvM_JESUp*csvM_JESUp );
  double csvM_Down = sqrt( csvM_LFDown*csvM_LFDown + csvM_JESDown*csvM_JESDown );

  double csvM_Err = 0.5 * (csvM_Up + csvM_Down);

  printf(" CSVM: SFb = %.3f +/- %.3f \n", csvM, csvM_Err );

  double csvT = h_csv_ratio_cumulative->GetBinContent(binCSVT);
  double csvT_LFUp = h_csv_ratio_cumulative_LFUp->GetBinContent(binCSVT) - csvT;
  double csvT_JESUp = h_csv_ratio_cumulative_JESUp->GetBinContent(binCSVT) - csvT;
  double csvT_LFDown = h_csv_ratio_cumulative_LFDown->GetBinContent(binCSVT) - csvT;
  double csvT_JESDown = h_csv_ratio_cumulative_JESDown->GetBinContent(binCSVT) - csvT;

  double csvT_Up = sqrt( csvT_LFUp*csvT_LFUp + csvT_JESUp*csvT_JESUp );
  double csvT_Down = sqrt( csvT_LFDown*csvT_LFDown + csvT_JESDown*csvT_JESDown );

  double csvT_Err = 0.5 * (csvT_Up + csvT_Down);

  printf(" CSVT: SFb = %.3f +/- %.3f \n", csvT, csvT_Err );


  hist_name.push_back("csv_ratio_all");

  hist_name.push_back("csv_ratio_cumulative_all");


  h_csv_ratio[numHists-2] = (TH1D*)h_csv_ratio_all->Clone( Form("h_%s",hist_name[numHists-2].Data()) );

  h_csv_ratio_LFUp[numHists-2] = (TH1D*)h_csv_ratio_all_LFUp->Clone( Form("h_%s_LFUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_LFDown[numHists-2] = (TH1D*)h_csv_ratio_all_LFDown->Clone( Form("h_%s_LFDown",hist_name[numHists-2].Data()) );

  h_csv_ratio_JESUp[numHists-2] = (TH1D*)h_csv_ratio_all_JESUp->Clone( Form("h_%s_JESUp",hist_name[numHists-2].Data()) );
  h_csv_ratio_JESDown[numHists-2] = (TH1D*)h_csv_ratio_all_JESDown->Clone( Form("h_%s_JESDown",hist_name[numHists-2].Data()) );


  h_csv_ratio[numHists-1] = (TH1D*)h_csv_ratio_cumulative->Clone( Form("h_%s",hist_name[numHists-1].Data()) );
  h_csv_ratio[numHists-1]->SetMarkerStyle(20);


  for( int iHist=0; iHist<numHists-1; iHist++ ){
    h_csv_ratio_LF[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_LF",hist_name[numHists-2].Data()) );

    for( int iBin=0; iBin<nBins; iBin++ ){
      double content = h_csv_ratio_LF[iHist]->GetBinContent(iBin+1);
      double hfUp = fabs( content - h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1) );
      double hfDown = fabs( content - h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1) );

      double hfUnc = 0.5 * ( hfUp + hfDown );
      h_csv_ratio_LF[iHist]->SetBinError(iBin+1,hfUnc);
    }
  }


  // for( int iHist=0; iHist<numHists-1; iHist++ ){
  //   h_csv_ratio_Stats1Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Up",hist_name[numHists-2].Data()) );
  //   h_csv_ratio_Stats1Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats1Down",hist_name[numHists-2].Data()) );
  //   h_csv_ratio_Stats2Up[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Up",hist_name[numHists-2].Data()) );
  //   h_csv_ratio_Stats2Down[iHist] = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_%s_Stats2Down",hist_name[numHists-2].Data()) );

  //   for( int iBin=0; iBin<nBins; iBin++ ){
  //     double center = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
  //     double content = h_csv_ratio[iHist]->GetBinContent(iBin+1);

  //     double delta = h_csv_ratio[iHist]->GetBinError(iBin+1);
  //     double stat1Up   = content + delta * ( 1 - 2*center );
  //     double stat1Down = content + delta * ( 2*center - 1 );

  //     double stat2Up   = content + delta * ( 1 - 6*center*(1-center) );
  //     double stat2Down = content - delta * ( 1 - 6*center*(1-center) );

  //     h_csv_ratio_Stats1Up[iHist]->SetBinContent(iBin+1,stat1Up);
  //     h_csv_ratio_Stats1Down[iHist]->SetBinContent(iBin+1,stat1Down);

  //     h_csv_ratio_Stats2Up[iHist]->SetBinContent(iBin+1,stat2Up);
  //     h_csv_ratio_Stats2Down[iHist]->SetBinContent(iBin+1,stat2Down);
  //   }
  // }



  int n = 100000;
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(n);



  TH1D* h_csv_ratio_final[numHists];
  TH1D* h_csv_ratio_final_LF[numHists];
  TH1D* h_csv_ratio_final_LFUp[numHists];
  TH1D* h_csv_ratio_final_LFDown[numHists];
  TH1D* h_csv_ratio_final_JESUp[numHists];
  TH1D* h_csv_ratio_final_JESDown[numHists];
  TH1D* h_csv_ratio_final_Stats1Up[numHists];
  TH1D* h_csv_ratio_final_Stats1Down[numHists];
  TH1D* h_csv_ratio_final_Stats2Up[numHists];
  TH1D* h_csv_ratio_final_Stats2Down[numHists];

  int NumFinalBins = 1000;


  TCanvas *c1 = new TCanvas("c1");
  gPad->SetRightMargin(.05);

  TF1* btagSF = new TF1("btagSF","0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396",0.15,0.95);
  TF1* btagSFUp   = new TF1("btagSFUp","1.02*(0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396)",0.15,0.95);
  TF1* btagSFDown = new TF1("btagSFUp","0.98*(0.607239*x*x*x*x+-1.49984*x*x*x+1.3473*x*x+-0.599888*x+1.09396)",0.15,0.95);
  h_csv_ratio_cumulative->SetStats(0);
  h_csv_ratio_cumulative->GetYaxis()->SetRangeUser(0.78,1.1);
  h_csv_ratio_cumulative->GetXaxis()->SetRangeUser(0.1,1.00);
  h_csv_ratio_cumulative->SetMarkerStyle(20);
  if( isCSV ) h_csv_ratio_cumulative->SetTitle(";CSV;Heavy flavor scale factor");
  else        h_csv_ratio_cumulative->SetTitle(";CMVAv2;Heavy flavor scale factor");
  
  btagSFUp->SetLineStyle(2);
  btagSFDown->SetLineStyle(2);

  btagSFUp->SetLineColor(kMagenta-2);
  btagSFDown->SetLineColor(kMagenta-2);


  //TH1D* h_csv_ratio_cumulative_LF = (TH1D*)h_csv_ratio_cumulative_LFUp->Clone("h_csv_ratio_cumulative_LF");

  for( int iBin=0; iBin<nBins; iBin++ ){
    double center = h_csv_ratio_cumulative->GetBinCenter(iBin+1);
    if( center>0.95 || (isCSV && center<0.15) ){
      h_csv_ratio_cumulative->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_LFUp->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_LFDown->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_JESUp->SetBinContent( iBin+1, 0 );
      h_csv_ratio_cumulative_JESDown->SetBinContent( iBin+1, 0 );
    }
  }


  h_csv_ratio_cumulative_LFUp->SetLineColor(kGreen+3);
  h_csv_ratio_cumulative_LFDown->SetLineColor(kGreen+3);
  h_csv_ratio_cumulative_JESUp->SetLineColor(kRed);
  h_csv_ratio_cumulative_JESDown->SetLineColor(kBlue);

  h_csv_ratio_cumulative_LFUp->SetLineWidth(2);
  h_csv_ratio_cumulative_LFDown->SetLineWidth(2);
  h_csv_ratio_cumulative_JESUp->SetLineWidth(2);
  h_csv_ratio_cumulative_JESDown->SetLineWidth(2);


  h_csv_ratio_cumulative->Draw("pe1");
  btagSF->Draw("same");
  btagSFUp->Draw("same");
  btagSFDown->Draw("same");

  h_csv_ratio_cumulative_LFUp->Draw("][histsame");
  h_csv_ratio_cumulative_LFDown->Draw("][histsame");
  h_csv_ratio_cumulative_JESUp->Draw("][histsame");
  h_csv_ratio_cumulative_JESDown->Draw("][histsame");
  
  TLegend *legend2 = new TLegend(0.2,0.75,0.87,0.89);

  legend2->SetFillColor(kWhite);
  legend2->SetLineColor(kWhite);
  legend2->SetShadowColor(kWhite);
  legend2->SetTextFont(42);
  legend2->SetTextSize(0.05);

  legend2->SetNColumns(3);

  legend2->AddEntry(h_csv_ratio_cumulative,"Scale Factor","pe1");
  legend2->AddEntry(h_csv_ratio_cumulative_JESUp,"JES Up","l");
  legend2->AddEntry(h_csv_ratio_cumulative_JESDown,"JES Down","l");
  legend2->AddEntry(h_csv_ratio_cumulative_LFUp,"LF Err","l");
  legend2->AddEntry(btagSF,"BTAG POG","l");
  legend2->AddEntry(btagSFUp,"BTAG POG Err","l");

  legend2->Draw();

  TString myimg = dirprefix + "hfSF_cumulative.png";
  c1->Print(myimg);

  myimg = dirprefix + "hfSF_cumulative.pdf";
  c1->Print(myimg);
  
  double xMin = csvbins_new[0];
  if(!isCSV) xMin = cMVAbins_new[0];
  for( int iHist=0; iHist<numHists-1; iHist++ ){
    // delete
    if( iHist>numHists-3 ) continue;
    //continue;
    h_csv_ratio_final[iHist] = new TH1D( Form("h_%s_final",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_JESUp[iHist] = new TH1D( Form("h_%s_final_JESUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_JESDown[iHist] = new TH1D( Form("h_%s_final_JESDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_LFUp[iHist] = new TH1D( Form("h_%s_final_LFUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_LFDown[iHist] = new TH1D( Form("h_%s_final_LFDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_Stats1Up[iHist] = new TH1D( Form("h_%s_final_Stats1Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_Stats1Down[iHist] = new TH1D( Form("h_%s_final_Stats1Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_Stats2Up[iHist] = new TH1D( Form("h_%s_final_Stats2Up",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    h_csv_ratio_final_Stats2Down[iHist] = new TH1D( Form("h_%s_final_Stats2Down",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );

    c_csv_ratio_final[iHist] = new TH1D( Form("c_%s_final",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    c_csv_ratio_final_CUp[iHist] = new TH1D( Form("c_%s_final_CUp",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );
    c_csv_ratio_final_CDown[iHist] = new TH1D( Form("c_%s_final_CDown",hist_name[iHist].Data()), ";CSV", NumFinalBins, xMin, 1.01 );

    std::vector<double> x; x.clear();
    std::vector<double> y; y.clear();
    std::vector<double> y_JESUp; y_JESUp.clear();
    std::vector<double> y_JESDown; y_JESDown.clear();
    std::vector<double> y_LFUp; y_LFUp.clear();
    std::vector<double> y_LFDown; y_LFDown.clear();
    std::vector<double> y_Stats1Up; y_Stats1Up.clear();
    std::vector<double> y_Stats1Down; y_Stats1Down.clear();
    std::vector<double> y_Stats2Up; y_Stats2Up.clear();
    std::vector<double> y_Stats2Down; y_Stats2Down.clear();

    for( int iBin=0; iBin<nBins; iBin++ ){
      double center = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
      if( isCSV && center<0 ) continue;
      double sf = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double sf_JESUp = h_csv_ratio_JESUp[iHist]->GetBinContent(iBin+1);
      double sf_JESDown = h_csv_ratio_JESDown[iHist]->GetBinContent(iBin+1);
      double sf_LFUp = h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1);
      double sf_LFDown = h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1);
      double sf_Stats1Up = h_csv_ratio_Stats1Up[iHist]->GetBinContent(iBin+1);
      double sf_Stats1Down = h_csv_ratio_Stats1Down[iHist]->GetBinContent(iBin+1);
      double sf_Stats2Up = h_csv_ratio_Stats2Up[iHist]->GetBinContent(iBin+1);
      double sf_Stats2Down = h_csv_ratio_Stats2Down[iHist]->GetBinContent(iBin+1);

      x.push_back(center);
      y.push_back(sf);
      y_JESUp.push_back(sf_JESUp);
      y_JESDown.push_back(sf_JESDown);
      y_LFUp.push_back(sf_LFUp);
      y_LFDown.push_back(sf_LFDown);
      y_Stats1Up.push_back(sf_Stats1Up);
      y_Stats1Down.push_back(sf_Stats1Down);
      y_Stats2Up.push_back(sf_Stats2Up);
      y_Stats2Down.push_back(sf_Stats2Down);
    }

    //ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kLINEAR;
    //ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kCSPLINE;
    //ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kPOLYNOMIAL;
    ROOT::Math::Interpolation::Type iType = ROOT::Math::Interpolation::kAKIMA;

    ROOT::Math::Interpolator * m_i =  new ROOT::Math::Interpolator(x,y,iType);
    ROOT::Math::Interpolator * m_i_JESUp =  new ROOT::Math::Interpolator(x,y_JESUp,iType);
    ROOT::Math::Interpolator * m_i_JESDown =  new ROOT::Math::Interpolator(x,y_JESDown,iType);
    ROOT::Math::Interpolator * m_i_LFUp =  new ROOT::Math::Interpolator(x,y_LFUp,iType);
    ROOT::Math::Interpolator * m_i_LFDown =  new ROOT::Math::Interpolator(x,y_LFDown,iType);
    ROOT::Math::Interpolator * m_i_Stats1Up =  new ROOT::Math::Interpolator(x,y_Stats1Up,iType);
    ROOT::Math::Interpolator * m_i_Stats1Down =  new ROOT::Math::Interpolator(x,y_Stats1Down,iType);
    ROOT::Math::Interpolator * m_i_Stats2Up =  new ROOT::Math::Interpolator(x,y_Stats2Up,iType);
    ROOT::Math::Interpolator * m_i_Stats2Down =  new ROOT::Math::Interpolator(x,y_Stats2Down,iType);



    h_csv_ratio_final[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_JESDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_LFUp[iHist]->SetLineWidth(2);
    h_csv_ratio_final_LFDown[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineWidth(2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineWidth(2);


    h_csv_ratio_final[iHist]->SetLineColor(kBlack);
    //h_csv_ratio_final_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_final_JESUp[iHist]->SetLineColor(kBlue);
    h_csv_ratio_final_JESDown[iHist]->SetLineColor(kBlue);
    h_csv_ratio_final_LFUp[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_LFDown[iHist]->SetLineColor(kGreen+3);
    h_csv_ratio_final_Stats1Up[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats1Down[iHist]->SetLineColor(kMagenta-2);
    h_csv_ratio_final_Stats2Up[iHist]->SetLineColor(kRed-2);
    h_csv_ratio_final_Stats2Down[iHist]->SetLineColor(kRed-2);



    double firstPoint = x[0];
    double lastPoint  = x[int(x.size())-1];
    for( int iBin=0; iBin<NumFinalBins; iBin++ ){
      double center = h_csv_ratio_final[iHist]->GetBinCenter(iBin+1);
      if( isCSV && center<0 ){
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_JESDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,h_csv_ratio_LFUp[iHist]->GetBinContent(1));
	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,h_csv_ratio_LFDown[iHist]->GetBinContent(1));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) + h_csv_ratio[iHist]->GetBinError(1));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,h_csv_ratio[iHist]->GetBinContent(1) - h_csv_ratio[iHist]->GetBinError(1));
      }
      else if( center<firstPoint ){
      	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,m_i->Eval( firstPoint ));
      	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,m_i_JESUp->Eval( firstPoint ));
      	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,m_i_JESDown->Eval( firstPoint ));
      	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,m_i_LFUp->Eval( firstPoint ));
      	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,m_i_LFDown->Eval( firstPoint ));
      	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,m_i_Stats1Up->Eval( firstPoint ));
      	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,m_i_Stats1Down->Eval( firstPoint ));
      	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,m_i_Stats2Up->Eval( firstPoint ));
      	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,m_i_Stats2Down->Eval( firstPoint ));
      }
      else if( center>lastPoint ){
      	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,m_i->Eval( lastPoint ));
      	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,m_i_JESUp->Eval( lastPoint ));
      	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,m_i_JESDown->Eval( lastPoint ));
      	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,m_i_LFUp->Eval( lastPoint ));
      	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,m_i_LFDown->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,m_i_Stats1Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,m_i_Stats1Down->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,m_i_Stats2Up->Eval( lastPoint ));
      	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,m_i_Stats2Down->Eval( lastPoint ));
      }
      else {
	h_csv_ratio_final[iHist]->SetBinContent(iBin+1,m_i->Eval( center ));
	h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,m_i_JESUp->Eval( center ));
	h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,m_i_JESDown->Eval( center ));
	h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,m_i_LFUp->Eval( center ));
	h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,m_i_LFDown->Eval( center ));
	h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,m_i_Stats1Up->Eval( center ));
	h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,m_i_Stats1Down->Eval( center ));
	h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,m_i_Stats2Up->Eval( center ));
	h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,m_i_Stats2Down->Eval( center ));
      }
    }

    /////////////////

    /////////

    bool includeCorrectionFactor = includeCorrectionFactor_;
    if( includeCorrectionFactor ){
      if( isCSV ){
	if( hist_name[iHist]=="csv_ratio_Pt0_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 1.000746 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 1.005247 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 0.996790 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.138417 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.099359 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 1.006231 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.995849 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.996587 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.004361 ); 

	  h_csv_ratio[iHist]->Scale( 1.000746 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 1.005247 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 0.996790 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.138417 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.099359 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 1.006231 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.995849 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 0.996587 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 1.004361 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt1_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 0.992857 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 0.990285 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 0.996496 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.068413 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.045215 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.998137 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.988329 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.990576 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.994780 ); 

	  h_csv_ratio[iHist]->Scale( 0.992857 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 0.990285 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 0.996496 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.068413 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.045215 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 0.998137 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.988329 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 0.990576 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 0.994780 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt2_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 0.989188 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 0.990549 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 0.989555 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.017009 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.009182 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.991383 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.987304 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.988935 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.989339 ); 

	  h_csv_ratio[iHist]->Scale( 0.989188 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 0.990549 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 0.989555 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.017009 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.009182 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 0.991383 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.987304 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 0.988935 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 0.989339 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt3_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 1.003432 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 1.003287 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 1.003425 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.020068 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.013813 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 1.004727 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 1.002334 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 1.003159 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.003563 ); 

	  h_csv_ratio[iHist]->Scale( 1.003432 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 1.003287 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 1.003425 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.020068 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.013813 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 1.004727 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 1.002334 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 1.003159 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 1.003563 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt4_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 1.001770 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 1.001669 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 1.003100 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.025874 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.017161 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 1.004439 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.999543 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 1.002560 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.000846 ); 

	  h_csv_ratio[iHist]->Scale( 1.001770 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 1.001669 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 1.003100 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.025874 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.017161 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 1.004439 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.999543 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 1.002560 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 1.000846 ); 
	} 
      }
      else if( !isCSV ){
	if( hist_name[iHist]=="csv_ratio_Pt0_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 0.993304 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 0.993226 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 0.993601 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.225842 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.141939 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.993034 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.993551 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.992538 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.994050 ); 
 
	  h_csv_ratio[iHist]->Scale( 0.993304 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 0.993226 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 0.993601 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.225842 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.141939 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 0.993034 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.993551 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 0.992538 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 0.994050 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt1_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 0.997116 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 0.999013 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 0.994050 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.067307 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.041428 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.997233 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.997001 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.996310 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.997948 ); 
 
	  h_csv_ratio[iHist]->Scale( 0.997116 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 0.999013 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 0.994050 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.067307 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.041428 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 0.997233 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.997001 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 0.996310 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 0.997948 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt2_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 0.996351 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 0.997550 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 0.994935 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.027951 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.015781 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 0.996654 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.996118 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.995629 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 0.997152 ); 
 
	  h_csv_ratio[iHist]->Scale( 0.996351 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 0.997550 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 0.994935 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.027951 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.015781 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 0.996654 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.996118 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 0.995629 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 0.997152 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt3_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 0.999850 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 1.000160 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 0.999616 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.018657 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.011242 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 1.000221 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 0.999425 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 0.999010 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.000654 ); 
 
	  h_csv_ratio[iHist]->Scale( 0.999850 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 1.000160 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 0.999616 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.018657 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.011242 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 1.000221 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 0.999425 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 0.999010 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 1.000654 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt4_Eta0" ){ 
	  h_csv_ratio_final[iHist]->Scale( 1.002596 ); 
	  h_csv_ratio_final_JESUp[iHist]->Scale( 1.002092 ); 
	  h_csv_ratio_final_JESDown[iHist]->Scale( 1.002597 ); 
	  h_csv_ratio_final_LFUp[iHist]->Scale( 1.036044 ); 
	  h_csv_ratio_final_LFDown[iHist]->Scale( 1.024838 ); 
	  h_csv_ratio_final_Stats1Up[iHist]->Scale( 1.003410 ); 
	  h_csv_ratio_final_Stats1Down[iHist]->Scale( 1.001804 ); 
	  h_csv_ratio_final_Stats2Up[iHist]->Scale( 1.001425 ); 
	  h_csv_ratio_final_Stats2Down[iHist]->Scale( 1.003817 ); 
 
	  h_csv_ratio[iHist]->Scale( 1.002596 ); 
	  h_csv_ratio_JESUp[iHist]->Scale( 1.002092 ); 
	  h_csv_ratio_JESDown[iHist]->Scale( 1.002597 ); 
	  h_csv_ratio_LFUp[iHist]->Scale( 1.036044 ); 
	  h_csv_ratio_LFDown[iHist]->Scale( 1.024838 ); 
	  h_csv_ratio_Stats1Up[iHist]->Scale( 1.003410 ); 
	  h_csv_ratio_Stats1Down[iHist]->Scale( 1.001804 ); 
	  h_csv_ratio_Stats2Up[iHist]->Scale( 1.001425 ); 
	  h_csv_ratio_Stats2Down[iHist]->Scale( 1.003817 ); 
	} 
      }
    }
    
    ////////////////

    h_csv_ratio[iHist]->SetMarkerStyle(20);

    h_csv_ratio[iHist]->SetTitle( Form("HF %s", hist_name[iHist].Data() ) );
    //h_csv_ratio[iHist]->SetTitle("");
    if( iHist==numHists-1 ) h_csv_ratio[iHist]->SetTitle( Form("LF %s", hist_name[iHist].Data() ) );
    h_csv_ratio[iHist]->GetYaxis()->SetTitle("Data/Simulation SF_{b}");
    if( isCSV ) h_csv_ratio[iHist]->GetXaxis()->SetTitle("CSVv2 Discriminator");
    else        h_csv_ratio[iHist]->GetXaxis()->SetTitle("CMVAv2 Discriminator");
    
    h_csv_ratio[iHist]->SetStats(0);

    double maxY = 0;
    for( int iBin=0; iBin<h_csv_ratio[iHist]->GetNbinsX()-2; iBin++ ){
      double content = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double err     = h_csv_ratio[iHist]->GetBinError(iBin+1);

      if( maxY<(content+err) ) maxY = content + err;
    }
    maxY *= 1.3;
    //double maxY = 1.3 * h_csv_ratio[iHist]->GetMaximum();
    maxY = std::min( maxY, 5. );

    h_csv_ratio[iHist]->SetMaximum(maxY);
    h_csv_ratio[iHist]->SetMinimum(0.01);

    TLegend *legend = new TLegend(0.2,0.75,0.87,0.89);


    h_csv_ratio_LFUp[iHist]->SetLineColor(kGreen+1);
    h_csv_ratio_LFDown[iHist]->SetLineColor(kGreen+1);

    //h_csv_ratio_JESUp[iHist]->SetLineColor(kRed);
    h_csv_ratio_JESUp[iHist]->SetLineColor(kBlue);
    h_csv_ratio_JESDown[iHist]->SetLineColor(kBlue);



    legend->SetFillColor(kWhite);
    legend->SetLineColor(kWhite);
    legend->SetShadowColor(kWhite);
    legend->SetTextFont(42);
    legend->SetTextSize(0.035);

    legend->SetNColumns(3);

    legend->AddEntry(h_csv_ratio[iHist],"HF SF","p");
    legend->AddEntry(h_csv_ratio_final_JESUp[iHist],"JES Unc","l");
    //legend->AddEntry(h_csv_ratio_final_JESDown[iHist],"JES Down","l");
    legend->AddEntry(h_csv_ratio_final[iHist],"HF Fit","l");
    legend->AddEntry(h_csv_ratio_final_LFUp[iHist],"LF Unc","l");
    legend->AddEntry(h_csv_ratio_final_Stats1Up[iHist],"Stats1 Unc","l");
    legend->AddEntry(h_csv_ratio_final_Stats2Up[iHist],"Stats2 Unc","l");


    //TLegend *legend2 = new TLegend(0.2,0.82,0.87,0.89);
    TLegend *legend2 = new TLegend(0.75,0.72,0.87,0.89);

    legend2->SetFillColor(kWhite);
    legend2->SetLineColor(kWhite);
    legend2->SetShadowColor(kWhite);
    legend2->SetTextFont(42);
    legend2->SetTextSize(0.04);

    //legend2->SetNColumns(3);

    legend2->AddEntry(h_csv_ratio[iHist],"HF SF","p");
    legend2->AddEntry(h_csv_ratio_final[iHist],"HF Fit","l");


    TString img;

    //////
    TString ptselectioninfo = label_ptbin[iHist];

    TLatex PTSELECTIONInfoLatex(0.16, 0.84, ptselectioninfo);
    PTSELECTIONInfoLatex.SetNDC();
    PTSELECTIONInfoLatex.SetTextFont(42);
    PTSELECTIONInfoLatex.SetTextSize(0.04);

    TString etaselectioninfo = label_etabin[iHist];

    TLatex ETASELECTIONInfoLatex(0.16, 0.78, etaselectioninfo);
    ETASELECTIONInfoLatex.SetNDC();
    ETASELECTIONInfoLatex.SetTextFont(42);
    ETASELECTIONInfoLatex.SetTextSize(0.04);


    ///////

    h_csv_ratio[iHist]->SetTitle("");
    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    //legend2->Draw();

    TLine *line = new TLine(0, 0.01, 0, maxY);
    line->SetLineStyle(7);

    LumiInfoLatex.Draw();
    CMSInfoLatex.Draw();
    PublishInfoLatex.Draw();

    PTSELECTIONInfoLatex.Draw();
    ETASELECTIONInfoLatex.Draw();

    if( isCSV ) line->Draw();

    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_only.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_only.pdf";
    c1->Print(img);


    //////    ///////    ///////    ///////


    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_JES.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_JES.pdf";
    c1->Print(img);


    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_LFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_LFDown[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_LF.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_LF.pdf";
    c1->Print(img);


    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats1.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats1.pdf";
    c1->Print(img);



    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();
 
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats2.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_Stats2.pdf";
    c1->Print(img);


    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    h_csv_ratio_final_JESUp[iHist]->Draw("histsame");
    h_csv_ratio_final_JESDown[iHist]->Draw("histsame");
    h_csv_ratio_final_LFUp[iHist]->Draw("histsame");
    h_csv_ratio_final_LFDown[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats1Down[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Up[iHist]->Draw("histsame");
    h_csv_ratio_final_Stats2Down[iHist]->Draw("histsame");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend->Draw();

    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_All.png";
    c1->Print(img);
    img = dirprefix + "hfSF_" + hist_name[iHist] + "_fit_All.pdf";
    c1->Print(img);

    int firstBin = ( isCSV ) ? 1 : 0;

    // Piecewise fitting
    int Nbins = h_csv_ratio[iHist]->GetNbinsX();
    
    TF1* fint[Nbins-1-firstBin];
    TF1* fint_JESUp[Nbins-1-firstBin];
    TF1* fint_JESDown[Nbins-1-firstBin];
    TF1* fint_LFUp[Nbins-1-firstBin];
    TF1* fint_LFDown[Nbins-1-firstBin];
    TF1* fint_Stats1Up[Nbins-1-firstBin];
    TF1* fint_Stats1Down[Nbins-1-firstBin];
    TF1* fint_Stats2Up[Nbins-1-firstBin];
    TF1* fint_Stats2Down[Nbins-1-firstBin];
    for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
      double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
      double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

      double y1 = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double y2 = h_csv_ratio[iHist]->GetBinContent(iBin+1+1);

      double slope = (y2 - y1) / (x2 - x1);
      double intercept = y1 - x1 * slope;

      fint[iBin] = new TF1(Form("fint_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint[iBin]->SetParameter(0,intercept);
      fint[iBin]->SetParameter(1,slope);

      fint[iBin]->SetLineColor(kRed);
      //printf("\t central %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );

      /// JESUp
      double y1_JESUp = h_csv_ratio_JESUp[iHist]->GetBinContent(iBin+1);
      double y2_JESUp = h_csv_ratio_JESUp[iHist]->GetBinContent(iBin+1+1);

      double slope_JESUp = (y2_JESUp - y1_JESUp) / (x2 - x1);
      double intercept_JESUp = y1_JESUp - x1 * slope_JESUp;

      fint_JESUp[iBin] = new TF1(Form("fint_JESUp_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_JESUp[iBin]->SetParameter(0,intercept_JESUp);
      fint_JESUp[iBin]->SetParameter(1,slope_JESUp);

      fint_JESUp[iBin]->SetLineColor(kRed);
      //printf("\t JESUp %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );

      /// JESDown
      double y1_JESDown = h_csv_ratio_JESDown[iHist]->GetBinContent(iBin+1);
      double y2_JESDown = h_csv_ratio_JESDown[iHist]->GetBinContent(iBin+1+1);

      double slope_JESDown = (y2_JESDown - y1_JESDown) / (x2 - x1);
      double intercept_JESDown = y1_JESDown - x1 * slope_JESDown;

      fint_JESDown[iBin] = new TF1(Form("fint_JESDown_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_JESDown[iBin]->SetParameter(0,intercept_JESDown);
      fint_JESDown[iBin]->SetParameter(1,slope_JESDown);

      fint_JESDown[iBin]->SetLineColor(kRed);
      //printf("\t JESDown %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );

      /// LFUp
      double y1_LFUp = h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1);
      double y2_LFUp = h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1+1);

      double slope_LFUp = (y2_LFUp - y1_LFUp) / (x2 - x1);
      double intercept_LFUp = y1_LFUp - x1 * slope_LFUp;

      fint_LFUp[iBin] = new TF1(Form("fint_LFUp_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_LFUp[iBin]->SetParameter(0,intercept_LFUp);
      fint_LFUp[iBin]->SetParameter(1,slope_LFUp);

      fint_LFUp[iBin]->SetLineColor(kRed);
      //printf("\t LFUp %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );

      /// LFDown
      double y1_LFDown = h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1);
      double y2_LFDown = h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1+1);

      double slope_LFDown = (y2_LFDown - y1_LFDown) / (x2 - x1);
      double intercept_LFDown = y1_LFDown - x1 * slope_LFDown;

      fint_LFDown[iBin] = new TF1(Form("fint_LFDown_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_LFDown[iBin]->SetParameter(0,intercept_LFDown);
      fint_LFDown[iBin]->SetParameter(1,slope_LFDown);

      fint_LFDown[iBin]->SetLineColor(kRed);
      //printf("\t LFDown %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );



      /// Stats1Up
      double y1_Stats1Up = h_csv_ratio_Stats1Up[iHist]->GetBinContent(iBin+1);
      double y2_Stats1Up = h_csv_ratio_Stats1Up[iHist]->GetBinContent(iBin+1+1);

      double slope_Stats1Up = (y2_Stats1Up - y1_Stats1Up) / (x2 - x1);
      double intercept_Stats1Up = y1_Stats1Up - x1 * slope_Stats1Up;

      fint_Stats1Up[iBin] = new TF1(Form("fint_Stats1Up_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_Stats1Up[iBin]->SetParameter(0,intercept_Stats1Up);
      fint_Stats1Up[iBin]->SetParameter(1,slope_Stats1Up);

      fint_Stats1Up[iBin]->SetLineColor(kRed);
      //printf("\t Stats1Up %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );

      /// Stats1Down
      double y1_Stats1Down = h_csv_ratio_Stats1Down[iHist]->GetBinContent(iBin+1);
      double y2_Stats1Down = h_csv_ratio_Stats1Down[iHist]->GetBinContent(iBin+1+1);

      double slope_Stats1Down = (y2_Stats1Down - y1_Stats1Down) / (x2 - x1);
      double intercept_Stats1Down = y1_Stats1Down - x1 * slope_Stats1Down;

      fint_Stats1Down[iBin] = new TF1(Form("fint_Stats1Down_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_Stats1Down[iBin]->SetParameter(0,intercept_Stats1Down);
      fint_Stats1Down[iBin]->SetParameter(1,slope_Stats1Down);

      fint_Stats1Down[iBin]->SetLineColor(kRed);
      //printf("\t Stats1Down %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );


      /// Stats2Up
      double y1_Stats2Up = h_csv_ratio_Stats2Up[iHist]->GetBinContent(iBin+1);
      double y2_Stats2Up = h_csv_ratio_Stats2Up[iHist]->GetBinContent(iBin+1+1);

      double slope_Stats2Up = (y2_Stats2Up - y1_Stats2Up) / (x2 - x1);
      double intercept_Stats2Up = y1_Stats2Up - x1 * slope_Stats2Up;

      fint_Stats2Up[iBin] = new TF1(Form("fint_Stats2Up_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_Stats2Up[iBin]->SetParameter(0,intercept_Stats2Up);
      fint_Stats2Up[iBin]->SetParameter(1,slope_Stats2Up);

      fint_Stats2Up[iBin]->SetLineColor(kRed);
      //printf("\t Stats2Up %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );

      /// Stats2Down
      double y1_Stats2Down = h_csv_ratio_Stats2Down[iHist]->GetBinContent(iBin+1);
      double y2_Stats2Down = h_csv_ratio_Stats2Down[iHist]->GetBinContent(iBin+1+1);

      double slope_Stats2Down = (y2_Stats2Down - y1_Stats2Down) / (x2 - x1);
      double intercept_Stats2Down = y1_Stats2Down - x1 * slope_Stats2Down;

      fint_Stats2Down[iBin] = new TF1(Form("fint_Stats2Down_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_Stats2Down[iBin]->SetParameter(0,intercept_Stats2Down);
      fint_Stats2Down[iBin]->SetParameter(1,slope_Stats2Down);

      fint_Stats2Down[iBin]->SetLineColor(kRed);
      //printf("\t Stats2Down %s \t intercept = %.3f, slope = %.3f, x1 = %.2f, x2 = %.2f, y1 = %.2f, y2 = %.2f \n", hist_name[iHist].Data(), intercept, slope, x1, x2, y1, y2 );



    }


    h_csv_ratio[iHist]->GetYaxis()->SetRangeUser(0.4,1.5);
    h_csv_ratio[iHist]->GetXaxis()->SetRangeUser(0.8,1.01);
    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    for( int iBin=1; iBin<Nbins-1; iBin++ ) fint[iBin]->Draw("same");
    h_csv_ratio[iHist]->Draw("pe1same");

    legend2->Draw();
 
    img = dirprefix + "interpolation_hfSF_" + hist_name[iHist] + "_fit.png";
    c1->Print(img);
    img = dirprefix + "interpolation_hfSF_" + hist_name[iHist] + "_fit.pdf";
    c1->Print(img);

    ///


    // Enforce non-negative weights
    for( int iBin=0; iBin<NumFinalBins; iBin++ ){
      if( h_csv_ratio_final[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_JESUp[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_JESUp[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_JESDown[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_JESDown[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_LFUp[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_LFUp[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_LFDown[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_LFDown[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats1Up[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats1Up[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats1Down[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats1Down[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats2Up[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats2Up[iHist]->SetBinContent(iBin+1,0.);
      if( h_csv_ratio_final_Stats2Down[iHist]->GetBinContent(iBin+1)<0 ) h_csv_ratio_final_Stats2Down[iHist]->SetBinContent(iBin+1,0.);
    }


    /////////
    // double scale_inv = 1.0;
    // if( hist_name[iHist]=="csv_ratio_Pt0_Eta0" )      scale_inv = 9.953936e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt1_Eta0" ) scale_inv = 1.007696e+00;
    // else if( hist_name[iHist]=="csv_ratio_Pt2_Eta0" ) scale_inv = 9.986852e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt3_Eta0" ) scale_inv = 9.891133e-01;
    // else if( hist_name[iHist]=="csv_ratio_Pt4_Eta0" ) scale_inv = 9.808719e-01;


    // double scale_all = 1.0 / scale_inv;
    
    // h_csv_ratio_final[iHist]->Scale( scale_all );
    // h_csv_ratio_final_JESUp[iHist]->Scale( scale_all );
    // h_csv_ratio_final_JESDown[iHist]->Scale( scale_all );
    // h_csv_ratio_final_LFUp[iHist]->Scale( scale_all );
    // h_csv_ratio_final_LFDown[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats1Up[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats1Down[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats2Up[iHist]->Scale( scale_all );
    // h_csv_ratio_final_Stats2Down[iHist]->Scale( scale_all );
	
    h_csv_ratio_final[iHist]->Write(Form("%s_final",hist_name[iHist].Data()));
    h_csv_ratio_final_JESUp[iHist]->Write(Form("%s_final_JESUp",hist_name[iHist].Data()));
    h_csv_ratio_final_JESDown[iHist]->Write(Form("%s_final_JESDown",hist_name[iHist].Data()));
    h_csv_ratio_final_LFUp[iHist]->Write(Form("%s_final_LFUp",hist_name[iHist].Data()));
    h_csv_ratio_final_LFDown[iHist]->Write(Form("%s_final_LFDown",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Up[iHist]->Write(Form("%s_final_Stats1Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats1Down[iHist]->Write(Form("%s_final_Stats1Down",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Up[iHist]->Write(Form("%s_final_Stats2Up",hist_name[iHist].Data()));
    h_csv_ratio_final_Stats2Down[iHist]->Write(Form("%s_final_Stats2Down",hist_name[iHist].Data()));

    for( int iBin=0; iBin<c_csv_ratio_final[iHist]->GetNbinsX(); iBin++ ) c_csv_ratio_final[iHist]->SetBinContent(iBin+1,1.0);

    TH1D* h_ratio_JESUp = (TH1D*)h_csv_ratio_final_JESUp[iHist]->Clone("h_ratio_JESUp_"+hist_name[iHist]);
    TH1D* h_ratio_JESDown = (TH1D*)h_csv_ratio_final_JESDown[iHist]->Clone("h_ratio_JESDown_"+hist_name[iHist]);
    TH1D* h_ratio_LFUp = (TH1D*)h_csv_ratio_final_LFUp[iHist]->Clone("h_ratio_LFUp_"+hist_name[iHist]);
    TH1D* h_ratio_LFDown = (TH1D*)h_csv_ratio_final_LFDown[iHist]->Clone("h_ratio_LFDown_"+hist_name[iHist]);
    TH1D* h_ratio_Stats1Up = (TH1D*)h_csv_ratio_final_Stats1Up[iHist]->Clone("h_ratio_Stats1Up_"+hist_name[iHist]);
    TH1D* h_ratio_Stats1Down = (TH1D*)h_csv_ratio_final_Stats1Down[iHist]->Clone("h_ratio_Stats1Down_"+hist_name[iHist]);
    TH1D* h_ratio_Stats2Up = (TH1D*)h_csv_ratio_final_Stats2Up[iHist]->Clone("h_ratio_Stats2Up_"+hist_name[iHist]);
    TH1D* h_ratio_Stats2Down = (TH1D*)h_csv_ratio_final_Stats2Down[iHist]->Clone("h_ratio_Stats2Down_"+hist_name[iHist]);

    TH1D* h_total_cUp   = (TH1D*)c_csv_ratio_final[iHist]->Clone("h_total_cUp_"+hist_name[iHist]);
    TH1D* h_total_cDown = (TH1D*)c_csv_ratio_final[iHist]->Clone("h_total_cDown_"+hist_name[iHist]);

    h_ratio_JESUp->Divide(h_csv_ratio_final[iHist]);
    h_ratio_JESDown->Divide(h_csv_ratio_final[iHist]);
    h_ratio_LFUp->Divide(h_csv_ratio_final[iHist]);
    h_ratio_LFDown->Divide(h_csv_ratio_final[iHist]);
    h_ratio_Stats1Up->Divide(h_csv_ratio_final[iHist]);
    h_ratio_Stats1Down->Divide(h_csv_ratio_final[iHist]);
    h_ratio_Stats2Up->Divide(h_csv_ratio_final[iHist]);
    h_ratio_Stats2Down->Divide(h_csv_ratio_final[iHist]);

    h_ratio_JESUp->SetLineWidth(2);
    h_ratio_JESDown->SetLineWidth(2);
    h_ratio_LFUp->SetLineWidth(2);
    h_ratio_LFDown->SetLineWidth(2);
    h_ratio_Stats1Up->SetLineWidth(2);
    h_ratio_Stats1Down->SetLineWidth(2);
    h_ratio_Stats2Up->SetLineWidth(2);
    h_ratio_Stats2Down->SetLineWidth(2);

    h_ratio_JESUp->SetLineColor(kBlue);
    h_ratio_JESDown->SetLineColor(kBlue);
    h_ratio_LFUp->SetLineColor(kGreen+1);
    h_ratio_LFDown->SetLineColor(kGreen+1);
    h_ratio_Stats1Up->SetLineColor(kMagenta-2);
    h_ratio_Stats1Down->SetLineColor(kMagenta-2);
    h_ratio_Stats2Up->SetLineColor(kRed-2);
    h_ratio_Stats2Down->SetLineColor(kRed-2);



    for( int iBin=0; iBin<c_csv_ratio_final[iHist]->GetNbinsX(); iBin++ ){
      double center = h_csv_ratio_final[iHist]->GetBinCenter(iBin+1);
      double nominal = 1.;
      double diff_JESUp = h_ratio_JESUp->GetBinContent(iBin+1)     - nominal;
      double diff_JESDown = h_ratio_JESDown->GetBinContent(iBin+1) - nominal;
      double diff_LFUp = h_ratio_LFUp->GetBinContent(iBin+1)     - nominal;
      double diff_LFDown = h_ratio_LFDown->GetBinContent(iBin+1) - nominal;
      double diff_Stats1Up = h_ratio_Stats1Up->GetBinContent(iBin+1)     - nominal;
      double diff_Stats1Down = h_ratio_Stats1Down->GetBinContent(iBin+1) - nominal;
      double diff_Stats2Up = h_ratio_Stats2Up->GetBinContent(iBin+1)     - nominal;
      double diff_Stats2Down = h_ratio_Stats2Down->GetBinContent(iBin+1) - nominal;

      double sf = h_csv_ratio_final[iHist]->GetBinContent(iBin+1);
      /*
      double sf = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double sf_JESUp = h_csv_ratio_JESUp[iHist]->GetBinContent(iBin+1) - sf;
      double sf_JESDown = h_csv_ratio_JESDown[iHist]->GetBinContent(iBin+1) - sf;
      double sf_LFUp = h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1) - sf;
      double sf_LFDown = h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1) - sf;
      double sf_Stats1Up = h_csv_ratio_Stats1Up[iHist]->GetBinContent(iBin+1) - sf;
      double sf_Stats1Down = h_csv_ratio_Stats1Down[iHist]->GetBinContent(iBin+1) - sf;
      double sf_Stats2Up = h_csv_ratio_Stats2Up[iHist]->GetBinContent(iBin+1) - sf;
      double sf_Stats2Down = h_csv_ratio_Stats2Down[iHist]->GetBinContent(iBin+1) - sf;

      double sf_JES    = 0.5 * ( fabs(sf_JESUp) + fabs(sf_JESDown) );
      double sf_LF     = 0.5 * ( fabs(sf_LFUp) + fabs(sf_LFDown) );
      double sf_Stats1 = 0.5 * ( fabs(sf_Stats1Up) + fabs(sf_Stats1Down) );
      double sf_Stats2 = 0.5 * ( fabs(sf_Stats2Up) + fabs(sf_Stats2Down) );
      */

      double sum_up2 = 0., sum_down2 = 0.;
      if( diff_JESUp>0 ) sum_up2   += diff_JESUp*diff_JESUp;
      else               sum_down2 += diff_JESUp*diff_JESUp;
      if( diff_JESDown>0 ) sum_up2   += diff_JESDown*diff_JESDown;
      else                 sum_down2 += diff_JESDown*diff_JESDown;

      if( diff_LFUp>0 ) sum_up2   += diff_LFUp*diff_LFUp;
      else              sum_down2 += diff_LFUp*diff_LFUp;
      if( diff_LFDown>0 ) sum_up2   += diff_LFDown*diff_LFDown;
      else                sum_down2 += diff_LFDown*diff_LFDown;

      if( diff_Stats1Up>0 ) sum_up2   += diff_Stats1Up*diff_Stats1Up;
      else                  sum_down2 += diff_Stats1Up*diff_Stats1Up;
      if( diff_Stats1Down>0 ) sum_up2   += diff_Stats1Down*diff_Stats1Down;
      else                    sum_down2 += diff_Stats1Down*diff_Stats1Down;

      if( diff_Stats2Up>0 ) sum_up2   += diff_Stats2Up*diff_Stats2Up;
      else                  sum_down2 += diff_Stats2Up*diff_Stats2Up;
      if( diff_Stats2Down>0 ) sum_up2   += diff_Stats2Down*diff_Stats2Down;
      else                    sum_down2 += diff_Stats2Down*diff_Stats2Down;


      double total_cUp_bin = nominal+sqrt(sum_up2);
      double total_cDown_bin = nominal-sqrt(sum_down2);

      h_total_cUp->SetBinContent(iBin+1,total_cUp_bin);
      h_total_cDown->SetBinContent(iBin+1,total_cDown_bin);

    }



    for( int iBin=0; iBin<h_csv_ratio[iHist]->GetNbinsX(); iBin++ ){
      double sf = h_csv_ratio[iHist]->GetBinContent(iBin+1);
      double diff_JESUp = h_csv_ratio_JESUp[iHist]->GetBinContent(iBin+1) - sf;
      double diff_JESDown = h_csv_ratio_JESDown[iHist]->GetBinContent(iBin+1) - sf;
      double diff_LFUp = h_csv_ratio_LFUp[iHist]->GetBinContent(iBin+1) - sf;
      double diff_LFDown = h_csv_ratio_LFDown[iHist]->GetBinContent(iBin+1) - sf;
      double diff_Stats1Up = h_csv_ratio_Stats1Up[iHist]->GetBinContent(iBin+1) - sf;
      double diff_Stats1Down = h_csv_ratio_Stats1Down[iHist]->GetBinContent(iBin+1) - sf;
      double diff_Stats2Up = h_csv_ratio_Stats2Up[iHist]->GetBinContent(iBin+1) - sf;
      double diff_Stats2Down = h_csv_ratio_Stats2Down[iHist]->GetBinContent(iBin+1) - sf;

      double diff_JES    = 0.5 * ( fabs(diff_JESUp) + fabs(diff_JESDown) );
      double diff_LF     = 0.5 * ( fabs(diff_LFUp) + fabs(diff_LFDown) );
      double diff_Stats1 = 0.5 * ( fabs(diff_Stats1Up) + fabs(diff_Stats1Down) );
      double diff_Stats2 = 0.5 * ( fabs(diff_Stats2Up) + fabs(diff_Stats2Down) );

      double sum_up2 = 0., sum_down2 = 0.;
      if( diff_JESUp>0 ) sum_up2   += diff_JESUp*diff_JESUp;
      else               sum_down2 += diff_JESUp*diff_JESUp;
      if( diff_JESDown>0 ) sum_up2   += diff_JESDown*diff_JESDown;
      else                 sum_down2 += diff_JESDown*diff_JESDown;

      if( diff_LFUp>0 ) sum_up2   += diff_LFUp*diff_LFUp;
      else              sum_down2 += diff_LFUp*diff_LFUp;
      if( diff_LFDown>0 ) sum_up2   += diff_LFDown*diff_LFDown;
      else                sum_down2 += diff_LFDown*diff_LFDown;

      if( diff_Stats1Up>0 ) sum_up2   += diff_Stats1Up*diff_Stats1Up;
      else                  sum_down2 += diff_Stats1Up*diff_Stats1Up;
      if( diff_Stats1Down>0 ) sum_up2   += diff_Stats1Down*diff_Stats1Down;
      else                    sum_down2 += diff_Stats1Down*diff_Stats1Down;

      if( diff_Stats2Up>0 ) sum_up2   += diff_Stats2Up*diff_Stats2Up;
      else                  sum_down2 += diff_Stats2Up*diff_Stats2Up;
      if( diff_Stats2Down>0 ) sum_up2   += diff_Stats2Down*diff_Stats2Down;
      else                    sum_down2 += diff_Stats2Down*diff_Stats2Down;

      double sum_err = 0.5 * ( sqrt(sum_up2) + sqrt(sum_down2) );
      if( iHist<5 && false ){
	printf(" iHist = %d,\t iBin = %d,\t SF = %.3f,\t TotalError = %.3f,\t JESError = %.3f,\t LFError = %.3f,\t Stats1Error = %.3f,\t Stats2Error = %.3f,\t binError = %.3f \n", 
	       iHist, iBin, sf, sum_err, diff_JES, diff_LF, diff_Stats1, diff_Stats2, h_csv_ratio[iHist]->GetBinError(iBin+1) );
      }
    }
    //std::cout << " " << std::endl;


    h_total_cUp->SetLineWidth(3);
    h_total_cDown->SetLineWidth(3);
    h_total_cUp->SetLineColor(kGreen+3);
    h_total_cDown->SetLineColor(kCyan+3);

    int binAtZero = h_total_cUp->FindBin(0.0000001);
    int binAtOne  = h_total_cUp->FindBin(1-0.0000001);
    double firstErr = TMath::Max( fabs(h_total_cUp->GetBinContent(binAtZero)), fabs(h_total_cDown->GetBinContent(binAtZero)) );
    double lastErr  = TMath::Max( fabs(h_total_cUp->GetBinContent(binAtOne)), fabs(h_total_cDown->GetBinContent(binAtOne)) );

    double linA  = 0.5 * fabs( firstErr + lastErr ) - 1;
    double quadA = 0.5 * fabs( firstErr - lastErr );

    //printf(" %s\t firstErr = %.1f,\t lastErr = %.1f,\t linA = %.1f,\t quadA = %.1f \n", hist_name[iHist].Data(), firstErr, lastErr, linA, quadA );

    TH1D* h_cErr1Up   = (TH1D*)c_csv_ratio_final[iHist]->Clone("h_cErr1Up_"+hist_name[iHist]);
    TH1D* h_cErr1Down = (TH1D*)c_csv_ratio_final[iHist]->Clone("h_cErr1Down_"+hist_name[iHist]);
    TH1D* h_cErr2Up   = (TH1D*)c_csv_ratio_final[iHist]->Clone("h_cErr2Up_"+hist_name[iHist]);
    TH1D* h_cErr2Down = (TH1D*)c_csv_ratio_final[iHist]->Clone("h_cErr2Down_"+hist_name[iHist]);

    for( int iBin=0; iBin<c_csv_ratio_final[iHist]->GetNbinsX(); iBin++ ){
      double center = h_total_cUp->GetBinCenter(iBin+1);
      double content = 1.0;

      double bSF = h_csv_ratio_final[iHist]->GetBinContent(iBin+1);
      double cUp = content + 2*fabs(h_total_cUp->GetBinContent(iBin+1) - content);
      double cDown = content - 2*fabs(h_total_cDown->GetBinContent(iBin+1) - content);
      if( (bSF - cUp)>0 )   cUp = bSF;
      if( (cDown - bSF)>0 ) cDown = bSF;
      cUp -= content;
      cDown -= content;

      double deltaUp = fabs( cUp );
      double deltaDown = fabs( cDown );

      // double delta = 2*TMath::Max( fabs(cUp), fabs(h_total_cDown->GetBinContent(iBin+1) - content) );
      double delta = TMath::Max( deltaUp, deltaDown );

      double cErr1Up   = ( center<0. ) ? TMath::Max( bSF, content + cUp   ) : content + delta * ( 1 - 2*center );
      double cErr1Down = ( center<0. ) ? TMath::Min( bSF, content + cDown ) : content - delta * ( 1 - 2*center );

      double cErr2Up   = ( center<0. ) ? content : content + delta * ( 1 - 6*center*(1-center) );
      double cErr2Down = ( center<0. ) ? content : content - delta * ( 1 - 6*center*(1-center) );

      //if( center>0 && center<0.03 ) printf("\t\t bin = %d,\t cErr1Up = %.1f,\t cErr1Down = %.1f,\t cErr2Up = %.1f,\t  cErr2Down = %.1f \n", iBin+1, cErr1Up, cErr1Down, cErr2Up, cErr2Down );

      h_cErr1Up->SetBinContent(iBin+1,cErr1Up);
      h_cErr1Down->SetBinContent(iBin+1,cErr1Down);

      h_cErr2Up->SetBinContent(iBin+1,cErr2Up);
      h_cErr2Down->SetBinContent(iBin+1,cErr2Down);
    }


    TLegend *legend3 = new TLegend(0.2,0.8,0.84,0.89);

    legend3->SetFillColor(kWhite);
    legend3->SetLineColor(kWhite);
    legend3->SetShadowColor(kWhite);
    legend3->SetTextFont(42);
    legend3->SetTextSize(0.035);

    legend3->SetNColumns(3);

    legend3->AddEntry(c_csv_ratio_final[iHist],"c SF","l");
    legend3->AddEntry(h_ratio_JESUp,"JES","l");
    legend3->AddEntry(h_ratio_LFUp,"LF","l");
    legend3->AddEntry(h_ratio_Stats1Up,"Stats1","l");
    legend3->AddEntry(h_ratio_Stats2Up,"Stats2","l");
    legend3->AddEntry(h_total_cUp,"Total Up","l");
    legend3->AddEntry(h_total_cDown,"Total Down","l");

    h_ratio_JESUp->SetTitle(";CSV;Uncertainty/Nominal");
    h_ratio_JESUp->SetStats(0);
    h_ratio_JESUp->GetYaxis()->SetRangeUser(0.5,1.5);
    h_ratio_JESUp->GetXaxis()->SetRangeUser(-0.041,0.9489);

    h_ratio_JESUp->Draw("hist");
    h_ratio_JESDown->Draw("histsame");
    h_ratio_LFUp->Draw("histsame");
    h_ratio_LFDown->Draw("histsame");
    h_ratio_Stats1Up->Draw("histsame");
    h_ratio_Stats1Down->Draw("histsame");
    h_ratio_Stats2Up->Draw("histsame");
    h_ratio_Stats2Down->Draw("histsame");
    c_csv_ratio_final[iHist]->Draw("histsame");
    h_total_cUp->Draw("histsame");
    h_total_cDown->Draw("histsame");

    legend3->Draw();

    img = dirprefix + "cSF_" + hist_name[iHist] + "_fit_All.png";
    if( makePlots ) c1->Print(img);
    img = dirprefix + "cSF_" + hist_name[iHist] + "_fit_All.pdf";
    if( makePlots ) c1->Print(img);




    TH1D* h_total_cUp_x2 = (TH1D*)h_total_cUp->Clone("h_total_cUp_x2_"+hist_name[iHist]);
    TH1D* h_total_cDown_x2 = (TH1D*)h_total_cDown->Clone("h_total_cDown_x2_"+hist_name[iHist]);

    for( int iBin=0; iBin<h_total_cUp_x2->GetNbinsX(); iBin++ ){
      double up_x2   = c_csv_ratio_final[iHist]->GetBinContent(iBin+1) + 2 * ( h_total_cUp->GetBinContent(iBin+1) - c_csv_ratio_final[iHist]->GetBinContent(iBin+1) );
      double down_x2 = c_csv_ratio_final[iHist]->GetBinContent(iBin+1) - 2 * ( c_csv_ratio_final[iHist]->GetBinContent(iBin+1) - h_total_cDown->GetBinContent(iBin+1) );

      double bSF = h_csv_ratio_final[iHist]->GetBinContent(iBin+1);
      double center = h_csv_ratio_final[iHist]->GetBinCenter(iBin+1);
      if( (center>=0.244 && center<=0.679) || true ){
	if( (bSF - up_x2)>0.   ) up_x2 = bSF;
	if( (down_x2 - bSF)>0. ) down_x2 = bSF;
      }

      h_total_cUp_x2->SetBinContent(iBin+1,up_x2);
      h_total_cDown_x2->SetBinContent(iBin+1,down_x2);
    }

    h_total_cUp_x2->SetLineWidth(3);
    h_total_cDown_x2->SetLineWidth(3);
    h_total_cUp_x2->SetLineColor(kGreen+3);
    //h_total_cDown_x2->SetLineColor(kCyan+3);
    h_total_cDown_x2->SetLineColor(kGreen+3);


    h_cErr1Up->SetLineWidth(2);
    h_cErr1Down->SetLineWidth(2);
    h_cErr2Up->SetLineWidth(2);
    h_cErr2Down->SetLineWidth(2);

    h_cErr1Up->SetLineColor(kRed);
    h_cErr1Down->SetLineColor(kRed);
    h_cErr2Up->SetLineColor(kBlue);
    h_cErr2Down->SetLineColor(kBlue);

    h_total_cUp_x2->SetTitle(";CSV;Scale Factor");
    h_total_cUp_x2->SetStats(0);
    h_total_cUp_x2->GetYaxis()->SetRangeUser(0.,2.0);
    //h_total_cUp_x2->GetXaxis()->SetRangeUser(-0.041,0.9489);
    h_total_cUp_x2->GetXaxis()->SetRangeUser(-0.041,1.0);

    h_total_cUp_x2->Draw("hist");
    h_total_cDown_x2->Draw("histsame");

    h_cErr1Up->Draw("histsame");
    h_cErr1Down->Draw("histsame");
    h_cErr2Up->Draw("histsame");
    h_cErr2Down->Draw("histsame");

    c_csv_ratio_final[iHist]->Draw("histsame");



    TLegend *legend4 = new TLegend(0.3,0.8,0.89,0.89);

    legend4->SetFillColor(kWhite);
    legend4->SetLineColor(kWhite);
    legend4->SetShadowColor(kWhite);
    legend4->SetTextFont(42);
    legend4->SetTextSize(0.035);

    legend4->SetNColumns(4);

    legend4->AddEntry(c_csv_ratio_final[iHist],"c SF","l");
    legend4->AddEntry(h_total_cUp_x2,"c Tot Err","l");
    legend4->AddEntry(h_cErr1Up,"cErr1","l");
    legend4->AddEntry(h_cErr2Up,"cErr2","l");

    legend4->Draw();

    img = dirprefix + "cSF_totalErr_" + hist_name[iHist] + "_fit_All.png";
    if( makePlots ) c1->Print(img);
    img = dirprefix + "cSF_totalErr_" + hist_name[iHist] + "_fit_All.pdf";
    if( makePlots ) c1->Print(img);



    h_total_cUp_x2->SetStats(0);
    h_total_cUp_x2->GetYaxis()->SetRangeUser(0.,2.0);
    h_total_cUp_x2->GetYaxis()->SetTitle("Scale Factor");
    h_total_cUp_x2->Draw("hist");
    h_total_cDown_x2->Draw("histsame");

    //////blahblahgr_x2->Draw("pe2same");
    h_csv_ratio[iHist]->SetLineWidth(2);
    h_csv_ratio[iHist]->SetLineColor(kBlack);
    h_csv_ratio[iHist]->Draw("pe1same");
    h_total_cUp_x2->Draw("histsame");
    h_total_cDown_x2->Draw("histsame");



    TLegend *legend5 = new TLegend(0.15,0.91,0.84,0.99);

    legend5->SetFillColor(kWhite);
    legend5->SetLineColor(kWhite);
    legend5->SetShadowColor(kWhite);
    legend5->SetTextFont(42);
    legend5->SetTextSize(0.035);

    legend5->SetNColumns(3);

    legend5->AddEntry(c_csv_ratio_final[iHist],"Charm SF","l");
    legend5->AddEntry(h_total_cUp,"Charm SF Unc","l");
    legend5->AddEntry(h_csv_ratio[iHist],"Bottom SF","pl");

    legend5->Draw();


    // h_cErr1Up->Draw("histsame");
    // h_cErr1Down->Draw("histsame");
    // h_cErr2Up->Draw("histsame");
    // h_cErr2Down->Draw("histsame");

    c_csv_ratio_final[iHist]->Draw("histsame");
    //h_csv_ratio[iHist]->Draw("pe1same");

    //h_csv_ratio_final[iHist]->Draw("histsame");

    img = dirprefix + "cSF_totalErr_compare_" + hist_name[iHist] + "_fit_All.png";
    if( makePlots ) c1->Print(img);
    img = dirprefix + "cSF_totalErr_compare_" + hist_name[iHist] + "_fit_All.pdf";
    if( makePlots ) c1->Print(img);


    // Enforce non-negative weights
    for( int iBin=0; iBin<NumFinalBins; iBin++ ){
      if( c_csv_ratio_final[iHist]->GetBinContent(iBin+1)<0 ) c_csv_ratio_final[iHist]->SetBinContent(iBin+1,0.);
      if( h_cErr1Up->GetBinContent(iBin+1)<0 ) h_cErr1Up->SetBinContent(iBin+1,0.);
      if( h_cErr1Down->GetBinContent(iBin+1)<0 ) h_cErr1Down->SetBinContent(iBin+1,0.);
      if( h_cErr2Up->GetBinContent(iBin+1)<0 ) h_cErr2Up->SetBinContent(iBin+1,0.);
      if( h_cErr2Down->GetBinContent(iBin+1)<0 ) h_cErr2Down->SetBinContent(iBin+1,0.);
    }


      // h_cErr1Up->Scale(1.067065);
      // h_cErr1Down->Scale(0.9403120);
      // h_cErr2Up->Scale(0.9454488);
      // h_cErr2Down->Scale(1.061163);

    bool includeCharmCorrectionFactor = includeCharmCorrectionFactor_;
    if( includeCharmCorrectionFactor ){
      if( isCSV ){
	if( iHist==0 ){
	  h_cErr1Up->Scale(7.449925e-01);
	  h_cErr1Down->Scale(1.491617e+00);
	  h_cErr2Up->Scale(9.793321e-01);
	  h_cErr2Down->Scale(1.021558e+00);
	}
	else if( iHist==1 ){
	  h_cErr1Up->Scale(7.963818e-01);
	  h_cErr1Down->Scale(1.324551e+00);
	  h_cErr2Up->Scale(9.860349e-01);
	  h_cErr2Down->Scale(1.014366e+00);
	}
	else if( iHist==2 ){
	  h_cErr1Up->Scale(8.408265e-01);
	  h_cErr1Down->Scale(1.199268e+00);
	  h_cErr2Up->Scale(9.584892e-01);
	  h_cErr2Down->Scale(1.045269e+00);
	}
	else if( iHist==3 ){
	  h_cErr1Up->Scale(8.940275e-01);
	  h_cErr1Down->Scale(1.123551e+00);
	  h_cErr2Up->Scale(9.680976e-01);
	  h_cErr2Down->Scale(1.034077e+00);
	}
	else if( iHist==4 ){
	  h_cErr1Up->Scale(8.913683e-01);
	  h_cErr1Down->Scale(1.136799e+00);
	  h_cErr2Up->Scale(9.596289e-01);
	  h_cErr2Down->Scale(1.043917e+00);
	}
	else if( iHist==5 ){
	  h_cErr1Up->Scale(9999);
	  h_cErr1Down->Scale(9999);
	  h_cErr2Up->Scale(9999);
	  h_cErr2Down->Scale(9999);
	}
      }
      else{/// !isCSV
	if( hist_name[iHist]=="csv_ratio_Pt0_Eta0" ){ 
	  h_cErr1Up->Scale( 0.727350 ); 
	  h_cErr1Down->Scale( 2.303592 ); 
	  h_cErr2Up->Scale( 0.990359 ); 
	  h_cErr2Down->Scale( 1.009831 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt1_Eta0" ){ 
	  h_cErr1Up->Scale( 0.767877 ); 
	  h_cErr1Down->Scale( 1.843299 ); 
	  h_cErr2Up->Scale( 0.995368 ); 
	  h_cErr2Down->Scale( 1.004675 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt2_Eta0" ){ 
	  h_cErr1Up->Scale( 0.817244 ); 
	  h_cErr1Down->Scale( 1.380438 ); 
	  h_cErr2Up->Scale( 0.997843 ); 
	  h_cErr2Down->Scale( 1.002167 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt3_Eta0" ){ 
	  h_cErr1Up->Scale( 0.868956 ); 
	  h_cErr1Down->Scale( 1.200022 ); 
	  h_cErr2Up->Scale( 0.998496 ); 
	  h_cErr2Down->Scale( 1.001508 ); 
	} 
	if( hist_name[iHist]=="csv_ratio_Pt4_Eta0" ){ 
	  h_cErr1Up->Scale( 0.830826 ); 
	  h_cErr1Down->Scale( 1.251338 ); 
	  h_cErr2Up->Scale( 0.997191 ); 
	  h_cErr2Down->Scale( 1.002825 ); 
	} 
      }
    }
    
    c_csv_ratio_final[iHist]->Write(Form("c_%s_final",hist_name[iHist].Data()));
    h_cErr1Up->Write(Form("c_%s_final_cErr1Up",hist_name[iHist].Data()));
    h_cErr1Down->Write(Form("c_%s_final_cErr1Down",hist_name[iHist].Data()));
    h_cErr2Up->Write(Form("c_%s_final_cErr2Up",hist_name[iHist].Data()));
    h_cErr2Down->Write(Form("c_%s_final_cErr2Down",hist_name[iHist].Data()));



    // Piecewise fitting
    TF1* fint_cErr1Up[Nbins-1-firstBin];
    TF1* fint_cErr1Down[Nbins-1-firstBin];
    TF1* fint_cErr2Up[Nbins-1-firstBin];
    TF1* fint_cErr2Down[Nbins-1-firstBin];
    for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
      double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
      double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

      int useBin1 = h_cErr1Up->FindBin(x1);
      int useBin2 = h_cErr1Up->FindBin(x2);

      // cErr1Up
      double y1_cErr1Up = h_cErr1Up->GetBinContent(useBin1);
      double y2_cErr1Up = h_cErr1Up->GetBinContent(useBin2);

      double slope_cErr1Up = (y2_cErr1Up - y1_cErr1Up) / (x2 - x1);
      double intercept_cErr1Up = y1_cErr1Up - x1 * slope_cErr1Up;

      fint_cErr1Up[iBin] = new TF1(Form("fint_cErr1Up_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_cErr1Up[iBin]->SetParameter(0,intercept_cErr1Up);
      fint_cErr1Up[iBin]->SetParameter(1,slope_cErr1Up);

      fint_cErr1Up[iBin]->SetLineColor(kRed);

      // cErr1Down
      double y1_cErr1Down = h_cErr1Down->GetBinContent(useBin1);
      double y2_cErr1Down = h_cErr1Down->GetBinContent(useBin2);

      double slope_cErr1Down = (y2_cErr1Down - y1_cErr1Down) / (x2 - x1);
      double intercept_cErr1Down = y1_cErr1Down - x1 * slope_cErr1Down;

      fint_cErr1Down[iBin] = new TF1(Form("fint_cErr1Down_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_cErr1Down[iBin]->SetParameter(0,intercept_cErr1Down);
      fint_cErr1Down[iBin]->SetParameter(1,slope_cErr1Down);

      fint_cErr1Down[iBin]->SetLineColor(kRed);

      // cErr2Up
      double y1_cErr2Up = h_cErr2Up->GetBinContent(useBin1);
      double y2_cErr2Up = h_cErr2Up->GetBinContent(useBin2);

      double slope_cErr2Up = (y2_cErr2Up - y1_cErr2Up) / (x2 - x1);
      double intercept_cErr2Up = y1_cErr2Up - x1 * slope_cErr2Up;

      fint_cErr2Up[iBin] = new TF1(Form("fint_cErr2Up_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_cErr2Up[iBin]->SetParameter(0,intercept_cErr2Up);
      fint_cErr2Up[iBin]->SetParameter(1,slope_cErr2Up);

      fint_cErr2Up[iBin]->SetLineColor(kRed);

      // cErr2Down
      double y1_cErr2Down = h_cErr2Down->GetBinContent(useBin1);
      double y2_cErr2Down = h_cErr2Down->GetBinContent(useBin2);

      double slope_cErr2Down = (y2_cErr2Down - y1_cErr2Down) / (x2 - x1);
      double intercept_cErr2Down = y1_cErr2Down - x1 * slope_cErr2Down;

      fint_cErr2Down[iBin] = new TF1(Form("fint_cErr2Down_bin%d",iBin),"[0] + [1]*x",x1,x2);
      fint_cErr2Down[iBin]->SetParameter(0,intercept_cErr2Down);
      fint_cErr2Down[iBin]->SetParameter(1,slope_cErr2Down);

      fint_cErr2Down[iBin]->SetLineColor(kRed);
    }



    TF1* f0 = new TF1("f0","[0] + x*([1] + x*([2] + x*([3] + x*([4] + x*([5] + x*[6])))))", 0.02, 0.4 );
    f0->SetLineColor(kBlue);
    int nPars = f0->GetNpar();
    for( int iPar=0; iPar<nPars; iPar++ ) f0->SetParameter(iPar,1.);

    TF1 *f1 = new TF1("fit_csv",fit_csv,0.02,0.98,21);
    int nParsF1 = f1->GetNpar();
    for( int iPar=0; iPar<nParsF1; iPar++ ) f1->SetParameter(iPar,1.);

    TF1 *f2 = new TF1("fit_csv_2",fit_csv_2,0.02,0.98,28);
    int nParsF2 = f2->GetNpar();
    for( int iPar=0; iPar<nParsF2; iPar++ ) f2->SetParameter(iPar,1.);

    TF1 *f3 = new TF1("fit_csv_1",fit_csv_1,0.02,0.98,23);
    int nParsF3 = f3->GetNpar();
    for( int iPar=0; iPar<nParsF3-2; iPar++ ) f3->SetParameter(iPar,1.);
    f3->SetParameter(nParsF3-2,0.3);
    f3->SetParameter(nParsF3-1,0.7);

    TF1 *f4 = new TF1("fit_csv_3",fit_csv_3,0.02,0.98,23);
    int nParsF4 = f4->GetNpar();
    for( int iPar=0; iPar<nParsF4-2; iPar++ ) f4->SetParameter(iPar,1.);
    f4->SetParameter(nParsF4-2,0.3);
    f4->SetParameter(nParsF4-1,0.7);

    TF1 *f55 = new TF1("fit_csv_5",fit_csv_5,0.001,1.01,20);
    int nParsF55 = f55->GetNpar();
    for( int iPar=0; iPar<nParsF55; iPar++ ) f55->SetParameter(iPar,1.);

    // TF1 *f5 = new TF1("f5","[0] + [1]*x + [2]*x*x + [3]*x*x*x + [4]*x*x*x*x + [5]*x*x*x*x*x + [6]*x*x*x*x*x*x + [7]*x*x*x*x*x*x*x + [8]*x*x*x*x*x*x*x*x",0.001,1.01);
    // int nParsF5 = f5->GetNpar();
    // for( int iPar=0; iPar<nParsF5; iPar++ ) f5->SetParameter(iPar,1.);
    TF1 *f5 = new TF1("f5","pol8",0.001,1.01);
    int nParsF5 = f5->GetNpar();
    for( int iPar=0; iPar<nParsF5; iPar++ ) f5->SetParameter(iPar,double(10.*(iPar+1)));



    TF1* f0_LFUp = (TF1*)f0->Clone("f0_LFUp");
    TF1* f0_LFDown = (TF1*)f0->Clone("f0_LFDown");

    TF1* f0_JESUp = (TF1*)f0->Clone("f0_JESUp");
    TF1* f0_JESDown = (TF1*)f0->Clone("f0_JESDown");

    TF1* f0_Stats1Up = (TF1*)f0->Clone("f0_Stats1Up");
    TF1* f0_Stats1Down = (TF1*)f0->Clone("f0_Stats1Down");

    TF1* f0_Stats2Up = (TF1*)f0->Clone("f0_Stats2Up");
    TF1* f0_Stats2Down = (TF1*)f0->Clone("f0_Stats2Down");



    TH1D* h_fit_csv_ratio_final = (TH1D*)h_csv_ratio_final[iHist]->Clone( Form("h_fit_csv_ratio_final_%s",hist_name[iHist].Data()) );
    for( int iBin=0; iBin<h_fit_csv_ratio_final->GetNbinsX(); iBin++ ){
      h_fit_csv_ratio_final->SetBinError(iBin+1,0.2);
    }
    // TH1D* h_fit_csv_ratio_final_rebin = (TH1D*)h_csv_ratio_final[iHist]->Clone( Form("h_fit_csv_ratio_final_%s_rebin",hist_name[iHist].Data()) );

    // h_fit_csv_ratio_final_rebin->Rebin(10);
    // h_fit_csv_ratio_final_rebin->Scale(1/10.);
    // for( int iBin=0; iBin<h_fit_csv_ratio_final_rebin->GetNbinsX(); iBin++ ){
    //   h_fit_csv_ratio_final_rebin->SetBinError(iBin+1,0.2);
    // }

    TH1D* h_fit_csv_ratio_final_rebin = (TH1D*)h_csv_ratio[iHist]->Clone( Form("h_fit_csv_ratio_final_%s_rebin",hist_name[iHist].Data()) );

    /*
    //h_csv_ratio[iHist]->Fit(f1,"+mrN0S");
    //for(int i=0;i<10;i++) h_fit_csv_ratio_final->Fit(f3,"+mrN0S");
    //for(int i=0;i<10;i++) h_fit_csv_ratio_final->Fit(f5,"+mrN0S");
    for(int i=0;i<10;i++) h_fit_csv_ratio_final_rebin->Fit(f5,"+mrN0QS");
    //for(int i=0;i<10;i++) h_csv_ratio[iHist]->Fit(f1,"+mrN0S");

    h_csv_ratio_final_JESUp[iHist]->Fit(f0_JESUp,"+mrN0QS");
    h_csv_ratio_final_JESDown[iHist]->Fit(f0_JESDown,"+mrNQ0S");

    h_csv_ratio_final_LFUp[iHist]->Fit(f0_LFUp,"+mrNQ0S");
    h_csv_ratio_final_LFDown[iHist]->Fit(f0_LFDown,"+mrNQ0S");

    h_csv_ratio_final_Stats1Up[iHist]->Fit(f0_Stats1Up,"+mrNQ0S");
    h_csv_ratio_final_Stats1Down[iHist]->Fit(f0_Stats1Down,"+mrNQ0S");

    h_csv_ratio_final_Stats2Up[iHist]->Fit(f0_Stats2Up,"+mrN0QS");
    h_csv_ratio_final_Stats2Down[iHist]->Fit(f0_Stats2Down,"+mrNQ0S");



    h_csv_ratio[iHist]->Draw("pe1");
    h_csv_ratio_final[iHist]->Draw("histsame");
    f5->SetLineColor(kRed);
    f5->Draw("same");
    h_fit_csv_ratio_final_rebin->SetLineColor(kBlue);
    h_fit_csv_ratio_final_rebin->Draw("histsame");

    img = dirprefix + "piecewise_fit_compare_" + hist_name[iHist] + ".png";
    if( makePlots ) c1->Print(img);
    */

    if( iHist<NumHists_normal ){

      fit_result_file[iHist].open( Form("BTVScaleFactorFitResult/fitResult_HF_%sSF_13TeV_%s.txt", btagger.Data(),bin_name[iHist].Data()) );

      TString etaMin = "0.0";
      TString etaMax = "2.4";

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
	ptMax = "100";
      }
      else if( hist_name[iHist].Contains("Pt4") ){
	ptMin = "100";
	ptMax = "10000";
      }

      // central
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, central, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio[iHist]->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, central, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, central, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i->Eval( firstPoint ) << "\"" << "\n";
      }
	
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, central, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, central, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i->Eval( lastPoint ) << "\"" << "\n";

      // JESUp
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_JESUp[iHist]->GetBinContent(1) << "\"" << "\n";
	
	fit_result_file[iHist] << "3, iterativefit, up_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_JESUp->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_JESUp->Eval( firstPoint ) << "\"" << "\n";
      }
      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, up_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_JESUp[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, up_jes, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_JESUp->Eval( lastPoint ) << "\"" << "\n";

      // JESDown
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_JESDown[iHist]->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, down_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_JESDown->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_JESDown->Eval( firstPoint ) << "\"" << "\n";
      }
      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, down_jes, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_JESDown[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, down_jes, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_JESDown->Eval( lastPoint ) << "\"" << "\n";


      // LFUp
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_LFUp[iHist]->GetBinContent(1) << "\"" << "\n";
	
	fit_result_file[iHist] << "3, iterativefit, up_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_LFUp->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_LFUp->Eval( firstPoint ) << "\"" << "\n";
      }

      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, up_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_LFUp[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, up_lf, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_LFUp->Eval( lastPoint ) << "\"" << "\n";

      // LFDown
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_LFDown[iHist]->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, down_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_LFDown->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_LFDown->Eval( firstPoint ) << "\"" << "\n";
      }
      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, down_lf, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_LFDown[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, down_lf, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_LFDown->Eval( lastPoint ) << "\"" << "\n";


      // Stats1Up
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_Stats1Up[iHist]->GetBinContent(1) << "\"" << "\n";
	
	fit_result_file[iHist] << "3, iterativefit, up_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats1Up->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats1Up->Eval( firstPoint ) << "\"" << "\n";
      }

      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, up_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_Stats1Up[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, up_hfstats1, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_Stats1Up->Eval( lastPoint ) << "\"" << "\n";

      // Stats1Down
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_Stats1Down[iHist]->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, down_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats1Down->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats1Down->Eval( firstPoint ) << "\"" << "\n";
      }

      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, down_hfstats1, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_Stats1Down[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, down_hfstats1, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_Stats1Down->Eval( lastPoint ) << "\"" << "\n";




      // Stats2Up
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_Stats2Up[iHist]->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, up_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats2Up->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats2Up->Eval( firstPoint ) << "\"" << "\n";
      }
	
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, up_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_Stats2Up[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, up_hfstats2, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_Stats2Up->Eval( lastPoint ) << "\"" << "\n";

      // Stats2Down
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_csv_ratio_Stats2Down[iHist]->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, down_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats2Down->Eval( firstPoint ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << m_i_Stats2Down->Eval( firstPoint ) << "\"" << "\n";
      }
	
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, down_hfstats2, 0, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_Stats2Down[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, down_hfstats2, 0, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << m_i_Stats2Down->Eval( lastPoint ) << "\"" << "\n";


      // Charm uncertainties
      // CErr1Up
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_cErr1Up->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, up_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr1Up->GetBinContent( h_cErr1Up->FindBin(firstPoint) ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr1Up->GetBinContent( h_cErr1Up->FindBin(firstPoint) ) << "\"" << "\n";
      }
      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, up_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_cErr1Up[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, up_cferr1, 1, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << h_cErr1Up->GetBinContent( h_cErr1Up->FindBin(lastPoint) ) << "\"" << "\n";


      // CErr1Down
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_cErr1Down->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, down_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr1Down->GetBinContent( h_cErr1Down->FindBin(firstPoint) ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr1Down->GetBinContent( h_cErr1Down->FindBin(firstPoint) ) << "\"" << "\n";
      }
      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, down_cferr1, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_cErr1Down[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, down_cferr1, 1, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << h_cErr1Down->GetBinContent( h_cErr1Down->FindBin(lastPoint) ) << "\"" << "\n";


      // CErr2Up
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_cErr2Up->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, up_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr2Up->GetBinContent( h_cErr2Up->FindBin(firstPoint) ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, up_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr2Up->GetBinContent( h_cErr2Up->FindBin(firstPoint) ) << "\"" << "\n";
      }
	
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, up_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_cErr2Up[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, up_cferr2, 1, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << h_cErr2Up->GetBinContent( h_cErr2Up->FindBin(lastPoint) ) << "\"" << "\n";


      // CErr2Down
      if( isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-15" << ", " << "0" << ", " 
			       << "\"" << h_cErr2Down->GetBinContent(1) << "\"" << "\n";

	fit_result_file[iHist] << "3, iterativefit, down_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "0" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr2Down->GetBinContent( h_cErr2Down->FindBin(firstPoint) ) << "\"" << "\n";
      }
      else if( !isCSV ){
	fit_result_file[iHist] << "3, iterativefit, down_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << "-1.1" << ", " << firstPoint << ", " 
			       << "\"" << h_cErr2Down->GetBinContent( h_cErr2Down->FindBin(firstPoint) ) << "\"" << "\n";
      }
      
      for( int iBin=firstBin; iBin<Nbins-1; iBin++ ){
	double x1 = h_csv_ratio[iHist]->GetBinCenter(iBin+1);
	double x2 = h_csv_ratio[iHist]->GetBinCenter(iBin+1+1);

	fit_result_file[iHist] << "3, iterativefit, down_cferr2, 1, " 
			       << etaMin << ", " << etaMax << ", " 
			       << ptMin << ", " << ptMax << ", " 
			       << x1 << ", " << x2 << ", " 
			       << "\"" << fint_cErr2Down[iBin]->GetExpFormula("p") << "\"" << "\n";
      }

      fit_result_file[iHist] << "3, iterativefit, down_cferr2, 1, " 
			     << etaMin << ", " << etaMax << ", " 
			     << ptMin << ", " << ptMax << ", " 
			     << lastPoint << ", " << "1.1" << ", " 
			     << "\"" << h_cErr2Down->GetBinContent( h_cErr2Down->FindBin(lastPoint) ) << "\"" << "\n";


    } // end if( iHist<NumHists_normal )


    /*
    fit_result_file << "*********************************************************** " << "\n";
    fit_result_file << "Bin: " << hist_name[iHist].Data() << "\n";
    fit_result_file << "\t central = " << f0->GetExpFormula("p") << "\n";
    fit_result_file << "\t LFUp = " << f0_LFUp->GetExpFormula("p") << "\n";
    fit_result_file << "\t LFDown = " << f0_LFDown->GetExpFormula("p") << "\n";

    fit_result_file << "\t JESUp = " << f0_JESUp->GetExpFormula("p") << "\n";
    fit_result_file << "\t JESDown = " << f0_JESDown->GetExpFormula("p") << "\n";

    fit_result_file << "\t Stats1Up = " << f0_Stats1Up->GetExpFormula("p") << "\n";
    fit_result_file << "\t Stats1Down = " << f0_Stats1Down->GetExpFormula("p") << "\n";

    fit_result_file << "\t Stats2Up = " << f0_Stats2Up->GetExpFormula("p") << "\n";
    fit_result_file << "\t Stats2Down = " << f0_Stats2Down->GetExpFormula("p") << "\n";
    fit_result_file << "*********************************************************** " << "\n";
    */

    delete m_i;
    delete m_i_JESUp;
    delete m_i_JESDown;
    delete m_i_LFUp;
    delete m_i_LFDown;
    delete m_i_Stats1Up;
    delete m_i_Stats1Down;
    delete m_i_Stats2Up;
    delete m_i_Stats2Down;

    delete legend;

    delete line;
  }


  //// CHARM
  // central
  ofstream fit_result_file_charm;
  fit_result_file_charm.open( Form("BTVScaleFactorFitResult/fitResult_CF_%sSF_13TeV.txt",btagger.Data()) );

  fit_result_file_charm << "3, iterativefit, central, 1, " 
			<< "0.0" << ", " << "2.4" << ", " 
			<< "20.0" << ", " << "10000" << ", " 
			<< "-15" << ", " << "1.1" << ", " 
			<< "\"" << "1.0" << "\"" << "\n";
  
  fit_result_file_charm.close();



  // // Uncertainties
  // ofstream fit_result_file_unc;
  // fit_result_file_unc.open( "BTVScaleFactorFitResult/fitResult_uncertainties_csvSF_13TeV.txt" );

  // fit_result_file_unc << "3, iterativefit, up_jes, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_jes, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_hf, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_hf, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_hf, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_hf, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_lf, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_lf, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_lf, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_lf, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_cferr1, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_cferr1, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_cferr2, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_cferr2, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_cferr1, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_cferr1, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_cferr2, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_cferr2, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";



  // fit_result_file_unc << "3, iterativefit, up_hfstats1, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_hfstats1, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_hfstats2, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_hfstats2, 2, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";



  // fit_result_file_unc << "3, iterativefit, up_hfstats1, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_hfstats1, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_hfstats2, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_hfstats2, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";



  // fit_result_file_unc << "3, iterativefit, up_lfstats1, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_lfstats1, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_lfstats2, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_lfstats2, 0, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";


  // fit_result_file_unc << "3, iterativefit, up_lfstats1, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_lfstats1, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, up_lfstats2, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";

  // fit_result_file_unc << "3, iterativefit, down_lfstats2, 1, " 
  // 		      << "0.0" << ", " << "2.4" << ", " 
  // 		      << "20.0" << ", " << "10000" << ", " 
  // 		      << "-15" << ", " << "1.1" << ", " 
  // 		      << "\"" << "1.0" << "\"" << "\n";


  // fit_result_file_unc.close();



  for( int iHist=0; iHist<NumHists_normal; iHist++ ){
    fit_result_file[iHist].close();
  }


  std::cout << " Done! " << std::endl;

  histofile.Write();
  histofile.Close();


}

/*


h_c_jet_csv_Pt0_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt0_Eta0_SysC1->Integral()
h_c_jet_csv_Pt0_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt0_Eta0_SysC2->Integral()
h_c_jet_csv_Pt0_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt0_Eta0_SysC3->Integral()
h_c_jet_csv_Pt0_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt0_Eta0_SysC4->Integral()

h_c_jet_csv_Pt1_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt1_Eta0_SysC1->Integral()
h_c_jet_csv_Pt1_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt1_Eta0_SysC2->Integral()
h_c_jet_csv_Pt1_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt1_Eta0_SysC3->Integral()
h_c_jet_csv_Pt1_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt1_Eta0_SysC4->Integral()

h_c_jet_csv_Pt2_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt2_Eta0_SysC1->Integral()
h_c_jet_csv_Pt2_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt2_Eta0_SysC2->Integral()
h_c_jet_csv_Pt2_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt2_Eta0_SysC3->Integral()
h_c_jet_csv_Pt2_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt2_Eta0_SysC4->Integral()

h_c_jet_csv_Pt3_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt3_Eta0_SysC1->Integral()
h_c_jet_csv_Pt3_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt3_Eta0_SysC2->Integral()
h_c_jet_csv_Pt3_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt3_Eta0_SysC3->Integral()
h_c_jet_csv_Pt3_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt3_Eta0_SysC4->Integral()

h_c_jet_csv_Pt4_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt4_Eta0_SysC1->Integral()
h_c_jet_csv_Pt4_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt4_Eta0_SysC2->Integral()
h_c_jet_csv_Pt4_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt4_Eta0_SysC3->Integral()
h_c_jet_csv_Pt4_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt4_Eta0_SysC4->Integral()

h_c_jet_csv_Pt5_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt5_Eta0_SysC1->Integral()
h_c_jet_csv_Pt5_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt5_Eta0_SysC2->Integral()
h_c_jet_csv_Pt5_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt5_Eta0_SysC3->Integral()
h_c_jet_csv_Pt5_Eta0->Integral()/h_c_jet_csv_CharmCSVSF_Pt5_Eta0_SysC4->Integral()








h_c_jet_cmva_Pt0_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt0_Eta0_SysC1->Integral()
h_c_jet_cmva_Pt0_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt0_Eta0_SysC2->Integral()
h_c_jet_cmva_Pt0_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt0_Eta0_SysC3->Integral()
h_c_jet_cmva_Pt0_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt0_Eta0_SysC4->Integral()

h_c_jet_cmva_Pt1_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt1_Eta0_SysC1->Integral()
h_c_jet_cmva_Pt1_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt1_Eta0_SysC2->Integral()
h_c_jet_cmva_Pt1_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt1_Eta0_SysC3->Integral()
h_c_jet_cmva_Pt1_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt1_Eta0_SysC4->Integral()

h_c_jet_cmva_Pt2_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt2_Eta0_SysC1->Integral()
h_c_jet_cmva_Pt2_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt2_Eta0_SysC2->Integral()
h_c_jet_cmva_Pt2_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt2_Eta0_SysC3->Integral()
h_c_jet_cmva_Pt2_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt2_Eta0_SysC4->Integral()

h_c_jet_cmva_Pt3_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt3_Eta0_SysC1->Integral()
h_c_jet_cmva_Pt3_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt3_Eta0_SysC2->Integral()
h_c_jet_cmva_Pt3_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt3_Eta0_SysC3->Integral()
h_c_jet_cmva_Pt3_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt3_Eta0_SysC4->Integral()

h_c_jet_cmva_Pt4_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt4_Eta0_SysC1->Integral()
h_c_jet_cmva_Pt4_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt4_Eta0_SysC2->Integral()
h_c_jet_cmva_Pt4_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt4_Eta0_SysC3->Integral()
h_c_jet_cmva_Pt4_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt4_Eta0_SysC4->Integral()

h_c_jet_cmva_Pt5_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt5_Eta0_SysC1->Integral()
h_c_jet_cmva_Pt5_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt5_Eta0_SysC2->Integral()
h_c_jet_cmva_Pt5_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt5_Eta0_SysC3->Integral()
h_c_jet_cmva_Pt5_Eta0->Integral()/h_c_jet_cmva_CharmCMVASF_Pt5_Eta0_SysC4->Integral()





TFile* f_old = new TFile("csv_rwt_fit_hf_v3_final_2015_11_23.root");
TFile* f_new = new TFile("data/csv_rwt_fit_hf_v2.root");


TH1D* h_old0 = (TH1D*)f_old->Get("csv_ratio_Pt0_Eta0_final")->Clone("h_old0");
TH1D* h_new0 = (TH1D*)f_new->Get("csv_ratio_Pt0_Eta0_final")->Clone("h_new0");

TH1D* h_ratio0 = (TH1D*)h_new0->Clone("h_ratio0");

h_ratio0->Divide(h_old0);

h_ratio0->Draw();



TH1D* h_old1 = (TH1D*)f_old->Get("csv_ratio_Pt1_Eta0_final")->Clone("h_old1");
TH1D* h_new1 = (TH1D*)f_new->Get("csv_ratio_Pt1_Eta0_final")->Clone("h_new1");

TH1D* h_ratio1 = (TH1D*)h_new1->Clone("h_ratio1");

h_ratio1->Divide(h_old1);

h_ratio1->Draw();



TH1D* h_old2 = (TH1D*)f_old->Get("csv_ratio_Pt2_Eta0_final")->Clone("h_old2");
TH1D* h_new2 = (TH1D*)f_new->Get("csv_ratio_Pt2_Eta0_final")->Clone("h_new2");

TH1D* h_ratio2 = (TH1D*)h_new2->Clone("h_ratio2");

h_ratio2->Divide(h_old2);

h_ratio2->Draw();



TH1D* h_old3 = (TH1D*)f_old->Get("csv_ratio_Pt3_Eta0_final")->Clone("h_old3");
TH1D* h_new3 = (TH1D*)f_new->Get("csv_ratio_Pt3_Eta0_final")->Clone("h_new3");

TH1D* h_ratio3 = (TH1D*)h_new3->Clone("h_ratio3");

h_ratio3->Divide(h_old3);

h_ratio3->Draw();



TH1D* h_old4 = (TH1D*)f_old->Get("csv_ratio_Pt4_Eta0_final")->Clone("h_old4");
TH1D* h_new4 = (TH1D*)f_new->Get("csv_ratio_Pt4_Eta0_final")->Clone("h_new4");

TH1D* h_ratio4 = (TH1D*)h_new4->Clone("h_ratio4");

h_ratio4->Divide(h_old4);

h_ratio4->Draw();



 */
