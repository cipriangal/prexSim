#include <iostream>
#include <string>
#include "MollerGlobalMagnetField.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"
#include "TGraph.h"

using namespace std;

int main(int argc, char **argv){

  if(argc!=4){
    cout<<"run as:\n\tbuild/drawBfield <configName -- can be either prex1, prex2, crex, prex2JayNoQ1shield, crexJayNoQ1shield, prex2UpdatedSeptumShield_allOn, crexUpdatedSeptumShield_allOn, crex_2PipeSeptumShield_allOn, test> <nSteps> <with Q1 fringe?: can be 0 or 1>\n";
    return 1;
  }
  string config=argv[1];
  const int nSteps=atoi(argv[2]);
  const int withQ1Fringe=atoi(argv[3]);
  double offset=69.91;

  MollerGlobalMagnetField bField;
  bField.SetConfiguration(config);
  string foutNm;
  if(withQ1Fringe){
    bField.SetQ1fringeValue(true);
    foutNm = Form("o_drawBfield_%s_withQ1fringe.root",config.c_str());
  }else{
    foutNm = Form("o_drawBfield_%s_woutQ1fringe.root",config.c_str());
  }



  TFile *fout=new TFile(foutNm.c_str(),"RECREATE");
  TGraph *bx=new TGraph();
  TGraph *by=new TGraph();
  TGraph *bz=new TGraph();
  double point[4]={-1*CLHEP::cm,0,0,0};
  double deltaY=0.001*CLHEP::cm;
  for(int i=0;i<nSteps;i++){
    if(i%100==1) cout<<"at step\t"<<i<<endl;
    double z = -220 + 440.*i/nSteps;//local septum position

    point[2]=(z+offset)*CLHEP::cm;//global position needed for calculation
    point[1]=0;
    double p1[3]={0,0,0};
    bField.GetFieldValue(point,p1);

    point[2]=(z+offset)*CLHEP::cm;//global position needed for calculation
    point[1]=deltaY;
    double p2[3]={0,0,0};
    bField.GetFieldValue(point,p2);

    bx->SetPoint(i,z,(p2[0]/CLHEP::gauss - p1[0]/CLHEP::gauss)/(deltaY/CLHEP::cm));
    by->SetPoint(i,z,(p2[1]/CLHEP::gauss - p1[1]/CLHEP::gauss)/(deltaY/CLHEP::cm));
    bz->SetPoint(i,z,(p2[2]/CLHEP::gauss - p1[2]/CLHEP::gauss)/(deltaY/CLHEP::cm));
    //cout<<"\t"<<z<<"\t"<<(p2[0]/CLHEP::gauss - p1[0]/CLHEP::gauss)/(deltaY/CLHEP::cm)<<endl;
  }
  bx->SetName("xField");
  bx->SetTitle(Form("dBx/dy for %s configuration at(-1cm,0,z); z [cm]; gauss/cm",config.c_str()));
  bx->Write();
  by->SetName("yField");
  by->SetTitle(Form("dBy/dy for %s configuration at(-1cm,0,z); z [cm]; gauss/cm",config.c_str()));
  by->Write();
  bz->SetName("zField");
  bz->SetTitle(Form("dBz/dy for %s configuration at(-1cm,0,z); z [cm]; gauss/cm",config.c_str()));
  bz->Write();

  fout->Close();

  return 0;
}
