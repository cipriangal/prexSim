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
#include "math.h"

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
  int det_list[]      = {1001, 1006, 9001, 9002};
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

void coll_shields_analysis(string sim, string conf, int n_events_k, Int_t nbinsx, Double_t xlo, Double_t xhi){
  string fin_name  = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root";
  string fout_name = sim + "_" + conf + "_plots.root";
  
  gStyle->SetOptStat("eMRuoi");
  TFile *fin = new TFile(fin_name.c_str());
  TTree *t = (TTree*)fin->Get("t");
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");

  double plast_min = 152.4;
  double conc_min  = 58.928;
  double flange_min = 8.7249;
  cout<<"Making plots for detector 3121..."<<endl;
  TH1F *h1 = new TH1F("3121_all", (conf + ": edep, volID==3121").c_str(), nbinsx, xlo, xhi);
  t->Project("3121_all", "edep", "volID==3121");
  fout->cd(); h1->Write();
  for(double r = plast_min + 0.5; r < plast_min + 5.5; r += 0.5){
    cout<<"  Making plot with index "<<r<<endl;
    string h_name = "3121_r=" + to_string(r) + "cut";
    string cuts = "volID==3121 && z>-655 && z<-430 && sqrt(x*x+y*y)<" + to_string(r);
    h1 = new TH1F(h_name.c_str(), (conf + ": edep, " + cuts).c_str(), nbinsx, xlo, xhi);
    t->Project(h_name.c_str(), "edep", cuts.c_str());
    fout->cd(); h1->Write();
  }
  cout<<"Making plots for detector 4101..."<<endl;
  TH1F *h2 = new TH1F("4101_all", (conf + ": edep, volID==4101").c_str(), nbinsx, xlo, xhi);
  t->Project("4101_all", "edep", "volID==4101");
  fout->cd(); h2->Write();
  for(double y = conc_min + 0.5; y < 65; y += 0.5){
    cout<<"  Making plot with index "<<y<<endl;
    string h_name = "4101_y=" + to_string(y) + "cut";
    string cuts = "volID==4101 && x>-10 && x<10 && y<" + to_string(y);
    h2 = new TH1F(h_name.c_str(), (conf + ": edep, " + cuts).c_str(), nbinsx, xlo, xhi);
    t->Project(h_name.c_str(), "edep", cuts.c_str());
    fout->cd(); h2->Write();
  }
  cout<<"Making plots for detector 4102..."<<endl;
  TH1F *h3 = new TH1F("4102_all", (conf + ": edep, volID==4102").c_str(), nbinsx, xlo, xhi);
  t->Project("4102_all", "edep", "volID==4102");
  fout->cd(); h3->Write();
  for(double r = flange_min + 0.5; r < 12; r += 0.5){
    cout<<"  Making plot with index "<<r<<endl;
    string h_name = "4102_r=" + to_string(r) + "cut";
    string cuts = "volID==4102 && sqrt(x*x+y*y)<" + to_string(r);
    h3 = new TH1F(h_name.c_str(), (conf + ": edep, " + cuts).c_str(), nbinsx, xlo, xhi);
    t->Project(h_name.c_str(), "edep", cuts.c_str());
    fout->cd(); h3->Write();
  }
  fin->Close(); fout->Close();
}

void sphere_det_subtraction_phi_theta(string sim, string conf, int n_events_k, Int_t nbinsx, Double_t xlo, Double_t xhi, Int_t nbinsy, Double_t ylo, Double_t yhi){
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + ".root";
  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  string iDet_innCut = "kineE*(volID==2103 && (x*px+y*py+(z+16.7478)*pz)/(sqrt(x*x+y*y+(z+16.7478)*(z+16.7478))*sqrt(px*px+py*py+pz*pz))<0)";
  string iDet_outCut = "kineE*(volID==2103 && (x*px+y*py+(z+16.7478)*pz)/(sqrt(x*x+y*y+(z+16.7478)*(z+16.7478))*sqrt(px*px+py*py+pz*pz))>0)";
  string oDet_innCut = "kineE*(volID==2104 && (x*px+y*py+(z+16.7478)*pz)/(sqrt(x*x+y*y+(z+16.7478)*(z+16.7478))*sqrt(px*px+py*py+pz*pz))<0)";
  string oDet_outCut = "kineE*(volID==2104 && (x*px+y*py+(z+16.7478)*pz)/(sqrt(x*x+y*y+(z+16.7478)*(z+16.7478))*sqrt(px*px+py*py+pz*pz))>0)";

  TH2F *h1 = new TH2F("h1", (conf + ": phi:theta, inner sphere, inner radiation").c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi);
  TH2F *h2 = new TH2F("h2", (conf + ": phi:theta, inner sphere, outer radiation").c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi);
  TH2F *h3 = new TH2F("h3", (conf + ": phi:theta, outer sphere, inner radiation").c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi);
  TH2F *h4 = new TH2F("h4", (conf + ": phi:theta, outer sphere, outer radiation").c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi);
  TH2F *h5 = new TH2F("h5", (conf + ": phi:theta, inner sphere, radiation difference").c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi);
  TH2F *h6 = new TH2F("h6", (conf + ": phi:theta, outer sphere, radiation difference").c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi);
  TH2F *h7 = new TH2F("h7", (conf + ": phi:theta, sDet difference").c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi);
 
  t->Project("h1", "acos(x/sqrt(x*x+y*y)):(3.14159 - acos((z+16.7478)/sqrt(x*x+y*y+(z+16.7478)*(z+16.7478))))", iDet_innCut.c_str());
  t->Project("h2", "acos(x/sqrt(x*x+y*y)):acos((z+16.7478)/sqrt(x*x+y*y+(z+16.7478)*(z+16.7478)))", iDet_outCut.c_str());
  t->Project("h3", "acos(x/sqrt(x*x+y*y)):(3.14159 - acos((z+16.7478)/sqrt(x*x+y*y+(z+16.7478)*(z+16.7478))))", oDet_innCut.c_str());
  t->Project("h4", "acos(x/sqrt(x*x+y*y)):acos((z+16.7478)/sqrt(x*x+y*y+(z+16.7478)*(z+16.7478)))", oDet_outCut.c_str());

  h5->Add(h2, 1); h5->Add(h1, -1);
  h6->Add(h4, 1); h6->Add(h3, -1);
  h7->Add(h5, 1); h7->Add(h6, -1);

  gStyle->SetOptStat("eMRiou");
  h1->GetXaxis()->SetTitle("theta"); h1->GetYaxis()->SetTitle("phi"); h2->GetXaxis()->SetTitle("theta"); h2->GetYaxis()->SetTitle("phi");
  h3->GetXaxis()->SetTitle("theta"); h3->GetYaxis()->SetTitle("phi"); h4->GetXaxis()->SetTitle("theta"); h4->GetYaxis()->SetTitle("phi");
  h5->GetXaxis()->SetTitle("theta"); h5->GetYaxis()->SetTitle("phi"); h6->GetXaxis()->SetTitle("theta"); h6->GetYaxis()->SetTitle("phi");
  h7->GetXaxis()->SetTitle("theta"); h7->GetYaxis()->SetTitle("phi");

  TH1D* h8 = h5->ProjectionX("h8", 0, 199); TH1D* h9 = h6->ProjectionX("h9", 0, 199); TH1D* h10 = h7->ProjectionX("h10", 0, 199);
  h8->GetXaxis()->SetTitle("theta"); h9->GetXaxis()->SetTitle("theta"); h10->GetXaxis()->SetTitle("theta");
  
  string fout_name = sim + "_" + conf + "_coll_sDet_plots.root";
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");
  fout->cd();
  h1->Write(); h2->Write(); h3->Write(); h4->Write(); h5->Write(); h6->Write(); h7->Write(); h8->Write(); h9->Write(); h10->Write();
  fout->Close();
}

void grand_analysis_krip(string sim, string conf, int n_events_k){
  cout<<"Analyzing Krip Dets..."<<endl;
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + "KripDets.root"; 

  TH1F *h1_1001, *h2_1001, *h3_1001, *h4_1001, *h5_1001, *h6_1001, *h7_1001, *h8_1001;
  TH1F *h9_1001, *h10_1001, *h11_1001, *h12_1001, *h13_1001, *h14_1001, *h15_1001, *h16_1001;
  TH1F *h1_1007, *h2_1007, *h3_1007, *h4_1007, *h5_1007, *h6_1007, *h7_1007, *h8_1007;
  TH1F *h9_1007, *h10_1007, *h11_1007, *h12_1007, *h13_1007, *h14_1007, *h15_1007, *h16_1007;
  TH1F *h1_1006, *h3_1006;
  TH2F *h17_1001, *h17_1007, *h2_1006;
  gStyle->SetOptStat("eMRiou");

  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  string e_low_cut = "abs(pdgID)==11 && edep<1";
  string e_med_cut = "abs(pdgID)==11 && edep>1 && edep<10";
  string e_hi_cut  = "abs(pdgID)==11 && edep>10";
  string n_therm   = "pdgID==2112 && edep<1E-6";
  string n_slow    = "pdgID==2112 && edep>1E-6 && edep<1E-3";
  string n_interm  = "pdgID==2112 && edep>1E-3 && edep<1";
  string n_fast    = "pdgID==2112 && edep>1 && edep<10";
  string n_ufast   = "pdgID==2112 && edep>10";
  string lHRS = "volID==1001";
  string rHRS = "volID==1007";
  string hallLid_cuts  = "volID==1006 && pdgID==2112 && edep>30";
  string hallLid_cuts2 = "volID==1006 && abs(pdgID)==11 && edep>10"; 

  h1_1001 = new TH1F("1001_e_lo_neil", ("lHRS: neil, " + e_low_cut).c_str(), 100, 0, 1);
  h1_1007 = new TH1F("1007_e_lo_neil", ("rHRS: neil, " + e_low_cut).c_str(), 100, 0, 1);
  h9_1001 = new TH1F("1001_e_lo_edep", ("lHRS: edep, " + e_low_cut).c_str(), 100, 0, 1);
  h9_1007 = new TH1F("1007_e_lo_edep", ("rHRS: edep, " + e_low_cut).c_str(), 100, 0, 1);
  t->Project("1001_e_lo_neil", "edep", ("neil*(" + lHRS + " && " + e_low_cut + ")").c_str());
  t->Project("1007_e_lo_neil", "edep", ("neil*(" + rHRS + " && " + e_low_cut + ")").c_str());
  t->Project("1001_e_lo_edep", "edep", (lHRS + " && " + e_low_cut).c_str());
  t->Project("1007_e_lo_edep", "edep", (rHRS + " && " + e_low_cut).c_str());
  cout<<"Low edep e's plotted..."<<endl;
  h2_1001 = new TH1F("1001_e_md_neil", ("lHRS: neil, " + e_med_cut).c_str(), 100, 1, 10);
  h2_1007 = new TH1F("1007_e_md_neil", ("rHRS: neil, " + e_med_cut).c_str(), 100, 1, 10);
  h10_1001 = new TH1F("1001_e_md_edep", ("lHRS: edep, " + e_med_cut).c_str(), 100, 1, 10);
  h10_1007 = new TH1F("1007_e_md_edep", ("rHRS: edep, " + e_med_cut).c_str(), 100, 1, 10);
  t->Project("1001_e_md_neil", "edep", ("neil*(" + lHRS + " && " + e_med_cut + ")").c_str());
  t->Project("1007_e_md_neil", "edep", ("neil*(" + rHRS + " && " + e_med_cut + ")").c_str());
  t->Project("1001_e_md_edep", "edep", (lHRS + " && " + e_med_cut).c_str());
  t->Project("1007_e_md_edep", "edep", (rHRS + " && " + e_med_cut).c_str());
  cout<<"Med edep e's plotted..."<<endl;
  h3_1001 = new TH1F("1001_e_hi_neil", ("lHRS: neil, " + e_hi_cut).c_str(), 100, 10, 1000);
  h3_1007 = new TH1F("1007_e_hi_neil", ("rHRS: neil, " + e_hi_cut).c_str(), 100, 10, 1000);
  h11_1001 = new TH1F("1001_e_hi_edep", ("lHRS: edep, " + e_hi_cut).c_str(), 100, 10, 1000);
  h11_1007 = new TH1F("1007_e_hi_edep", ("rHRS: edep, " + e_hi_cut).c_str(), 100, 10, 1000);
  t->Project("1001_e_hi_neil", "edep", ("neil*(" + lHRS + " && " + e_hi_cut + ")").c_str());
  t->Project("1007_e_hi_neil", "edep", ("neil*(" + rHRS + " && " + e_hi_cut + ")").c_str());
  t->Project("1001_e_hi_edep", "edep", (lHRS + " && " + e_hi_cut).c_str());
  t->Project("1007_e_hi_edep", "edep", (rHRS + " && " + e_hi_cut).c_str());
  cout<<"High edep e's plotted..."<<endl;

  h4_1001 = new TH1F("1001_n_therm_neil", ("lHRS: neil, " + n_therm).c_str(), 100, 0, 1E-6);
  h4_1007 = new TH1F("1007_n_therm_neil", ("rHRS: neil, " + n_therm).c_str(), 100, 0, 1E-6);
  h12_1001 = new TH1F("1001_n_therm_edep", ("lHRS: edep, " + n_therm).c_str(), 100, 0, 1E-6);
  h12_1007 = new TH1F("1007_n_therm_edep", ("rHRS: edep, " + n_therm).c_str(), 100, 0, 1E-6);
  t->Project("1001_n_therm_neil", "edep", ("neil*(" + lHRS + " && " + n_therm + ")").c_str());
  t->Project("1007_n_therm_neil", "edep", ("neil*(" + rHRS + " && " + n_therm + ")").c_str());
  t->Project("1001_n_therm_edep", "edep", (lHRS + " && " + n_therm).c_str());
  t->Project("1007_n_therm_edep", "edep", (rHRS + " && " + n_therm).c_str());
  cout<<"Therm n's plotted..."<<endl;
  h5_1001 = new TH1F("1001_n_slow_neil", ("lHRS: neil, " + n_slow).c_str(), 100, 1E-6, 1E-3);
  h5_1007 = new TH1F("1007_n_slow_neil", ("rHRS: neil, " + n_slow).c_str(), 100, 1E-6, 1E-3);
  h13_1001 = new TH1F("1001_n_slow_edep", ("lHRS: edep, " + n_slow).c_str(), 100, 1E-6, 1E-3);
  h13_1007 = new TH1F("1007_n_slow_edep", ("rHRS: edep, " + n_slow).c_str(), 100, 1E-6, 1E-3);
  t->Project("1001_n_slow_neil", "edep", ("neil*(" + lHRS + " && " + n_slow + ")").c_str());
  t->Project("1007_n_slow_neil", "edep", ("neil*(" + rHRS + " && " + n_slow + ")").c_str());
  t->Project("1001_n_slow_edep", "edep", (lHRS + " && " + n_slow).c_str());
  t->Project("1007_n_slow_edep", "edep", (rHRS + " && " + n_slow).c_str());
  cout<<"Slow n's plotted..."<<endl;
  h6_1001 = new TH1F("1001_n_interm_neil", ("lHRS: neil, " + n_interm).c_str(), 100, 1E-3, 1);
  h6_1007 = new TH1F("1007_n_interm_neil", ("rHRS: neil, " + n_interm).c_str(), 100, 1E-3, 1);
  h14_1001 = new TH1F("1001_n_interm_edep", ("lHRS: edep, " + n_interm).c_str(), 100, 1E-3, 1);
  h14_1007 = new TH1F("1007_n_interm_edep", ("rHRS: edep, " + n_interm).c_str(), 100, 1E-3, 1);
  t->Project("1001_n_interm_neil", "edep", ("neil*(" + lHRS + " && " + n_interm + ")").c_str());
  t->Project("1007_n_interm_neil", "edep", ("neil*(" + rHRS + " && " + n_interm + ")").c_str());
  t->Project("1001_n_interm_edep", "edep", (lHRS + " && " + n_interm).c_str());
  t->Project("1007_n_interm_edep", "edep", (rHRS + " && " + n_interm).c_str());
  cout<<"Interm n's plotted..."<<endl;
  h7_1001 = new TH1F("1001_n_fast_neil", ("lHRS: neil, " + n_fast).c_str(), 100, 1, 10);
  h7_1007 = new TH1F("1007_n_fast_neil", ("rHRS: neil, " + n_fast).c_str(), 100, 1, 10);
  h15_1001 = new TH1F("1001_n_fast_edep", ("lHRS: edep, " + n_fast).c_str(), 100, 1, 10);
  h15_1007 = new TH1F("1007_n_fast_edep", ("rHRS: edep, " + n_fast).c_str(), 100, 1, 10);
  t->Project("1001_n_fast_neil", "edep", ("neil*(" + lHRS + " && " + n_fast + ")").c_str());
  t->Project("1007_n_fast_neil", "edep", ("neil*(" + rHRS + " && " + n_fast + ")").c_str());
  t->Project("1001_n_fast_edep", "edep", (lHRS + " && " + n_fast).c_str());
  t->Project("1007_n_fast_edep", "edep", (rHRS + " && " + n_fast).c_str());
  cout<<"Fast n's plotted..."<<endl;
  h8_1001 = new TH1F("1001_n_ufast_neil", ("lHRS: neil, " + n_ufast).c_str(), 100, 10, 500);
  h8_1007 = new TH1F("1007_n_ufast_neil", ("rHRS: neil, " + n_ufast).c_str(), 100, 10, 500);
  h16_1001 = new TH1F("1001_n_ufast_edep", ("lHRS: edep, " + n_ufast).c_str(), 100, 10, 500);
  h16_1007 = new TH1F("1007_n_ufast_edep", ("rHRS: edep, " + n_ufast).c_str(), 100, 10, 500);
  t->Project("1001_n_ufast_neil", "edep", ("neil*(" + lHRS + " && " + n_ufast + ")").c_str());
  t->Project("1007_n_ufast_neil", "edep", ("neil*(" + rHRS + " && " + n_ufast + ")").c_str());
  t->Project("1001_n_ufast_edep", "edep", (lHRS + " && " + n_ufast).c_str());
  t->Project("1007_n_ufast_edep", "edep", (rHRS + " && " + n_ufast).c_str());
  cout<<"Ultrafast n's plotted..."<<endl;
  
  h1_1006 = new TH1F("1006_n_ufast_edep", ("Hall Lid: n edep, " + hallLid_cuts).c_str(), 200, 30, 500);
  h3_1006 = new TH1F("1006_e_hi_edep", ("Hall Lid: e edep, " + hallLid_cuts2).c_str(), 300, 10, 600);
  t->Project("1006_n_ufast_edep", "edep", hallLid_cuts.c_str());
  t->Project("1006_e_hi_edep", "edep", hallLid_cuts2.c_str());
  cout<<"Hall Lid plotted..."<<endl;

  h17_1001 = new TH2F("1001_x0z0_map", "Left HRS: xz", 400, -32000, 32000, 400, -32000, 32000);
  h17_1007 = new TH2F("1007_x0z0_map", "Right HRS: xz", 400, -32000, 32000, 400, -32000, 32000);
  h2_1006 = new TH2F("1006_x0z0_map", "Hall Lid: xz", 400, -32000, 32000, 400, -32000, 32000);
  t->Project("1001_x0z0_map", "x0:z0", "edep*(volID==1001)");
  t->Project("1007_x0z0_map", "x0:z0", "edep*(volID==1007)");
  t->Project("1006_x0z0_map", "x0:z0", "edep*(volID==1006)");
  cout<<"Particle maps plotted..."<<endl;
  
  string fout_name = sim + "_" + conf + "_kripDets_plots.root";
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");
  fout->cd();
  h1_1001->Write(); h2_1001->Write(); h3_1001->Write(); h4_1001->Write(); 
  h5_1001->Write(); h6_1001->Write(); h7_1001->Write(); h8_1001->Write();
  h9_1001->Write(); h10_1001->Write(); h11_1001->Write(); h12_1001->Write(); 
  h13_1001->Write(); h14_1001->Write(); h15_1001->Write(); h16_1001->Write();
  h1_1007->Write(); h2_1007->Write(); h3_1007->Write(); h4_1007->Write(); 
  h5_1007->Write(); h6_1007->Write(); h7_1007->Write(); h8_1007->Write();
  h9_1007->Write(); h10_1007->Write(); h11_1007->Write(); h12_1007->Write(); 
  h13_1007->Write(); h14_1007->Write(); h15_1007->Write(); h16_1007->Write();
  h1_1006->Write(); h3_1006->Write();
  h17_1001->Write(); h17_1007->Write(); h2_1006->Write();
  fout->Close();
}

void grand_analysis_collimator(string sim, string conf, int n_events_k){
  cout<<"Analyzing collimator dets, please wait..."<<endl;
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + "CollDets.root";

  TH1F *h1_2101, *h1_2102;

  gStyle->SetOptStat("eMRiou");
  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  h1_2101 = new TH1F("2101_kineE_pzCut", "Coll US Det: kineE, pz > 0", 230, 0, 2300);
  h1_2102 = new TH1F("2102_kineE_pzCut", "Coll DS Det: kineE, pz > 0", 230, 0, 2300);
  t->Project("2101_kineE_pzCut", "kineE", "volID==2101 && pz>0");
  cout<<"US Det plot made..."<<endl;
  t->Project("2102_kineE_pzCut", "kineE", "volID==2102 && pz>0");
  cout<<"DS Det plot made..."<<endl;

  string fout_name = sim + "_" + conf + "_collDets_plots.root";
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");
  fout->cd();
  h1_2101->Write(); h1_2102->Write(); 
  fout->Close();
}

void grand_analysis_plast_shields(string sim, string conf, int n_events_k){
  cout<<"Analyzing collimator shield dets, please wait..."<<endl;
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + "PlastShieldDets.root";

  TH1F *h1_3121, *h1_3122, *h2_3121, *h2_3122;
  TH2F *h3_3121, *h3_3122;

  gStyle->SetOptStat("eMRiou");
  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  h1_3121 = new TH1F("3121_edep", "Plast Shld Block Det: edep", 200, 0, 1000);
  h1_3122 = new TH1F("3122_edep", "Targ Shield: edep", 200, 0, 1000);
  h2_3121 = new TH1F("3121_edep_inner_bore", "Plast Block r<16.875: edep", 200, 0, 1000);
  h2_3122 = new TH1F("3122_edep_inner_bore", "Targ Shld r<16.875: edep", 200, 0, 1000);
  h3_3121 = new TH2F("3121_yz_map", "Plast Block: yz map", 400, -750, -190, 400, -500, 500);
  h3_3122 = new TH2F("3122_yz_map", "Targ Shld: yz map", 400, -950, -750, 400, -400, 400);
  t->Project("3121_edep", "edep", "volID==3121");
  cout<<"Plast Det plot 1/3 made..."<<endl;
  t->Project("3121_edep_inner_bore", "edep", "volID==3121 && sqrt(x*x + y*y)<168.75 && z<-425.14 && z>-653.74");
  cout<<"Plast Det plot 2/3 made..."<<endl;
  t->Project("3121_yz_map", "y:z", "edep*(volID==3121)");
  cout<<"Plast Det plot 3/3 made..."<<endl;
  t->Project("3122_edep", "edep", "volID==3122");
  cout<<"Targ Shld plot 1/3 made..."<<endl;
  t->Project("3122_edep_inner_bore", "edep", "volID==3122 && sqrt(x*x + y*y)<168.75 && z<-852.75 && z>-928.95");
  cout<<"Targ Shld plot 2/3 made..."<<endl;
  t->Project("3122_yz_map", "y:z", "edep*(volID==3122)");
  cout<<"Targ Shld plot 3/3 made..."<<endl;

  string fout_name = sim + "_" + conf + "_collShieldDets_plots.root";
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");
  fout->cd();
  h1_3121->Write(); h1_3122->Write(); h2_3121->Write(); h2_3122->Write(); h3_3121->Write(); h3_3122->Write();
  fout->Close();
}

void grand_analysis_coil_hoses(string sim, string conf, int n_events_k){
  cout<<"Analyzing coil hose dets, please wait..."<<endl;
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + "CoilHoseDets.root";

  TH1F *h1_3701, *h1_3702, *h1_3703, *h1_3704, *h1_3705, *h1_3706, *h1_3707, *h1_3708;
  TH1F *h1_3709, *h1_3710, *h1_3711, *h1_3712, *h1_3713, *h1_3714, *h1_3715, *h1_3716;

  gStyle->SetOptStat("eMRiou");
  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  h1_3701 = new TH1F("3701_edep", "Coil Hose 3701: edep", 200, 0, 1000);
  h1_3702 = new TH1F("3702_edep", "Coil Hose 3702: edep", 200, 0, 1000);
  h1_3703 = new TH1F("3703_edep", "Coil Hose 3703: edep", 200, 0, 1000);
  h1_3704 = new TH1F("3704_edep", "Coil Hose 3704: edep", 200, 0, 1000);
  h1_3705 = new TH1F("3705_edep", "Coil Hose 3705: edep", 200, 0, 1000);
  h1_3706 = new TH1F("3706_edep", "Coil Hose 3706: edep", 200, 0, 1000);
  h1_3707 = new TH1F("3707_edep", "Coil Hose 3707: edep", 200, 0, 1000);
  h1_3708 = new TH1F("3708_edep", "Coil Hose 3708: edep", 200, 0, 1000);
  h1_3709 = new TH1F("3709_edep", "Coil Hose 3709: edep", 200, 0, 1000);
  h1_3710 = new TH1F("3710_edep", "Coil Hose 3710: edep", 200, 0, 1000);
  h1_3711 = new TH1F("3711_edep", "Coil Hose 3711: edep", 200, 0, 1000);
  h1_3712 = new TH1F("3712_edep", "Coil Hose 3712: edep", 200, 0, 1000);
  h1_3713 = new TH1F("3713_edep", "Coil Hose 3713: edep", 200, 0, 1000);
  h1_3714 = new TH1F("3714_edep", "Coil Hose 3714: edep", 200, 0, 1000);
  h1_3715 = new TH1F("3715_edep", "Coil Hose 3715: edep", 200, 0, 1000);
  h1_3716 = new TH1F("3716_edep", "Coil Hose 3716: edep", 200, 0, 1000);
  t->Project("3701_edep", "edep", "volID==3701");
  t->Project("3702_edep", "edep", "volID==3702");
  t->Project("3703_edep", "edep", "volID==3703");
  t->Project("3704_edep", "edep", "volID==3704");
  cout<<"25 pct done with hose plots..."<<endl;
  t->Project("3705_edep", "edep", "volID==3705");
  t->Project("3706_edep", "edep", "volID==3706");
  t->Project("3707_edep", "edep", "volID==3707");
  t->Project("3708_edep", "edep", "volID==3708");
  cout<<"50 pct done with hose plots..."<<endl;
  t->Project("3709_edep", "edep", "volID==3709");
  t->Project("3710_edep", "edep", "volID==3710");
  t->Project("3711_edep", "edep", "volID==3711");
  t->Project("3712_edep", "edep", "volID==3712");
  cout<<"75 pct done with hose plots..."<<endl;
  t->Project("3713_edep", "edep", "volID==3713");
  t->Project("3714_edep", "edep", "volID==3714");
  t->Project("3715_edep", "edep", "volID==3715");
  t->Project("3716_edep", "edep", "volID==3716");
  cout<<"100 pct done with hose plots!"<<endl;

  string fout_name = sim + "_" + conf + "_coilHoseDets_plots.root";
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");
  fout->cd();
  h1_3701->Write(); h1_3702->Write(); h1_3703->Write(); h1_3704->Write(); h1_3705->Write(); h1_3706->Write(); h1_3707->Write(); h1_3708->Write();
  h1_3709->Write(); h1_3710->Write(); h1_3711->Write(); h1_3712->Write(); h1_3713->Write(); h1_3714->Write(); h1_3715->Write(); h1_3716->Write();
  fout->Close();
}

void grand_analysis_q1_hoses(string sim, string conf, int n_events_k){
  cout<<"Analyzing coil hose dets, please wait..."<<endl;
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + "Q1HoseDets.root";

  TH1F *h1_3211, *h1_3212, *h1_3213, *h1_3214;

  gStyle->SetOptStat("eMRiou");
  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  h1_3211 = new TH1F("3211_edep", "Coil Hose 3211: edep", 200, 0, 1000);
  h1_3212 = new TH1F("3212_edep", "Coil Hose 3212: edep", 200, 0, 1000);
  h1_3213 = new TH1F("3213_edep", "Coil Hose 3213: edep", 200, 0, 1000);
  h1_3214 = new TH1F("3214_edep", "Coil Hose 3214: edep", 200, 0, 1000);
  t->Project("3211_edep", "edep", "volID==3211");
  cout<<"Q1 hose one finished..."<<endl;
  t->Project("3212_edep", "edep", "volID==3212");
  cout<<"Q1 hose two finished..."<<endl;
  t->Project("3213_edep", "edep", "volID==3213");
  cout<<"Q1 hose three finished..."<<endl;
  t->Project("3214_edep", "edep", "volID==3214");
  cout<<"Q1 hose four finished..."<<endl;

  string fout_name = sim + "_" + conf + "_Q1HoseDets_plots.root";
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");
  fout->cd();
  h1_3211->Write(); h1_3212->Write(); h1_3213->Write(); h1_3214->Write();
  fout->Close();
}

void grand_analysis_us_dets(string sim, string conf, int n_events_k){
  cout<<"Analyzing Moller and Compton Detectors, please wait..."<<endl;
  string fname = "~/farmOut/" + sim + "_" + conf + "_" + to_string(n_events_k) + "kEv/" + sim + "_" + conf + "USDets.root";

  TH1F *h1_2401, *h1_2411;

  gStyle->SetOptStat("eMRiou");
  TFile *f = new TFile(fname.c_str());
  TTree* t = (TTree*)f->Get("t");

  h1_2401 = new TH1F("2401_kineE", "Moller Det: kineE", 200, 0, 1000);
  h1_2411 = new TH1F("2411_kineE", "Compton Det: kineE", 200, 0, 1000);
  t->Project("2401_kineE", "kineE", "volID==2401");
  t->Project("2411_kineE", "kineE", "volID==2411");

  string fout_name = sim + "_" + conf + "_USDets_plots.root";
  TFile *fout = new TFile(fout_name.c_str(), "RECREATE");
  fout->cd();
  h1_2401->Write(); h1_2411->Write();
  fout->Close();
}

void grand_analysis(string sim, string conf, int n_events_k){
  cout<<"Running grand analysis... this will take some time. Please be patient."<<endl;
  grand_analysis_krip(sim, conf, n_events_k);
  grand_analysis_collimator(sim, conf, n_events_k);
  grand_analysis_plast_shields(sim, conf, n_events_k);
  grand_analysis_coil_hoses(sim, conf, n_events_k);
  grand_analysis_q1_hoses(sim, conf, n_events_k);
  cout<<"Grand Analysis finished!"<<endl;
}
