#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

#include <TH1D.h>
#include <TH1.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>
#include "radDamage.hh"

using namespace std;

TFile *fout;
int nEv(0), tID, pID, pdgID, vID;
float x,y,z, px,py,pz;
float xO,yO,zO;
float kE, eD, neil, mrem;
TTree *tout;
int nTotalPerFile(0);
int cut_flag(0);
vector<int> detNr;

void Initialize();
void WriteOutput();

long currentEv(0), prevEv(0), processedEv(0);
void ProcessOne(string, long, int);
radDamage radDmg;

int main(int argc, char **argv){
  if(argc < 6){
    cout<<"Usage: build/redTree_AJZ <folder for event rootfiles> <outfile name> <num files to process> <num events per file to process>  <100 keV cut flag> <list of detector IDs>\n"
        <<"\tfor example: build/redTree_AJZ ~/farmOut/prexII_current_900kEv/ prexII_current 50 100000 1 1001 1002 1003\n";
    return 1;
  }
  
  nTotalPerFile = atoi(argv[4]);
  cut_flag = atoi(argv[5]);
  for(int i = 6; i < argc; i++){
    detNr.push_back(atoi(argv[i]));
    cout<<"\t\t"<<detNr[i-6]<<endl;
  }
  
  string dir    = argv[1];
  int nfiles    = atoi(argv[3]);
  string foutNm = Form("%s/%s.root", argv[1], argv[2]);
  fout          = new TFile(foutNm.c_str(),"RECREATE");
  tout          = new TTree("t","PREX sim reduced tree");

  Initialize();
  for(int i = 0; i < nfiles; i++){
    string infile_num = "";
    if(i < 10){infile_num = "0000" + to_string(i);}
    else if(i < 100){infile_num = "000" + to_string(i);}
    else{infile_num = "00" + to_string(i);}

    string infile_name = dir + "/" + infile_num + "/o_prexSim.root";
    cout<<"Processing file: "<<dir<<"/"<<infile_num<<endl;
    ProcessOne(infile_name, nTotalPerFile, cut_flag);
  }

  cout<<"Processed a total of "<<processedEv<<endl;
  WriteOutput();
  cout<<"written"<<endl;
  return 0;
}

void ProcessOne(string fnm, long ntpf, int keV_cut){
  TFile *fin=new TFile(fnm.c_str(),"READ");
  if(!fin->IsOpen()){
    cout<<"Problem: can't find file: "<<fnm<<endl;
    fin->Close();
    delete fin;
    return;
  }

  Float_t type, volume, evNr;
  Float_t Edeposit,kinE;
  Float_t xd,yd,zd,xd0,yd0,zd0;
  Float_t pxd,pyd,pzd;
  Float_t pdg,trackstatus,track,parent;
  TNtuple *t = (TNtuple*)fin->Get("geant");
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("volume",&volume);
  t->SetBranchAddress("x",&xd);
  t->SetBranchAddress("y",&yd);
  t->SetBranchAddress("z",&zd);
  t->SetBranchAddress("px",&pxd);
  t->SetBranchAddress("py",&pyd);
  t->SetBranchAddress("pz",&pzd);
  t->SetBranchAddress("x0",&xd0);
  t->SetBranchAddress("y0",&yd0);
  t->SetBranchAddress("z0",&zd0);
  t->SetBranchAddress("ev_num",&evNr);
  t->SetBranchAddress("PDGid",&pdg);
  t->SetBranchAddress("Edeposit",&Edeposit);
  t->SetBranchAddress("kineE",&kinE);
  t->SetBranchAddress("trackstatus",&trackstatus);
  t->SetBranchAddress("track",&track);
  t->SetBranchAddress("parent",&parent);

  long nEntries = t->GetEntries();
  long nComp    = 0;
  if(nEntries <= ntpf || ntpf == 0){nComp = nEntries;}
  else{nComp = ntpf;}

  float currentProc = 1, procStep = 10;
  //int tst=0;
  for(long i = 0; i < nComp; i++){
    //if( float(i+1)/nEntries*100 > 51) continue;
    t->GetEntry(i);
    if( float(i+1)/nComp*100 >= currentProc){
      cout<<"at tree entry\t"<<i<<"\t"<< float(i+1)/nEntries*100<<" %"<<endl;
      currentProc += procStep;
    }

    currentEv += evNr - prevEv;
    prevEv = evNr;

    //electrons directly from the gun
    //if(zO== -17720) continue;
    //if(zO>26000) continue;

    // cout<<volume<<"\t"<<zO<<"\t"<<trackstatus<<"\t"<<Edeposit<<"\t"<<kinE<<endl;
    // std::cin.ignore();
    if(find(detNr.begin(),detNr.end(),volume) == detNr.end()) continue;
    // cout<<volume<<"\tvol\t"<<zO<<"\t"<<trackstatus<<"\t"<<Edeposit<<"\t"<<kinE<<endl;
    // std::cin.ignore();

    //if(trackstatus==0)
    //  E = kinE;
    //else
    //  E = Edeposit;

    if( kinE < 0.1 && Edeposit < 0.1 && keV_cut) continue;

    nEv   = processedEv + evNr;
    tID   = track;
    pID   = parent;
    pdgID = pdg;
    vID   = volume;
    x     = xd;
    y     = yd;
    z     = zd;
    xO    = xd0;
    yO    = yd0;
    zO    = zd0;
    px    = pxd;
    py    = pyd;
    pz    = pzd;
    kE    = kinE;
    eD    = Edeposit;

    neil = radDmg.getNEIL(pdgID,kE+eD,0);
    mrem = radDmg.getMREM(pdgID,kE+eD,0);
    // cout<<tst<<endl;
    // tst++;
    // std::cin.ignore();
    tout->Fill();
  }

  processedEv += ceil(prevEv/1000.)*1000;
  prevEv = 0;
  fin->Close();
  delete fin;
}

void Initialize(){

  tout->Branch("nEv",&nEv,"nEv/I");
  tout->Branch("trackID",&tID,"trackID/I");
  tout->Branch("parentID",&pID,"parentID/I");
  tout->Branch("pdgID",&pdgID,"pdgID/I");
  tout->Branch("volID",&vID,"volID/I");
  tout->Branch("x",&x,"x/F");
  tout->Branch("y",&y,"y/F");
  tout->Branch("z",&z,"z/F");
  tout->Branch("x0",&xO,"x0/F");
  tout->Branch("y0",&yO,"y0/F");
  tout->Branch("z0",&zO,"z0/F");
  tout->Branch("px",&px,"px/F");
  tout->Branch("py",&py,"py/F");
  tout->Branch("pz",&pz,"pz/F");
  tout->Branch("kineE",&kE,"kE/F");
  tout->Branch("edep",&eD,"eD/F");
  tout->Branch("neil",&neil,"neil/F");
  tout->Branch("mrem",&mrem,"mrem/F");

}

void WriteOutput(){
  fout->cd();
  tout->Write();
  fout->Close();
}
