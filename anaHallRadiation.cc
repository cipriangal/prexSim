#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TTree.h>
#include <TF1.h>
#include <TFile.h>
#include <TChain.h>
#include <TDirectory.h>

using namespace std;

//Radiation All Vertices
TH1F *Histo_vert_z_full[3];//particle type = photons, electrons, neutrons
//2D histograms
TH2F *Histo_vert_2D[3],*Histo_vert_full_2D[3];
TH1F *Histo_vert_z_weighted_full[3],*Histo_Energy_lt_10[3],*Histo_Energy_gt_10[3];
//create plots for three vertex ranges [ranges][particle type]
TH1F *Histo_vert_z[3][3],*Histo_vert_z_weighted[3][3],*Histo_Energy_custom_lt_10[3][3],*Histo_Energy_custom_gt_10[3][3];
//plot for the detector faces
TH1F *Det_Face;
TH3F *DetFace;

Int_t low_ranges[3],up_ranges[3];
//Flux and power parameters Full range //indices [particle type][energy ranges 0->0.1->10->1000] 
Double_t flux[3][3],power[3][3];
//indices[z range][energy range][particle type]
Double_t flux_range[3][3][3],power_range[3][3][3];
float tot_events;
TDirectory* gD;
string prename;

int SensVolume_v;
//8002 cylindircal det at the radius close to electronic hut
//8003 cyl det at the wall

void bookHisto();
void processTree(string tname);
void Init();
void WriteHisto(string fname);


int main(int argc,char** argv) {

  if(argc < 3){
    cout<<"Usage: anaRad [input file name] [output file name] [list of sensative detectors #]"<<endl;
    cout<<" for example: build/anaRad o_HAPPEX2_1e6.root HAPPEX2_1e6 10008 10009 2001 2002 8002 8003"<<endl;
    cout<<"   Output files will be of the type: output/anaRad_OutputFileName.root"<<endl;
    return 1;
  }
  string ifnm=argv[1]; 
  string ofnm=argv[2];
  TFile *rf=new TFile(Form("output/anaRad_%s.root",ofnm.c_str()),"RECREATE");
  rf->Close();

  map <int,string> SensNames;
  SensNames[8002] ="HallD2";
  SensNames[8003] ="HallD3";  
  SensNames[10008]="HRShut";
  SensNames[10009]="Septum";
  SensNames[2001] ="Lpower";
  SensNames[2002] ="Rpower";

  for(int i=3;i<argc;i++){
    TString _sensVol(argv[i]);
    SensVolume_v=_sensVol.Atoi();
    cout<<"~~~~ Sensative volue set to :" <<SensVolume_v<<endl;
    cout<<endl;

    prename=Form("SV%d",SensVolume_v);
    if( SensNames.find(SensVolume_v)!=SensNames.end() )
      prename=SensNames[SensVolume_v];

    bookHisto();
    Init();
    processTree(ifnm);
    WriteHisto(Form("output/anaRad_%s.root",ofnm.c_str()));
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
      Histo_Energy_lt_10[i]         ->Reset();
      Histo_Energy_gt_10[i]         ->Reset();
    }
    
    for(int j=0;j<3;j++){
      flux[i][j]=0.0;
      power[i][j]=0.0;
      
      if(Histo_vert_z_full[i]){
      	Histo_vert_z[i][j]             ->Reset();
	Histo_vert_z_weighted[i][j]    ->Reset();
	Histo_Energy_custom_lt_10[i][j]->Reset();
	Histo_Energy_custom_gt_10[i][j]->Reset();

      }
      
      for(int k=0;k<3;k++){
	flux_range[i][j][k]=0.0;
	power_range[i][j][k]=0.0;
      }   
    }
  }

  if(Det_Face)  Det_Face ->Reset();  
  if(DetFace)   DetFace  ->Reset();  
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

  Det_Face = new TH1F("Det_Face","Detector Face Hit",8,-0.5,7.5);
  
  int knownDet[4]={10008,10009,2001,2002};
  float ranges[5][3][2]={
    {{3500,5500},{-1000,1000},{1000,2000}},//10008
    {{-1750,-750},{500,1500},{1000,2000}},//10009	   
    {{8100,9100},{-500,500},{-15110,-14110}},//2001  
    {{-4750,-3750},{-500,500},{-15110,-14110}},//2002
    {{-5000,9000},{-1000,2000},{-16000,4000}}
  };

  int rn=4;
  for(int i=0;i<4;i++)
    if(SensVolume_v==knownDet[i])
      rn=i;
  
  DetFace = new TH3F("DetFace","Detector Hits",
		     100,ranges[rn][0][0],ranges[rn][0][1],
		     100,ranges[rn][1][0],ranges[rn][1][1],
		     100,ranges[rn][2][0],ranges[rn][2][1]);
}

void PrintInfo(){
 
  TString oName[3]={"Photons  ","Electrons","Neutrons"};
  TString eName[3]={"  0<E<0.1","0.1<E<10"," 10<E<1000"};
  TString zName[3]={"-2600<z<-110"," -110<z<135","  135<z<3400"};
  cout<<" ~Printing:"<<endl;
  ofstream fout(Form("output/o_%s_powerFlux.dat",prename.c_str()),std::ofstream::out);
  
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

void processTree(string tname){

  Float_t type, volume, event;
  Float_t Energy;
  Float_t x_0,y_0,z_0,xd,yd,zd;
  
  TChain *t = new TChain("geant");
  t->Add(tname.c_str());
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("volume",&volume);
  t->SetBranchAddress("x",&xd);  
  t->SetBranchAddress("y",&yd);  
  t->SetBranchAddress("z",&zd);  
  t->SetBranchAddress("x0",&x_0);
  t->SetBranchAddress("y0",&y_0);
  t->SetBranchAddress("z0",&z_0);
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("event",&event);

  if ( SensVolume_v==2001 || SensVolume_v==2002 || SensVolume_v==10008 || SensVolume_v==10009){
    t->SetBranchAddress("Edeposit",&Energy); //because these are made from Kryptonite
  }else if( SensVolume_v==8003  || SensVolume_v==8004  || SensVolume_v==8005 ||
	    SensVolume_v==10001 || SensVolume_v==10002 || SensVolume_v==10003||
	    SensVolume_v==10004 ) {
    t->SetBranchAddress("kineE",&Energy); //because these are vacuum
  }

  
  Double_t hit_radius_min = 46.038; //cm inner radius of the beam pipe 45.72 cm and outer radius of the beam pipe 46.038 cm
  
  if ( SensVolume_v==8004 || SensVolume_v==8005 || SensVolume_v==10001 || SensVolume_v==10002 || SensVolume_v==10003 || SensVolume_v==10004 )
    hit_radius_min = 0.; //this is for looking at results along the beamline

  Double_t hit_radius;
  int partType[6]={1,1,-1,-1,0,2}; //electron,positron,N/A,N/A,gamma,neutron
  
  Int_t nentries = (Int_t)t->GetEntries();
  for (int i=0; i<nentries ; i++) {
    t->GetEntry(i);

    hit_radius = sqrt(pow(xd/10,2)+pow(yd/10,2));
     
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
    
    if  ( volume==SensVolume_v  && z_0 > -26000  && type<=5){
      DetFace->Fill(xd,yd,zd);
      if (SensVolume_v == 2001){
	if ( (xd>8100 && xd<9100) && (yd>-500 && yd<500) && zd==-14110 ) Det_Face->Fill(1);
	else if ( (xd>8100 && xd<9100) && (yd>-500 && yd<500) && zd==-15110 ) Det_Face->Fill(2);
	else if ( xd==9100 && (yd>-500 && yd<500) && (zd>-15110 && zd<-14110) ) Det_Face->Fill(3);
	else if ( xd==8100 && (yd>-500 && yd<500) && (zd>-15110 && zd<-14110) ) Det_Face->Fill(4);
	else if ( (xd>8100 && xd<9100) && yd==500 && (zd>-15110 && zd<-14110) ) Det_Face->Fill(5);
	else if ( (xd>8100 && xd<9100) && yd==-500 && (zd>-15110 && zd<-14110) ) Det_Face->Fill(6);	
      }
      if (SensVolume_v == 2002){
	if ( (xd>-4750 && xd<-3750) && (yd>-500 && yd<500) && zd==-14110 ) Det_Face->Fill(1);
	else if ( (xd>-4750 && xd<-3750) && (yd>-500 && yd<500) && zd==-15110 ) Det_Face->Fill(2);
	else if ( xd==-3750 && (yd>-500 && yd<500) && (zd>-15110 && zd<-14110) ) Det_Face->Fill(3);
	else if ( xd==-4750 && (yd>-500 && yd<500) && (zd>-15110 && zd<-14110) ) Det_Face->Fill(4);
	else if ( (xd>-4750 && xd<-3750) && yd==500 && (zd>-15110 && zd<-14110) ) Det_Face->Fill(5);
	else if ( (xd>-4750 && xd<-3750) && yd==-500 && (zd>-15110 && zd<-14110) ) Det_Face->Fill(6);	
      }
      if (SensVolume_v == 10008){
	if ( (xd>3500 && xd<5500) && (yd>-1000 && yd<1000) && zd==21000 ) Det_Face->Fill(1);
	else if ( (xd>3500 && xd<5500) && (yd>-1000 && yd<1000) && zd==17000 ) Det_Face->Fill(2);
	else if ( xd==5500 && (yd>-1000 && yd<1000) && (zd>17000 && zd<21000) ) Det_Face->Fill(3);
	else if ( xd==3500 && (yd>-1000 && yd<1000) && (zd>17000 && zd<21000) ) Det_Face->Fill(4);
	else if ( (xd>3500 && xd<5500) && yd==1000 && (zd>17000 && zd<21000) ) Det_Face->Fill(5);
	else if ( (xd>3500 && xd<5500) && yd==-1000 && (zd>17000 && zd<21000) ) Det_Face->Fill(6);	
      }
      if (SensVolume_v == 10009){
	if ( (xd>-1750 && xd<-750) && (yd>500 && yd<1500) && zd==2000 ) Det_Face->Fill(1);
	else if ( (xd>-1750 && xd<-750) && (yd>500 && yd<1500) && zd==1000 ) Det_Face->Fill(2);
	else if ( xd==-750 && (yd>500 && yd<1500) && (zd>1000 && zd<2000) ) Det_Face->Fill(3);
	else if ( xd==-1750 && (yd>500 && yd<1500) && (zd>1000 && zd<2000) ) Det_Face->Fill(4);
	else if ( (xd>-1750 && xd<-750) && yd==1500 && (zd>1000 && zd<2000) ) Det_Face->Fill(5);
	else if ( (xd>-1750 && xd<-750) && yd==500 && (zd>1000 && zd<2000) ) Det_Face->Fill(6);	
      }
    }
       
    if(i % 1000000 == 1 ) cout<<" processed: "<<tname.c_str()<<" "<<i<<endl;
  }
  
  PrintInfo();
  delete t;
}

void writeEachHisto(TH1* a){
  a -> SetName(Form("%s_%s",prename.c_str(),a->GetName()));
  a -> SetTitle(Form("%s %s",prename.c_str(),a->GetTitle()));
  a -> SetDirectory(gD);
  a -> Write();
}

void WriteHisto(string fname){
  
  TFile *fout=TFile::Open(fname.c_str(),"UPDATE");
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

  writeEachHisto(Det_Face);
  writeEachHisto(DetFace);
  
  fout->Close();
  delete fout;
}

