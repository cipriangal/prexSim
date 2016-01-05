#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <new>
#include <cstdlib>
#include <math.h>

#include "TRandom.h"
#include <TApplication.h>

#include <TH2F.h>
#include <TTree.h>
#include <TF1.h>
#include <Rtypes.h>
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TString.h> 
#include <TDatime.h>
#include <stdexcept>
#include <time.h>
#include <cstdio>
#include <TMath.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TFrame.h>
#include <TDirectory.h>
#include <TObjArray.h>

//Radiation All Vertices
TH1F *Histo_vert_z_full[3];//particle type = photons, electrons, neutrons
//2D histograms
TH2F *Histo_vert_2D[3],*Histo_vert_full_2D[3];
TH1F *Histo_vert_z_weighted_full[3],*Histo_Energy_lt_10[3],*Histo_Energy_gt_10[3];
//create plots for three vertex ranges [ranges][particle type]
TH1F *Histo_vert_z[3][3],*Histo_vert_z_weighted[3][3],*Histo_Energy_custom_lt_10[3][3],*Histo_Energy_custom_gt_10[3][3];

Int_t low_ranges[3],up_ranges[3];
//Flux and power parameters Full range //indices [particle type][energy ranges 0->0.1->10->1000] 
Double_t flux[3][3],power[3][3];
//indices[z range][energy range][particle type]
Double_t flux_range[3][3][3],power_range[3][3][3]; 
float tot_events;
TDirectory* gD;
TString prename;

float SensVolume_v;
//8002 cylindircal det at the radius close to electronic hut
//8003 cyl det at the wall

void bookHisto();
void processTree(TString tname);
void Init();
void WriteHisto(TString fname);


using namespace std;

int main(int argc,char** argv) {

  if(argc < 3){
    cout<<"Usage: anaRad [sensative detector #] [output filename] [additional file names]"<<endl;
    cout<<"   Output files should be of the type: path/Prename_*.root"<<endl;
    return 1;
  }
  TString _sensVol(argv[1]);
  SensVolume_v=_sensVol.Atof();
  
  if(!(SensVolume_v==10008 || SensVolume_v==10009 ||
       SensVolume_v==8003 ||SensVolume_v==8004 ||
       SensVolume_v==8005 || SensVolume_v==10001 ||
       SensVolume_v==10002 || SensVolume_v==10003 ||
       SensVolume_v==10004 )) {
    cout<<" ~~~ You are asking for detector "<<SensVolume_v<<endl;
    cout<<" This code does not work for that detector. Please modify."<<endl;
    return 1;
  }
  cout<<endl;
  cout<<"~~~~ Sensative volue set to :" <<SensVolume_v<<endl;
  cout<<endl;
  
  TString rfile(Form("output/o_xx_%d_Radiation.root",(int)SensVolume_v));
  TFile *rf=new TFile(rfile.Data(),"RECREATE");
  rf->Close();

  Init();
  bookHisto();
  
  /* basic way to process one sim file
  TString inFile="";
  processTree(inFile);
  prename="";
  WriteHisto(rfile);
  */
  for(int i=2;i<argc;i++){
    Init();
    TString inFile(argv[i]);
    prename=inFile;
    prename.Remove(0,prename.First("/")+1);
    prename.Remove(prename.First("_"),prename.Length());
    prename+="_"+_sensVol;
    cout<<"Processing "<<inFile.Data()<<" with prename "<<prename<<endl;
    processTree(inFile);
    WriteHisto(rfile);
  }
  
  return 0;
}

void Init(){
  low_ranges[0]=-2600;
  low_ranges[1]=-110;
  low_ranges[2]= 135;
  up_ranges[0]=-110;
  up_ranges[1]= 135;
  up_ranges[2]= 3400;
  
  
  for(int i=0;i<3;i++){
    if(Histo_vert_z_full[i]){
      Histo_vert_z_full[i]          ->Reset();
      Histo_vert_2D[i]              ->Reset();
      Histo_vert_full_2D[i]         ->Reset();
      Histo_vert_z_weighted_full[i] ->Reset();
      Histo_Energy_lt_10[i]          ->Reset();
      Histo_Energy_gt_10[i]          ->Reset();
    }
    
    for(int j=0;j<3;j++){
      flux[i][j]=0.0;
      power[i][j]=0.0;
      
      if(Histo_vert_z_full[i]){
	Histo_vert_z[i][j]            ->Reset();
	Histo_vert_z_weighted[i][j]   ->Reset();
	Histo_Energy_custom_lt_10[i][j]->Reset();
	Histo_Energy_custom_gt_10[i][j]->Reset();
      }
      
      for(int k=0;k<3;k++){
	flux_range[i][j][k]=0.0;
	power_range[i][j][k]=0.0;
      }
    }
  }
}
 
void bookHisto(){
  char hPnm[3]={'g','e','n'};
  TString pType[3]={"#gamma","e#pm","n"};
  TString sranges[3]={"z < -110 cm","-110 < z < 135 cm","135 < z < 3400 cm"};
 
  for(int i=0;i<3;i++){
    Histo_vert_z_full[i]  = new TH1F(Form("vert_z_full_%c",hPnm[i]),
				     Form("%s Vertices;z (cm);%s per e-",pType[i].Data(),pType[i].Data()) ,300,-2600.,3400.);
    Histo_vert_2D[i]      = new TH2F(Form("vert_2D_%c",hPnm[i]),
				     Form("%s Vertices 2D;z (cm);x (cm);[W/uA]",pType[i].Data())       ,100,-150.,150. ,100,-150. ,150.);
    Histo_vert_full_2D[i] = new TH2F(Form("vert_full_2D%c",hPnm[i]),
				     Form("%s Vertices 2D;z (cm);x (cm);[W/uA]",pType[i].Data())       ,100,-400.,3400.,100,-1000.,1000.);
    
    Histo_vert_z_weighted_full[i]  = new TH1F(Form("vert_z_weighted_full_%c",hPnm[i]),
					      Form("%s Vertices (KE Weighted);z (cm);W/#muA",pType[i].Data()),300,-2600.,3400.);
    Histo_Energy_lt_10[i]           = new TH1F(Form("Energy_lt_10_%c",hPnm[i]),
					      Form("%s E < 10 MeV;Energy (MeV);Counts",pType[i].Data())       ,100,0.,10.);
    Histo_Energy_gt_10[i]           = new TH1F(Form("Energy_gt_10_%c",hPnm[i]),
					      Form("%s 10 < E < 1000 MeV;Energy (MeV);Counts",pType[i].Data()),100,10.,1000.);
 
    for(int j=0;j<3;j++){
      Histo_vert_z[i][j]           = new TH1F(Form("vert_z_rn_%d_%c",j,hPnm[i]),
					      Form("%s Vertices (%s);z (cm);%s per e-",pType[i].Data(),sranges[j].Data(),pType[i].Data()),
					      200,low_ranges[j],up_ranges[j]);
      Histo_vert_z_weighted[i][j]  = new TH1F(Form("vert_z_weighted_rn_%d_%c",j,hPnm[i]),
					      Form("%s Vertices (KE Weighted for %s);z (cm);W/#muA",pType[i].Data(),sranges[j].Data()),
					      300,low_ranges[j],up_ranges[j]);
      
      Histo_Energy_custom_lt_10[i][j] = new TH1F(Form("Energy_custom_lt_10_rn_%d_%c",j,hPnm[i]),
						Form("%s E < 10 MeV (%s);Energy (MeV);Counts",pType[i].Data(),sranges[j].Data()),
						100,0.,10.);
      Histo_Energy_custom_gt_10[i][j] = new TH1F(Form("Energy_custom_gt_10_rn_%d_%c",j,hPnm[i]),
						Form("%s 10 < E < 1000 MeV (%s);Energy (MeV);Counts",pType[i].Data(),sranges[i].Data()),
						100,10.,1000.);
    }
  }
 
}

void PrintInfo(){
 
  TString oName[3]={"Photons  ","Electrons","Neutrons"};
  TString eName[3]={"  0<E<0.1","0.1<E<10"," 10<E<1000"};
  TString zName[3]={"-2600<z<-110"," -110<z<135","  135<z<3400"};
  cout<<" ~Printing:"<<endl;
  ofstream fout(Form("output/o_%s_powerFlux.dat",prename.Data()),std::ofstream::out);
  
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
    {
      fout<<oName[i].Data()<<"\t"<<eName[j].Data()<<"\t"<<power[i][j]<<"\t"<<flux[i][j]<<endl;
      cout<<oName[i].Data()<<"\t"<<eName[j].Data()<<"\t"<<power[i][j]<<"\t"<<flux[i][j]<<endl;
    }
  cout<<endl;
  fout<<endl;
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
      for(int k=0;k<3;k++)
      {
	fout<<oName[i].Data()<<"\t"<<eName[j].Data()<<"\t"<<zName[k].Data()<<"\t"<<power_range[k][j][i]<<"\t"<<flux_range[k][j][i]<<endl;
	cout<<oName[i].Data()<<"\t"<<eName[j].Data()<<"\t"<<zName[k].Data()<<"\t"<<power_range[k][j][i]<<"\t"<<flux_range[k][j][i]<<endl;
      }
      
  fout.close(); 

}

void processTree(TString tname){

  Float_t kineE, type, volume, track, parent, PDGid, Edeposit, event, creator;
  //New Float for simulation of various detectors
  Float_t Energy;
  Float_t px,py,pz;
  Float_t x_0,y_0,z_0,xd,yd,zd;
  
  TChain * t = new TChain("geant");
  t->Add(tname.Data());
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("volume",&volume);
  t->SetBranchAddress("px",&px);
  t->SetBranchAddress("py",&py);
  t->SetBranchAddress("pz",&pz);
  t->SetBranchAddress("x",&xd);  
  t->SetBranchAddress("y",&yd);  
  t->SetBranchAddress("z",&zd);  
  t->SetBranchAddress("x0",&x_0);
  t->SetBranchAddress("y0",&y_0);
  t->SetBranchAddress("z0",&z_0);
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("parent",&parent);  
  t->SetBranchAddress("creator",&creator);  
  t->SetBranchAddress("PDGid",&PDGid);  
  t->SetBranchAddress("track",&track);  
  t->SetBranchAddress("event",&event);

  if ( SensVolume_v==10008 || SensVolume_v==10009){
  t->SetBranchAddress("Edeposit",&Energy); 
  t->SetBranchAddress("kineE",&kineE);
  }else if( SensVolume_v==8004 || SensVolume_v==8005 || SensVolume_v==10001 || SensVolume_v==10002 || SensVolume_v==10003 || SensVolume_v==10004 ) {
    t->SetBranchAddress("kineE",&Energy); 
    t->SetBranchAddress("Edeposit",&Edeposit); //hack to get the deposited energy
  }
  
  Double_t hit_radius_min = 46.038; //cm inner radius of the beam pipe 45.72 cm and outer radius of the beam pipe 46.038 cm
  
  if ( SensVolume_v==8004 || SensVolume_v==8005 || SensVolume_v==10001 || SensVolume_v==10002 || SensVolume_v==10003 || SensVolume_v==10004 )
    hit_radius_min = 0.; //this is for looking at results along the beamline

  Double_t hit_radius;
  int partType[6]={1,1,-1,-1,0,2}; //electron,positron,N/A,N/A,gamma,neutron
  
  Int_t nentries = (Int_t)t->GetEntries();
  for (int i=0; i<nentries ; i++) {
    t->GetEntry(i);
    hit_radius = TMath::Sqrt(TMath::Power(xd/10,2)+TMath::Power(yd/10,2));
     
    if ( volume==SensVolume_v  && z_0 > -26000  && type<=5 && hit_radius > hit_radius_min ){
      int hist=partType[(int)type];
      if(hist==-1) continue;
      //electron vertices (not from beam, the vertex for real raster is at -17720.0 cm)
      if(hist==1 && z_0== -17720) continue;
       
      Histo_vert_z_full[hist]         ->Fill(z_0/10,1/tot_events);
      Histo_vert_z_weighted_full[hist]->Fill(z_0/10,Energy/tot_events);

      Histo_vert_2D[hist]     ->Fill(z_0/10,x_0/10,Energy/tot_events);
      Histo_vert_full_2D[hist]->Fill(z_0/10,x_0/10,Energy/tot_events);

      if (Energy<0.10){
	Histo_Energy_lt_10[hist]->Fill(Energy);
	power[hist][0]+=Energy;
	flux[hist][0]++;
      }else if (Energy>=0.10 && Energy<10){
	Histo_Energy_lt_10[hist]->Fill(Energy);
	power[hist][1]+=Energy;
	flux[hist][1]++;	  
      }else if (Energy>=10 && Energy<1000){
	Histo_Energy_gt_10[hist]->Fill(Energy);
	power[hist][2]+=Energy;
	flux[hist][2]++;	  
      }
      
      for(Int_t j=0;j<3;j++){
	if (low_ranges[j] <= z_0/10 && z_0/10 < up_ranges[j]){
	    Histo_vert_z[j][hist]->Fill(z_0/10,1/tot_events);
	    Histo_vert_z_weighted[j][hist]->Fill(z_0/10,Energy/tot_events);
	    
	    if (Energy<0.10){
	      Histo_Energy_custom_lt_10[j][hist]->Fill(Energy);
	      power_range[j][0][hist]+=Energy;
	      flux_range[j][0][hist]++;
	    }else if (Energy>=0.10 && Energy<10){
	      Histo_Energy_custom_lt_10[j][hist]->Fill(Energy);
	      power_range[j][1][hist]+=Energy;
	      flux_range[j][1][hist]++;
	    }else if (Energy>=10 && Energy<1000){
	      Histo_Energy_custom_gt_10[j][hist]->Fill(Energy);
	      power_range[j][2][hist]+=Energy;
	      flux_range[j][2][hist]++;
	    }
	}
      }
    }
    
       
    if(i % 1000000 == 1 ) cout<<" processed: "<<tname.Data()<<" "<<i<<endl;
  }
  
  PrintInfo();
}

void writeEachHisto(TH1F* a){
  a -> SetName(Form("%s_%s",prename.Data(),a->GetName()));
  a -> SetTitle(Form("%s %s",prename.Data(),a->GetTitle()));
  a -> SetDirectory(gD);
  a -> Write();
}

void writeEachHisto(TH2F* a){
  a -> SetName(Form("%s_%s",prename.Data(),a->GetName()));
  a -> SetTitle(Form("%s %s",prename.Data(),a->GetTitle()));
  a -> SetDirectory(gD);
  a -> Write();
}

void WriteHisto(TString fname){
  
  TFile *fout=TFile::Open(fname.Data(),"UPDATE");
  gD=fout->GetDirectory("");
  
  for(int i=0;i<3;i++){
    writeEachHisto(Histo_vert_z_full[i]);
    writeEachHisto(Histo_vert_2D[i]);
    writeEachHisto(Histo_vert_full_2D[i]);
    writeEachHisto(Histo_vert_z_weighted_full[i]);
    writeEachHisto(Histo_Energy_lt_10[i]);
    writeEachHisto(Histo_Energy_gt_10[i]); 
    for(int j=0;j<3;j++){
      writeEachHisto(Histo_vert_z[i][j]);
      writeEachHisto(Histo_vert_z_weighted[i][j]);
      writeEachHisto(Histo_Energy_custom_lt_10[i][j]);
      writeEachHisto(Histo_Energy_custom_gt_10[i][j]);
    }
  }
  
  fout->Close();
}

