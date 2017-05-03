#include <iostream>
#include <string>
#include "MollerGlobalMagnetField.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include "TGraph.h"

using namespace std;

int main(int argc, char **argv){

  if(argc!=3){
    cout<<"run as:\n\tbuild/drawBfield <configName -- can be either prex or crex> <nSteps>\n";
    return 1;
  }
  string config=argv[1];
  const int nSteps=atoi(argv[2]);
  
  MollerGlobalMagnetField bField;
  bField.SetLowLimSeptumField(-74*CLHEP::cm);
  bField.SetHighLimSeptumField(74*CLHEP::cm);
  bField.SetScaleFactor(1);

  TFile *fout=new TFile(Form("o_drawBfield_%s.root",config.c_str()),"RECREATE");
  TGraph *bx=new TGraph();
  TGraph *by=new TGraph();
  TGraph *bz=new TGraph();
  double point[4]={-1*CLHEP::cm,0,0,0};    
  for(int i=0;i<nSteps;i++){
    if(i%100==1) cout<<"at step\t"<<i<<endl;
    double x = -110 + 69.91 + 220.*i/nSteps;
    point[2]=x*CLHEP::cm;
    double y[3]={0,0,0};
    bField.GetFieldValue(point,y);
    bx->SetPoint(i,x,y[0]);
    by->SetPoint(i,x,y[1]);
    bz->SetPoint(i,x,y[2]);
  }
  bx->SetName("xField");
  bx->SetTitle(Form("Bx for %s configuration at(-1cm,0,z); z [cm]",config.c_str()));
  bx->Write();
  by->SetName("yField");
  by->SetTitle(Form("By for %s configuration at(-1cm,0,z); z [cm]",config.c_str()));
  by->Write();
  bz->SetName("zField");
  bz->SetTitle(Form("Bz for %s configuration at(-1cm,0,z); z [cm]",config.c_str()));
  bz->Write();

  fout->Close();
  
  return 0;
}
