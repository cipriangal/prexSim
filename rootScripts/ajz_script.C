#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1F.h"
#include "THStack.h"
#include "TObjArray.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

void sumRootFiles(string config,string ident, int nfiles, int n_events_k){
  string fnms[nfiles];
  
  for(int i = 0; i < nfiles; i++){
  	char fbuf[100];
  	if(i < 10){sprintf(fbuf, "../output/%s_%dkEv_000%d_%s/o_prexSim.root", config.c_str(), n_events_k, i, ident.c_str()); }
  	else{sprintf(fbuf, "../output/%s_%dkEv_00%d_%s/o_prexSim.root", config.c_str(), n_events_k, i, ident.c_str());}
  	fnms[i] = (string)fbuf;
  }
  
  TChain ch("geant");
  
  for(int i = 0; i < nfiles; i++){
  	printf(fnms[i].c_str()); printf("\n");
  	ch.Add(fnms[i].c_str());
  }
  
  ch.Merge((config + "_" + ident + ".root").c_str());
  
}

void hitMap(const char* fname, int vol){
	char vol_buf[100];
	TFile *f = new TFile(fname);
	TTree *geant = (TTree*)f->Get("geant");
	
	sprintf(vol_buf,"volume==%d && x*x + y*y <= 2500", vol);
	
	cout<<vol_buf<<endl;
	
	geant->Draw("y:x",vol_buf,"colz");
	
}

void dumpShieldHitMap(string fname, int xy_limit, int z0_limit){
	TFile *f = new TFile(fname.c_str());
	TTree *geant = (TTree*)f->Get("geant");
	gStyle->SetOptStat("e");
	
	TCanvas *c1 = new TCanvas("c1","heat map",2000,1000);
	c1->Divide(2,1);

	c1->cd(1);
	char vol_buf1[100];
	sprintf(vol_buf1,"volume==%d && x>=%d && x<=%d && y>=%d && y<=%d && z0>=%d", 9002, -xy_limit, xy_limit, -xy_limit, xy_limit, z0_limit);
	cout<<vol_buf1<<endl;
	geant->Draw("y:x",vol_buf1,"colz");
	
	c1->cd(2);
	char vol_buf2[100];
	sprintf(vol_buf2,"volume==%d && x>=%d && x<=%d && y>=%d && y<=%d && z0>=%d", 9001, -xy_limit, xy_limit, -xy_limit, xy_limit, z0_limit);
	cout<<vol_buf2<<endl;	
	geant->Draw("y:x",vol_buf2,"colz");
}

void energy_histo(const char* fname, int vol){
	char param_buff[200];
	TFile *f = new TFile(fname);
	TTree *geant = (TTree*)f->Get("geant");
	
	gStyle->SetOptStat("eMRiou");
	
	sprintf(param_buff,"volume==%d && x*x + y*y <= 2500 && pz>0",vol);

	std::cout << param_buff << endl;
	
	geant->Draw("kineE",param_buff);
}

void kineEHistoComparison(string fname, Float_t xy_limit, Float_t z0_limit){
	TH1F* kin1 = new TH1F("kin1","DSDet Hits",100,0.0,10.0);
	kin1->SetLineColor(kBlue);
	TH1F* kin2 = new TH1F("kin2","USDet Hits",100,0.0,10.0);
	kin2->SetLineColor(kRed);
	THStack *hs = new THStack("hs","");
	TCanvas *c1 = new TCanvas("c1","KineE Histo Comparison",1000,800);
	gStyle->SetOptStat("eMRiou");

	Float_t type, volume, event;
  Float_t kineE;
  Float_t z_0,xd,yd;
  
  TChain *t = new TChain("geant");
  t->Add(fname.c_str());
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("volume",&volume);
  t->SetBranchAddress("x",&xd);  
  t->SetBranchAddress("y",&yd);  
  t->SetBranchAddress("z0",&z_0);
  t->SetBranchAddress("event",&event);
  t->SetBranchAddress("kineE",&kineE);
  
  Int_t nentries = (Int_t)t->GetEntries();
  for (int i=0; i<nentries ; i++) {
    t->GetEntry(i);
    if(volume==9002 && xd<=xy_limit && xd>=-xy_limit && yd<=xy_limit && yd>=-xy_limit && z_0>=z0_limit){
    	kin1->Fill(kineE);
    }
    else if(volume==9001 && xd<=xy_limit && xd>=-xy_limit && yd<=xy_limit && yd>=-xy_limit && z_0>=z0_limit){
    	kin2->Fill(kineE);
    }
  }
  hs->Add(kin1);
  hs->Add(kin2);
  hs->Draw("nostack");
}


void z0_histo(string fname, int vol){
	TH1F* z01 = new TH1F("z01","DSDet Hits",100,0,35000);
	z01->SetLineColor(kBlue);
	TH1F* z02 = new TH1F("z02","USDet Hits",100,0,35000);
	z02->SetLineColor(kRed);
	THStack *hs = new THStack("hs","");
	TCanvas *c1 = new TCanvas("c1","z0 Hits in 9001 & 9002",1000,800);
	gStyle->SetOptStat("eMRiou");
	
	Float_t type, volume, event, kinE;
  Float_t z_0;
  
  TChain *t = new TChain("geant");
  t->Add(fname.c_str());
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("volume",&volume);
  t->SetBranchAddress("z0",&z_0);
  t->SetBranchAddress("event",&event);
  t->SetBranchAddress("kineE",&kinE);
  
  Int_t nentries = (Int_t)t->GetEntries();
  for (int i=0; i<nentries ; i++) {
    t->GetEntry(i);
    if(volume==9002 && z_0>=0 && kinE>=1){
    	z01->Fill(z_0);
    }
    else if(volume==9001 && z_0>=0 && kinE>=1){
    	z02->Fill(z_0);
    }
  }
  hs->Add(z01);
  hs->Add(z02);
  hs->Draw("nostack");
}

void oneD_histo(string fname, string meas, string cuts){
	TFile *f = new TFile(fname.c_str());
	TTree *geant = (TTree*)f->Get("geant");
	gStyle->SetOptStat("eMRiou");
	
	geant->Draw(meas.c_str(), cuts.c_str());
}

void compare_pos0(string fname1, string fname2, string comp_type, Int_t vol, Int_t lower_limit, Int_t upper_limit){
	TH1F* histo1 = new TH1F("z01","Iron",100,lower_limit,upper_limit);
	histo1->SetLineColor(kBlue);
	TH1F* histo2 = new TH1F("z02","No Shield",100,lower_limit,upper_limit);
	histo2->SetLineColor(kRed);
	char title[100]; sprintf(title,"hits in volume %d",vol);
	THStack *hs = new THStack("hs",title);
	TCanvas *c1 = new TCanvas("c1",title,1000,800);
	gStyle->SetOptStat("eMRiou");
	
	Float_t type1, volume1, event1, edep1, type2, volume2, event2, edep2;
  Float_t x_01, x_02, z_01, z_02;
  
  cout<<"Analyzing file "<<fname1.c_str()<<endl;
  TChain *t1 = new TChain("geant");
  t1->Add(fname1.c_str());
  t1->SetBranchAddress("type",&type1);
  t1->SetBranchAddress("volume",&volume1);
  t1->SetBranchAddress("x0",&x_01);
  t1->SetBranchAddress("z0",&z_01);
  t1->SetBranchAddress("event",&event1);
  t1->SetBranchAddress("Edeposit",&edep1);
  
  Int_t nentries1 = (Int_t)t1->GetEntries();
  for (int i=0; i<nentries1 ; i++) {
    t1->GetEntry(i);
    if(volume1==vol && z_01>=0 && edep1>=1){
    	if(comp_type == "x0") 
    		histo1->Fill(x_01);
    	else if(comp_type == "z0") 
    		histo1->Fill(z_01);
    	else
    		histo1->Fill(edep1);
    }
  }
  
  cout<<"Analyzing file "<<fname2.c_str()<<endl;
  TChain *t2 = new TChain("geant");
  t2->Add(fname2.c_str());
  t2->SetBranchAddress("type",&type2);
  t2->SetBranchAddress("volume",&volume2);
  t2->SetBranchAddress("x0",&x_02);
  t2->SetBranchAddress("z0",&z_02);
  t2->SetBranchAddress("event",&event2);
  t2->SetBranchAddress("Edeposit",&edep2);
  
  Int_t nentries2 = (Int_t)t2->GetEntries();
  for (int i=0; i<nentries2 ; i++) {
    t2->GetEntry(i);
    if(volume2==vol && z_02>=0 && edep2>=1){
    	if(comp_type == "x0")
    		histo2->Fill(x_02);
    	else if(comp_type == "z0")
    		histo2->Fill(z_02);
    	else
    		histo2->Fill(edep2);
    }
  }
  
  hs->Add(histo1);
  hs->Add(histo2);
  hs->Draw("nostack");
  
  hs->GetXaxis()->SetTitle(comp_type.c_str());
	
	c1->Modified();
  gPad->BuildLegend(0.75,0.75,0.95,0.95,"");
}
