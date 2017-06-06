#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <stdio.h>
#include <iostream>

using namespace std;

void sumRootFiles(){
  int nfiles = 10;
  string fnms[nfiles];
  
  for(int i = 0; i < nfiles; i++){
  	char fbuf[100];
  	if(i < 10){sprintf(fbuf, "../output/prexII_100kEv_000%d_concWShields_septumHoses/o_prexSim.root", i); }
  	else{sprintf(fbuf, "../output/prexII_100kEv_00%d_concWShields_septumHoses/o_prexSim.root", i);}
  	fnms[i] = (string)fbuf;
  }
  
  TChain ch("geant");
  
  for(int i = 0; i < nfiles; i++){
  	printf(fnms[i].c_str()); printf("\n");
  	ch.Add(fnms[i].c_str());
  }
  
  ch.Merge("prexII_concWShields_septumHoses.root");
  
}

void plotEdep(int vol){
	char vol_buf[100];
	TFile *f = new TFile("../output/prexII_concWShields_septumHoses.root");
	TTree *geant = (TTree*)f->Get("geant");
	
	sprintf(vol_buf, "volume==%d", vol);
	geant->Draw("Edeposit",vol_buf);
}

void hitMapQuad(const char* fname){
	//char vol_buf[100];
	TFile *f = new TFile(fname);
	TTree *geant = (TTree*)f->Get("geant");
	TCanvas *c1 = new TCanvas("c1","multipads",900,700);
	c1->Divide(2,2);
	
	c1->cd(1);
	geant->Draw("y:x","Edeposit*volume==3202","colz");
	
	c1->cd(2);
	geant->Draw("y:x","Edeposit*volume==3201","colz");
	
	c1->cd(3);
	geant->Draw("y:x","Edeposit*volume==3204","colz");
	
	c1->cd(4);
	geant->Draw("y:x","Edeposit*volume==3203","colz");
	//sprintf(vol_buf,"Edeposit*");
	//sprintf(vol_buf,"volume==%d", vol);
	
}

void hitMap(const char* fname, int vol){
	char vol_buf[100];
	TFile *f = new TFile(fname);
	TTree *geant = (TTree*)f->Get("geant");
	
	sprintf(vol_buf,"Edeposit*");
	sprintf(vol_buf,"volume==%d", vol);
	
	geant->Draw("y:x",vol_buf,"colz");
	
}

void edep_histo(const char* fname, int vol){
	char param_buff[200];
	TFile *f = new TFile(fname);
	TTree *geant = (TTree*)f->Get("geant");
	
	gStyle->SetOptStat("eMRiou");
	
	if(vol == 3201){sprintf(param_buff,"volume==%d && x>=127 && x<=177 && y>=302.125 && y<=352.125",vol);}
	else if(vol == 3202){sprintf(param_buff,"volume==%d && x<=-127 && x>=-177 && y>=302.125 && y<=352.125",vol);}
	else if(vol == 3203){sprintf(param_buff,"volume==%d && x>=127 && x<=177 && y<=-302.125 && y>=-352.125",vol);}
	else if(vol == 3204){sprintf(param_buff,"volume==%d && x<=-127 && x>=-177 && y<=-302.125 && y>=-352.125",vol);}
	
	std::cout << param_buff << endl;
	
	geant->Draw("Edeposit",param_buff);
}
