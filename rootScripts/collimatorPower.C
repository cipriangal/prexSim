void collimatorPower(){
  int nev=9e5;

  string fnm="/lustre/expphy/volatile/halla/parity/ciprian/farmOut/prexII_newHRS_newDet_900kEv_00000/o_prexSim.root";

  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TNtuple *t=(TNtuple*)fin->Get("geant");

  TH1D *h1=new TH1D("h1","US face;r[mm];Energy [MeV]",100,0,120);
  TH1D *h2=new TH1D("h2","DS face;r[mm];Energy [MeV]",100,0,120);
  TH1D *h3=new TH1D("h3","difference;r[mm];Energy [MeV]",100,0,120);
  gStyle->SetOptStat("eMiou");

  TCanvas *c1=new TCanvas("c1","c1",1200,400);
  c1->Divide(3);
  c1->cd(1);
  t->Draw("sqrt(x*x+y*y)>>h1","kineE*(volume==2101)");
  h1->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  c1->cd(2);
  t->Draw("sqrt(x*x+y*y)>>h2","kineE*(volume==2102)");
  h2->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  c1->cd(3);
  h3->Add(h1,h2,1,-1);
  h3->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  cout<<h3->Integral()/nev<<" W/uA"<<endl;
  fin->Close();
}
