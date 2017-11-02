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
float E, neil, mrem;
TTree *tout;
int nTotal(0);
vector<int> detNr;

void Initialize();
void WriteOutput();

long currentEv(0),prevEv(0),processedEv(0);
void ProcessOne(string);
radDamage radDmg;

int main(int argc, char **argv){
  if(argc<4){
    cout<<"Usage: build/redTree <inputFile name: either rootfile or list of rootfiles>  <limit number of events; 0==all> <list of detector IDs>\n"
        <<"\tfor example: build/redTree o_SimName.root 1001 1002 1003\n";
    return 1;
  }

  string finNm=argv[1];
  nTotal=atoi(argv[2]);
  cout<<"looking at file: "<<finNm<<" for "<<nTotal<<" events "
      <<"\n\tand for detectors:"<<endl;
  for(int i=3;i<argc;i++){
    detNr.push_back(atoi(argv[i]));
    cout<<"\t\t"<<detNr[i-3]<<endl;
  }

  string foutNm = Form("%s_redTree.root",finNm.substr(0,finNm.find(".")).c_str());
  fout=new TFile(foutNm.c_str(),"RECREATE");
  tout=new TTree("t","PREX sim reduced tree");

  Initialize();
  if ( finNm.find(".root") < finNm.size() ){
    if(processedEv<nTotal && nTotal>0)
      ProcessOne(finNm);
  }else{
    ifstream ifile(finNm.c_str());
    string data;
    while(ifile>>data){
      cout<<" processing: "<<data<<endl;
      if(processedEv<nTotal && nTotal>0)
	ProcessOne(data);
    }
  }
  cout<<"Processed a total of "<<processedEv<<endl;
  WriteOutput();
  cout<<"written"<<endl;
  return 0;
}

void ProcessOne(string fnm){
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

  long nEntries= t->GetEntries();
  float currentProc=1,procStep=10;
  //int tst=0;
  for(long i=0;i<nEntries;i++){
    //if( float(i+1)/nEntries*100 > 51) continue;
    if(currentEv>nTotal && nTotal>0) continue;
    t->GetEntry(i);
    if( float(i+1)/nEntries*100 > currentProc){
      cout<<"at tree entry\t"<<i<<"\t"<< float(i+1)/nEntries*100<<" %"<<endl;
      currentProc+=procStep;
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

    if(trackstatus==0)
      E=kinE;
    else
      E=Edeposit;

    if( E < 0.1 ) continue;

    nEv = processedEv + evNr;
    tID = track;
    pID = parent;
    pdgID = pdg;
    vID = volume;
    x = xd;
    y = yd;
    z = zd;
    xO = xd0;
    yO = yd0;
    zO = zd0;

    neil = radDmg.getNEIL(pdgID,E,0);
    mrem = radDmg.getMREM(pdgID,E,0);
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
  tout->Branch("E",&E,"E/F");
  tout->Branch("neil",&neil,"neil/F");
  tout->Branch("mrem",&mrem,"mrem/F");

}

void WriteOutput(){
  fout->cd();
  tout->Write();
  fout->Close();
}

