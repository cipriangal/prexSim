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
#include <TFile.h>
#include <TDirectory.h>
#include "radDamage.hh"

using namespace std;

//log axis histograms
void niceLogBins(TH1*);
//energy/neil/mrem
//photons/electron/neutrons
vector<vector<vector<TH1D*> > > hTotal, hAvg, valAvg;
vector<vector<vector<vector<int> > > > intAvg;
void UpdateMeans();
int nAvg(-1);

TFile *fout;
vector<int> detNr;

void Initialize();
void WriteOutput();

long currentEv(0),prevEv(0),processedEv(0);
void ProcessOne(string);
radDamage radDmg;

int main(int argc, char **argv){
  if(argc<4){
    cout<<"Usage: build/hallRad <inputFile name: either rootfile or list of rootfiles> <nr of events to average over> <list of detector IDs>\n"
	<<"\tfor example: build/hallRad o_SimName.root 5000 10013 10008 10009\n";
    return 1;
  }
  string finNm=argv[1];
  nAvg = atoi(argv[2]);
  cout<<"looking at file: "<<finNm
      <<"\n\twith an average of "<<nAvg<<" and for detectors:"<<endl;
  for(int i=3;i<argc;i++){
    detNr.push_back(atoi(argv[i]));
    cout<<"\t\t"<<detNr[i-3]<<endl;
  }

  string foutNm = Form("%s_hallRad.root",finNm.substr(0,finNm.find(".")).c_str());
  fout=new TFile(foutNm.c_str(),"RECREATE");

  Initialize();
  if ( finNm.find(".root") < finNm.size() ){
    ProcessOne(finNm);
  }else{
    ifstream ifile(finNm.c_str());
    string data;
    while(ifile>>data){
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
  Float_t x0,y0,z0,xd,yd,zd;
  Float_t pdgID;
  TNtuple *t = (TNtuple*)fin->Get("geant");
  t->SetBranchAddress("type",&type);
  t->SetBranchAddress("volume",&volume);
  t->SetBranchAddress("x",&xd);
  t->SetBranchAddress("y",&yd);
  t->SetBranchAddress("z",&zd);
  t->SetBranchAddress("x0",&x0);
  t->SetBranchAddress("y0",&y0);
  t->SetBranchAddress("z0",&z0);
  t->SetBranchAddress("ev_num",&evNr);
  t->SetBranchAddress("PDGid",&pdgID);
  t->SetBranchAddress("Edeposit",&Edeposit);
  t->SetBranchAddress("kineE",&kinE);

  long nEntries= t->GetEntries();
  float currentProc=1,procStep=10;
  int nDet=detNr.size();
  for(long i=0;i<nEntries;i++){
    t->GetEntry(i);
    if( float(i+1)/nEntries*100 > currentProc){
      cout<<"at event\t"<<i<<"\t"<< float(i+1)/nEntries*100<<" %"<<endl;
      currentProc+=procStep;
    }
    currentEv += evNr - prevEv;
    prevEv = evNr;
    if( currentEv > nAvg ){
      currentEv=currentEv - nAvg;
      UpdateMeans();
    }

    int nHist(-1);
    for(int id=0;id<nDet;id++)
      if(volume==detNr[id]){
	nHist=id;
	break;
      }
    if(nHist==-1) continue;

    int nPart(-1);
    if( abs(pdgID) == 11 ) nPart=1;
    else if( pdgID == 2112 ) nPart=2;
    else if( pdgID == 22 ) nPart=0;
    else continue;

    //electrons directly from the gun
    if(z0== -17720) continue;

    double energy(-1);
    if( volume >=10008 && volume <= 10013 ) //Kryptonite detectors
      energy = Edeposit;
    else //vacuum detectors
      energy = kinE;
    //cout<<nHist<<" "<<nPart<<" "<<volume<<endl;
    hTotal[nHist][nPart][0]->Fill(energy);
    valAvg[nHist][nPart][0]->Fill(energy);

    double val(-1);
    val = radDmg.getNEIL(pdgID,energy,0);
    if(val!=-999){
      hTotal[nHist][nPart][1]->Fill(energy,val);
      valAvg[nHist][nPart][1]->Fill(energy,val);
    }

    val = radDmg.getMREM(pdgID,energy,0);
    if(val!=-999){
      hTotal[nHist][nPart][2]->Fill(energy,val);
      valAvg[nHist][nPart][2]->Fill(energy,val);
    }
  }

  processedEv += ceil(prevEv/1000.)*1000;
  prevEv = 0;
  fin->Close();
  delete fin;
}

void UpdateMeans(){
  int nDet=detNr.size();
  for(int id=0;id<nDet;id++){
    for(int ip=0;ip<3;ip++){
      for(int idmg=0;idmg<3;idmg++){
	int nbins= hAvg[id][ip][idmg]->GetXaxis()->GetNbins();
	for(int ib=1;ib<=nbins;ib++){
	  double val = valAvg[id][ip][idmg]->GetBinContent(ib)/nAvg;
	  if(val > 0){
	    intAvg[id][ip][idmg][ib]++;
	    double currentMean = hAvg[id][ip][idmg]->GetBinContent(ib);
	    double currentVar  = hAvg[id][ip][idmg]->GetBinError(ib);

	    double delta   = val - currentMean;
	    double newMean = currentMean + delta/intAvg[id][ip][idmg][ib];
	    double delta2  = val - newMean;
	    double newVar  = currentVar + delta*delta2;

	    hAvg[id][ip][idmg]->SetBinContent(ib,newMean);
	    if(intAvg[id][ip][idmg][ib]>=2)
	      hAvg[id][ip][idmg]->SetBinError(ib,newVar);
	  }
	  valAvg[id][ip][idmg]->SetBinContent(ib,0);	  
	}
      }
    }
  }
}

void Initialize(){
  string hPnm[3]={"g","e","n"};
  string type[3]={"ener","neil","mRem"};

  int nDet=detNr.size();
  for(int id=0;id<nDet;id++){
    vector<vector<TH1D*> > dt1,da1,dv1;
    intAvg.push_back(vector<vector<vector<int> > >(3, vector<vector<int> >(3, vector<int>(100))));
    for(int ip=0;ip<3;ip++){
      vector<TH1D*> dt2,da2,dv2;
      for(int idmg=0;idmg<3;idmg++){
	TH1D *h=new TH1D(Form("ht_%d_%s_%s",detNr[id],hPnm[ip].c_str(),type[idmg].c_str()),
			 Form("Total hits for det %d| part: %s| %s; energy [MeV]",detNr[id],hPnm[ip].c_str(),type[idmg].c_str()),
			 100,-4,4);
	niceLogBins(h);
	dt2.push_back(h);

	TH1D *a=new TH1D(Form("ha_%d_%s_%s",detNr[id],hPnm[ip].c_str(),type[idmg].c_str()),
			 Form("Hits/(%d ev) hits for det %d| part: %s| %s; energy [MeV]",nAvg,detNr[id],hPnm[ip].c_str(),type[idmg].c_str()),
			 100,-4,4);
	niceLogBins(a);
	da2.push_back(a);

	//dummy histograms
	TH1D *v=new TH1D(Form("hv_%d_%s_%s",detNr[id],hPnm[ip].c_str(),type[idmg].c_str()),
			 Form("Hits/(%d ev) hits for det %d| part: %s| %s; energy [MeV]",nAvg,detNr[id],hPnm[ip].c_str(),type[idmg].c_str()),
			 100,-4,4);
	niceLogBins(v);
	dv2.push_back(v);
      }
      dt1.push_back(dt2);
      da1.push_back(da2);
      dv1.push_back(dv2);
    }
    hTotal.push_back(dt1);
    hAvg.push_back(da1);
    valAvg.push_back(dv1);
  }
}

void WriteOutput(){
  fout->cd();
  int nDet=detNr.size();
  for(int id=0;id<nDet;id++){
    fout->cd();
    fout->mkdir(Form("Det_%d",detNr[id]));
    fout->cd(Form("Det_%d",detNr[id]));
    for(int ip=0;ip<3;ip++){
      for(int idmg=0;idmg<3;idmg++){
	hTotal[id][ip][idmg]->Write();
	int nbins = hAvg[id][ip][idmg]->GetXaxis()->GetNbins();
	for(int ib=1;ib<=nbins;ib++)
	  hAvg[id][ip][idmg]->SetBinError(ib, sqrt(hAvg[id][ip][idmg]->GetBinError(ib)));
	hAvg[id][ip][idmg]->Write();
      }
    }
  }
  fout->Close();
}

void niceLogBins(TH1*h)
{
   TAxis *axis = h->GetXaxis();
   int bins = axis->GetNbins();

   double from = axis->GetXmin();
   double to = axis->GetXmax();
   double width = (to - from) / bins;
   double *new_bins = new double[bins + 1];

   for (int i = 0; i <= bins; i++) {
     new_bins[i] = pow(10, from + i * width);
   }
   axis->Set(bins, new_bins);
   delete new_bins;
}
