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

//######################
//# FOR USE ON RIVANNA #
//######################
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
    cout<<fnms[i].c_str()<<endl;
    ch.Add(fnms[i].c_str());
  }
  
  ch.Merge((config + "_" + ident + ".root").c_str());
  
}

//####################
//# FOR USE ON IFARM #
//####################
void one_d_histo(string sim, string conf, int n_events_k, string meas, string cuts, Int_t nbinsx, Double_t xlo, Double_t xhi){
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root";
  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
  TH1F *h1 = new TH1F("h1", (conf + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);
  t->Project("h1", meas.c_str(), cuts.c_str());

  gStyle->SetOptStat("eMRiou");
  h1->GetXaxis()->SetTitle(meas.c_str());

  h1->Draw();
}

void two_d_histo(string sim, string conf, int n_events_k, string meas, string cuts, Int_t nbinsx, Double_t xlow, Double_t xhi, Int_t nbinsy, Double_t ylow, Double_t yhi, Double_t zmax){
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root";

  TCanvas *c1=new TCanvas("c1","c1",800,600);
  
  TFile *f = new TFile(fname.c_str());
  TTree* geant = (TTree*)f->Get("t");
  TH2F *h = new TH2F("h1", (conf + ": " + meas + ", " + cuts).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);

  geant->Project("h1", meas.c_str(), cuts.c_str());
  if(zmax > 0){h->GetZaxis()->SetRangeUser(0, zmax);}
  h->Draw("colz");
}

void side_by_side_comp_h1(string sim, string conf1, string conf2, int n_events_k1, int n_events_k2, string meas, string cuts, Int_t nbinsx, Double_t xlo, Double_t xhi){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + ".root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + ".root";

  cout<<"File 1: "<<f1name<<"; File 2: "<<f2name<<endl;

  gStyle->SetOptStat("eMRuoi");
  TCanvas *c1=new TCanvas("c1","c1",1600,600);
  c1->Divide(2);
  TFile *f1 = new TFile(f1name.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TFile *f2 = new TFile(f2name.c_str());
  TTree *t2 = (TTree*)f2->Get("t");
  TH1F *h1 = new TH1F("h1", (conf1 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);
  TH1F *h2 = new TH1F("h2", (conf2 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);

  t1->Project("h1", meas.c_str(), cuts.c_str());
  t2->Project("h2", meas.c_str(), cuts.c_str());

  h1->GetXaxis()->SetTitle(meas.c_str());
  h2->GetXaxis()->SetTitle(meas.c_str());
  c1->cd(1); h1->Draw();
  c1->cd(2); h2->Draw();
}

void side_by_side_comp_h2(string sim, string conf1, string conf2, int n_events_k1, int n_events_k2, string meas, string cuts, Int_t nbinsx, Double_t xlow, Double_t xhi, Int_t nbinsy, Double_t ylow, Double_t yhi){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + ".root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + ".root";

  cout<<"File 1: "<<f1name<<"; File 2: "<<f2name<<endl;

  gStyle->SetOptStat("e");
  TCanvas *c1=new TCanvas("c1","c1",2400,1200);
  c1->Divide(2);
  TFile *f1 = new TFile(f1name.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TFile *f2 = new TFile(f2name.c_str());
  TTree *t2 = (TTree*)f2->Get("t");
  TH2F *h1 = new TH2F("h1", (conf1 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  TH2F *h2 = new TH2F("h2", (conf2 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  //h1->SetStats(kFALSE);
  //h2->SetStats(kFALSE);

  t1->Project("h1", meas.c_str(), cuts.c_str());
  t2->Project("h2", meas.c_str(), cuts.c_str());

  c1->cd(1); h1->Draw("colz");
  c1->cd(2); h2->Draw("colz");
}

void stack_comp_h1(string sim, string conf1, string conf2, int n_events_k1, int n_events_k2, string meas, string cuts, Int_t nbinsx, Double_t xlo, Double_t xhi){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + ".root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + ".root";

  TCanvas *c1=new TCanvas("c1","c1",1200,1200);
  TFile *f1 = new TFile(f1name.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TFile *f2 = new TFile(f2name.c_str());
  TTree *t2 = (TTree*)f2->Get("t");
  TH1F *h1 = new TH1F("h1", (conf1 + " vs " + conf2 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);
  TH1F *h2 = new TH1F("h2", (conf2 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);

  t1->Project("h1", meas.c_str(), cuts.c_str());
  t2->Project("h2", meas.c_str(), cuts.c_str());

  h1->SetLineColor(kBlue); h2->SetLineColor(kRed);
  h1->GetXaxis()->SetTitle(meas.c_str());
  h1->Draw(); h2->Draw("same");
}

void stack_comp_h1_3(string sim, string conf1, string conf2, string conf3, int n_events_k1, int n_events_k2, int n_events_k3, string meas, string cuts, Int_t nbinsx, Double_t xlo, Double_t xhi){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + ".root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + ".root";
  string f3name = "~/farmOut/" + sim + "_" + conf3 + "_" + to_string(n_events_k3) + "kEv/" + sim + "_" + conf3 + ".root";

  TCanvas *c1=new TCanvas("c1","c1",1200,1200);
  TFile *f1 = new TFile(f1name.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TFile *f2 = new TFile(f2name.c_str());
  TTree *t2 = (TTree*)f2->Get("t");
  TFile *f3 = new TFile(f3name.c_str());
  TTree *t3 = (TTree*)f3->Get("t");
  TH1F *h1 = new TH1F("h1", (conf1 + " vs " + conf2 + " vs " + conf3 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);
  TH1F *h2 = new TH1F("h2", (conf2 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);
  TH1F *h3 = new TH1F("h3", (conf3 + ": " + meas + ", " + cuts).c_str(), nbinsx, xlo, xhi);

  t1->Project("h1", meas.c_str(), cuts.c_str());
  t2->Project("h2", meas.c_str(), cuts.c_str());
  t3->Project("h3", meas.c_str(), cuts.c_str());

  h1->SetLineColor(kBlue); h2->SetLineColor(kRed); h3->SetLineColor(kGreen);
  h1->GetXaxis()->SetTitle(meas.c_str());
  h1->Draw(); h2->Draw("same"); h3->Draw("same");
}

void stack_comp_h1_weighted(string sim, string conf1, string conf2, int n_events_k1, int n_events_k2, string meas, string cuts, string weight, Int_t nbinsx, Double_t xlo, Double_t xhi){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + ".root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + ".root";

  TCanvas *c1=new TCanvas("c1","c1",1200,1200);
  TFile *f1 = new TFile(f1name.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TFile *f2 = new TFile(f2name.c_str());
  TTree *t2 = (TTree*)f2->Get("t");
  TH1F *h1 = new TH1F("h1", (conf1 + " vs " + conf2 + ": " + meas + ", " + weight + "*(" + cuts + ")").c_str(), nbinsx, xlo, xhi);
  TH1F *h2 = new TH1F("h2", (conf2 + ": " + meas + ", " + weight + "*(" + cuts + ")").c_str(), nbinsx, xlo, xhi);

  t1->Project("h1", meas.c_str(), (weight + "*(" + cuts + ")").c_str());
  t2->Project("h2", meas.c_str(), (weight + "*(" + cuts + ")").c_str());

  h1->SetLineColor(kBlue); h2->SetLineColor(kRed);
  h1->GetXaxis()->SetTitle(meas.c_str());
  h1->Draw("h"); h2->Draw("same && h");
}

void stack_comp_h1_3_weighted(string sim, string conf1, string conf2, string conf3, int n_events_k1, int n_events_k2, int n_events_k3, string meas, string cuts, string weight, Int_t nbinsx, Double_t xlo, Double_t xhi){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + ".root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + ".root";
  string f3name = "~/farmOut/" + sim + "_" + conf3 + "_" + to_string(n_events_k3) + "kEv/" + sim + "_" + conf3 + ".root";

  TCanvas *c1=new TCanvas("c1","c1",1200,1200);
  TFile *f1 = new TFile(f1name.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TFile *f2 = new TFile(f2name.c_str());
  TTree *t2 = (TTree*)f2->Get("t");
  TFile *f3 = new TFile(f3name.c_str());
  TTree *t3 = (TTree*)f3->Get("t");
  TH1F *h1 = new TH1F("h1", (conf1 + " vs " + conf2 +  + " vs " + conf3 + ": " + meas + ", " + weight + "*(" + cuts + ")").c_str(), nbinsx, xlo, xhi);
  TH1F *h2 = new TH1F("h2", (conf2 + ": " + meas + ", " + weight + "*(" + cuts + ")").c_str(), nbinsx, xlo, xhi);
  TH1F *h3 = new TH1F("h3", (conf3 + ": " + meas + ", " + weight + "*(" + cuts + ")").c_str(), nbinsx, xlo, xhi);

  t1->Project("h1", meas.c_str(), (weight + "*(" + cuts + ")").c_str());
  t2->Project("h2", meas.c_str(), (weight + "*(" + cuts + ")").c_str());
  t3->Project("h3", meas.c_str(), (weight + "*(" + cuts + ")").c_str());

  h1->SetLineColor(kBlue); h2->SetLineColor(kRed); h3->SetLineColor(kGreen);
  h1->GetXaxis()->SetTitle(meas.c_str());
  h1->Draw("h"); h2->Draw("same && h"); h3->Draw("same && h");
}


void particle_histo_quad(string sim, string conf, int n_events_k, string meas, string cuts, Int_t nbinsx, Double_t xlo, Double_t xhi){
  TFile *f = new TFile(("~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root").c_str());
  TTree *t = (TTree*)f->Get("t");

  TH1F *h1 = new TH1F("h1", (conf + ": " + cuts + ", all PDGs").c_str(),      nbinsx, xlo, xhi);
  TH1F* h2 = new TH1F("h2", (conf + ": " + cuts + ", electrons").c_str(),     nbinsx, xlo, xhi);
  TH1F* h3 = new TH1F("h3", (conf + ": " + cuts + ", gammas").c_str(),        nbinsx, xlo, xhi);
  TH1F* h4 = new TH1F("h4", (conf + ": " + cuts + ", neutrons").c_str(),      nbinsx, xlo, xhi);
  //TH1F* h5 = new TH1F("h5", (conf + ": " + cuts + ", targ neutrons").c_str(), nbinsx, xlo, xhi);
  //TH1F* h6 = new TH1F("h6", (conf + ": " + cuts + ", coll neutrons").c_str(), nbinsx, xlo, xhi);

  TCanvas *c1 = new TCanvas("c1", "c1", 2000, 1400);
  //c1->Divide(2, 3);
  c1->Divide(2, 2);
  gStyle->SetOptStat("eMRiou");
  
  t->Project("h1", meas.c_str(), cuts.c_str());
  t->Project("h2", meas.c_str(), ("(" + cuts + ") && abs(pdgID)==11").c_str());
  t->Project("h3", meas.c_str(), ("(" + cuts + ") && abs(pdgID)==22").c_str());
  t->Project("h4", meas.c_str(), ("(" + cuts + ") && abs(pdgID)==2112").c_str());
  //t->Project("h5", meas.c_str(), ("(" + cuts + ") && abs(pdgID)==2112 && z0>-1225.24 && z0<-895.04").c_str());
  //t->Project("h6", meas.c_str(), ("(" + cuts + ") && abs(pdgID)==2112 && z0>-250 && z0<-85").c_str());

  h1->GetXaxis()->SetTitle(meas.c_str());
  h2->GetXaxis()->SetTitle(meas.c_str());
  h3->GetXaxis()->SetTitle(meas.c_str());
  h4->GetXaxis()->SetTitle(meas.c_str());
  //h5->GetXaxis()->SetTitle(meas.c_str());
  //h6->GetXaxis()->SetTitle(meas.c_str());

  c1->cd(1); h1->Draw();
  c1->cd(2); h2->Draw();
  c1->cd(3); h3->Draw();
  c1->cd(4); h4->Draw();
  //c1->cd(5); h5->Draw();
  //c1->cd(6); h6->Draw();
}

void particle_histo_quad_weighted(string sim, string conf, int n_events_k, string meas, string cuts, string weight, Int_t nbinsx, Double_t xlo, Double_t xhi){
  TFile *f = new TFile(("~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root").c_str());
  TTree *t = (TTree*)f->Get("t");

  TH1F *h1 = new TH1F("h1", (conf + ": " + weight + "*(" + cuts + "), all PDGs").c_str(),      nbinsx, xlo, xhi);
  TH1F* h2 = new TH1F("h2", (conf + ": " + weight + "*(" + cuts + "), electrons").c_str(),     nbinsx, xlo, xhi);
  TH1F* h3 = new TH1F("h3", (conf + ": " + weight + "*(" + cuts + "), gammas").c_str(),        nbinsx, xlo, xhi);
  TH1F* h4 = new TH1F("h4", (conf + ": " + weight + "*(" + cuts + "), neutrons").c_str(),      nbinsx, xlo, xhi);

  TCanvas *c1 = new TCanvas("c1", "c1", 2000, 1400);
  c1->Divide(2, 2);
  gStyle->SetOptStat("eMRiou");
  
  t->Project("h1", meas.c_str(), (weight + "*("  + cuts + ")").c_str());
  t->Project("h2", meas.c_str(), (weight + "*((" + cuts + ") && abs(pdgID)==11)").c_str());
  t->Project("h3", meas.c_str(), (weight + "*((" + cuts + ") && abs(pdgID)==22)").c_str());
  t->Project("h4", meas.c_str(), (weight + "*((" + cuts + ") && abs(pdgID)==2112)").c_str());

  h1->GetXaxis()->SetTitle(meas.c_str());
  h2->GetXaxis()->SetTitle(meas.c_str());
  h3->GetXaxis()->SetTitle(meas.c_str());
  h4->GetXaxis()->SetTitle(meas.c_str());

  c1->cd(1); h1->Draw();
  c1->cd(2); h2->Draw();
  c1->cd(3); h3->Draw();
  c1->cd(4); h4->Draw();
}

void particle_histo_quad_h2(string sim, string conf, int n_events_k, string meas, string cuts, Int_t nbinsx, Double_t xlow, Double_t xhi, Int_t nbinsy, Double_t ylow, Double_t yhi){
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root";

  TCanvas *c1 = new TCanvas("c1", "c1", 2000, 1400);
  c1->Divide(2, 2);

  TFile *f1 = new TFile(fname.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TH2F *h1 = new TH2F("h1", (conf + ": " + meas + ", " + cuts + ", all PDGs").c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  TH2F *h2 = new TH2F("h2", (conf + ": " + meas + ", " + cuts + ", electrons").c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  TH2F *h3 = new TH2F("h3", (conf + ": " + meas + ", " + cuts + ", gammas"   ).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  TH2F *h4 = new TH2F("h4", (conf + ": " + meas + ", " + cuts + ", neutrons" ).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);

  t1->Project("h1", meas.c_str(), cuts.c_str());
  t1->Project("h2", meas.c_str(), ("(" + cuts + ") && abs(pdgID)==11").c_str());
  t1->Project("h3", meas.c_str(), ("(" + cuts + ") && pdgID==22").c_str());
  t1->Project("h4", meas.c_str(), ("(" + cuts + ") && pdgID==2112").c_str());

  c1->cd(1); h1->Draw("colz");
  c1->cd(2); h2->Draw("colz");
  c1->cd(3); h3->Draw("colz");
  c1->cd(4); h4->Draw("colz");
}

void two_by_two_hit_map(string sim, string conf1, string conf2, int n_events_k, string meas1, string meas2, string cuts, Int_t nbinsx, Double_t xlow, Double_t xhi, Int_t nbinsy, Double_t ylow, Double_t yhi){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf1 + ".root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf2 + ".root";

  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 900);
  c1->Divide(2, 2);

  TFile *f1 = new TFile(f1name.c_str());
  TTree *t1 = (TTree*)f1->Get("t");
  TFile *f2 = new TFile(f2name.c_str());
  TTree *t2 = (TTree*)f2->Get("t");
  TH2F *h1 = new TH2F("h1", (conf1 + ": " + meas1 + ", " + cuts).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  TH2F *h2 = new TH2F("h2", (conf2 + ": " + meas1 + ", " + cuts).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  TH2F *h3 = new TH2F("h2", (conf1 + ": " + meas2 + ", " + cuts).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);
  TH2F *h4 = new TH2F("h2", (conf2 + ": " + meas2 + ", " + cuts).c_str(), nbinsx, xlow, xhi, nbinsy, ylow, yhi);

  t1->Project("h1", meas1.c_str(), cuts.c_str());
  t2->Project("h2", meas1.c_str(), cuts.c_str());
  t1->Project("h3", meas2.c_str(), cuts.c_str());
  t2->Project("h4", meas2.c_str(), cuts.c_str());

  c1->cd(1); h1->Draw("colz");
  c1->cd(2); h2->Draw("colz");
  c1->cd(3); h3->Draw("colz");
  c1->cd(4); h4->Draw("colz");
}

void hallRad_comp(string sim, string conf1, string conf2, int n_events_k1, int n_events_k2, string h_type){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + "_hallRad.root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + "_hallRad.root";

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  TFile *f1 = new TFile(f1name.c_str()); TFile *f2 = new TFile(f2name.c_str());
  string hist_name = "hSummary_" + h_type + "LogX";
  TH1D *h1 = (TH1D*)f1->Get(hist_name.c_str());
  TH1D *h2 = (TH1D*)f2->Get(hist_name.c_str());

  h1->SetLineColor(kBlue); h2->SetLineColor(kRed);
  h1->SetTitle( (conf1 + " (Blue), " + conf2 + " (Red): " + h_type).c_str());
  h1->Draw(); h2->Draw("same"); 
}

void hallRad_det_comp(string sim, string conf1, string conf2, int n_events_k1, int n_events_k2, int det, string part, string h_type, string lin_log){
  string f1name = "~/farmOut/" + sim + "_" + conf1 + "_" + to_string(n_events_k1) + "kEv/" + sim + "_" + conf1 + "_hallRad.root";
  string f2name = "~/farmOut/" + sim + "_" + conf2 + "_" + to_string(n_events_k2) + "kEv/" + sim + "_" + conf2 + "_hallRad.root";

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 700);
  TFile *f1 = new TFile(f1name.c_str()); TFile *f2 = new TFile(f2name.c_str());
  string hist_name = "Det_" + to_string(det) + "/ha_" + to_string(det) + "_" + part+ "_" + h_type + lin_log + "X";
  TH1D *h1 = (TH1D*)f1->Get(hist_name.c_str());
  TH1D *h2 = (TH1D*)f2->Get(hist_name.c_str());
  
  h1->SetLineColor(kBlue); h2->SetLineColor(kRed);
  h1->SetTitle( (conf1 + " (Blue), " + conf2 + " (Red): " + to_string(det) + ", " + part + ", " + h_type).c_str());
  h1->Draw("h"); h2->Draw("same && h"); 
}

void check_two_tags(const char* fname, const char* prop1, const char* prop2){
  TFile *f = new TFile(fname);
  TTree *t = (TTree*)f->Get("t");
  
  Float_t value1;
  Int_t value2;

  long nEntries = t->GetEntries();
  for(int i = 0; i < nEntries; i+=1000){
    t->GetEntry(i);
    t->SetBranchAddress(prop1,&value1);
    t->SetBranchAddress(prop2,&value2);

    cout<<"Volume ID for event "<<i<<": "<<value1<<"; "<<value2<<endl;
  }
}

//##############################
//# SPECIAL ANALYSIS FUNCTIONS #
//##############################

void edep_neil_comp(string sim, string conf, int n_events_k){
  string fin_name  = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root";
  string fout_name = sim + "_" + conf + "_plots.root";
  
  gStyle->SetOptStat("eMRuoi");
  TFile *fin = new TFile(fin_name.c_str());
  TTree *t = (TTree*)fin->Get("t");
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");

  string edep_lims1[] = {"0", "1E-6", "1E-3", "1", "10"};   int len_elim1 = sizeof(edep_lims1)/sizeof(edep_lims1[0]);
  string edep_lims2[] = {"0", "1", "30"};                   int len_elim2 = sizeof(edep_lims2)/sizeof(edep_lims2[0]);
  int pdgIDs[]        = {11, 2112};                         int len_pdgID = sizeof(pdgIDs)/sizeof(pdgIDs[0]);
  int det_list[]      = {1001, 1006, 1101, 1102, 2401, 2411};
  int len_dets  = sizeof(det_list)/sizeof(det_list[0]);

  for(int n = 0; n < len_dets; n++){
    cout<<"Making plots for det "<<det_list[n]<<"..."<<endl;
    string en_type = "edep";
    if(det_list[n] >= 2000 && det_list[n] <= 2999) en_type = "kineE";
    else if(det_list[n] >= 3000) en_type = "(edep + kineE)";
    for(int i = 0; i < len_pdgID; i++){
      string part = "";
      if(pdgIDs[i] == 11) part = "e";
      else if(pdgIDs[i] == 2112) part = "n";
      else if(pdgIDs[i] == 22) part = "g";
      if(det_list[n] == 1006){
        for(int j = 0; j < len_elim2; j++){
          string h1name = (en_type + "_" + to_string(det_list[n]) + "_" + part + "_" + edep_lims2[j]).c_str();
          string h2name = ("neil_" + to_string(det_list[n]) + "_" + part + "_" + edep_lims2[j]).c_str();
          if(j == 2){
            string cuts = "abs(pdgID)==" + to_string(pdgIDs[i]) + " && volID==" + to_string(det_list[n]) + " && " + en_type + ">=" + edep_lims2[j];
            TH1F *h1 = new TH1F(h1name.c_str(), (conf + ": " + en_type + ", " + cuts).c_str(), 200, stod(edep_lims2[j]), 400);
            TH1F *h2 = new TH1F(h2name.c_str(), (conf + ": NEIL, " + cuts).c_str(), 200, stod(edep_lims2[j]), 400);
            t->Project(h1name.c_str(), en_type.c_str(), cuts.c_str());
            t->Project(h2name.c_str(), en_type.c_str(), ("neil*(" + cuts + ")").c_str());
            fout->cd();
            h1->Write(); h2->Write();
          }
          else{
            string cuts = "abs(pdgID)==" + to_string(pdgIDs[i]) + " && volID==" + to_string(det_list[n]) + " && " + en_type + ">=" + edep_lims2[j] + " && " + en_type + "<=" + edep_lims2[j + 1]; 
            TH1F *h1 = new TH1F(h1name.c_str(), (conf + ": " + en_type + ", " + cuts).c_str(), 100, stod(edep_lims2[j]), stod(edep_lims2[j + 1]));
            TH1F *h2 = new TH1F(h2name.c_str(), (conf + ": NEIL, " + cuts).c_str(), 100, stod(edep_lims2[j]), stod(edep_lims2[j + 1]));
            t->Project(h1name.c_str(), en_type.c_str(), cuts.c_str());
            t->Project(h2name.c_str(), en_type.c_str(), ("neil*(" + cuts + ")").c_str());
            fout->cd();
            h1->Write(); h2->Write();
          }
        }
      }
      else{
        for(int j = 0; j < len_elim1; j++){
          string h1name = (en_type + "_" + to_string(det_list[n]) + "_" + part + "_" + edep_lims1[j]).c_str();
          string h2name = ("neil_" + to_string(det_list[n]) + "_" + part + "_" + edep_lims1[j]).c_str();
          if(j == 4){

            string cuts = "abs(pdgID)==" + to_string(pdgIDs[i]) + " && volID==" + to_string(det_list[n]) + " && " + en_type + ">=" + edep_lims1[j]; 
            TH1F *h1 = new TH1F(h1name.c_str(), (conf + ": " + en_type + ", " + cuts).c_str(), 200, stod(edep_lims1[j]), 400);
            TH1F *h2 = new TH1F(h2name.c_str(), (conf + ": NEIL, " + cuts).c_str(), 200, stod(edep_lims1[j]), 400);
            t->Project(h1name.c_str(), en_type.c_str(), cuts.c_str());
            t->Project(h2name.c_str(), en_type.c_str(), ("neil*(" + cuts + ")").c_str());
            fout->cd();
            h1->Write(); h2->Write();
          }
          else{
            string cuts = "abs(pdgID)==" + to_string(pdgIDs[i]) + " && volID==" + to_string(det_list[n]) + " && " + en_type + ">=" + edep_lims1[j] + " && " + en_type + "<=" + edep_lims1[j + 1];
            TH1F *h1 = new TH1F(h1name.c_str(), (conf + ": " + en_type + ", " + cuts).c_str(), 100, stod(edep_lims1[j]), stod(edep_lims1[j + 1]));
            TH1F *h2 = new TH1F(h2name.c_str(), (conf + ": NEIL, " + cuts).c_str(), 100, stod(edep_lims1[j]), stod(edep_lims1[j + 1]));
            t->Project(h1name.c_str(), en_type.c_str(), cuts.c_str());
            t->Project(h2name.c_str(), en_type.c_str(), ("neil*(" + cuts + ")").c_str());
            fout->cd();
            h1->Write(); h2->Write();
          }
        }
      }
    }
  }
  fin->Close(); fout->Close();
}
