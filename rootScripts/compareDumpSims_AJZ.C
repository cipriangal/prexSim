void compareOne(string, string, string, int);
void printHEneutrons(string, string, string, int);

void compareDumpSims(string sim, string config1, string config2, int n_events){
  gStyle->SetOptStat(0);
  
  TCanvas *c1=new TCanvas("c1","c1",800,600);
  compareOne(sim, config1, config2, n_events);
  printHEneutrons(sim, config1, config2, n_events);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  //c1->cd(2);
  //compareOne("crex5", config1, config2, 200);
  //printHEneutrons("crex5", config1);
  //gPad->SetGridx(1);
  //gPad->SetGridy(1);
  //gPad->SetLogy(1);

}

void compareOne(string sim, string conf1, string conf2, int n_events){
  TFile *f1=TFile::Open(Form("~/farmOut/%s_%s_%dkEv/%s_%s_hallRad.root", sim.c_str(), conf1.c_str(), n_events, sim.c_str(), conf1.c_str()));
  //TFile *fc=TFile::Open(Form("../output/dump_%s_newHRS_newDet/file_hallRad.root",sim.c_str()));
  TFile *f2=TFile::Open(Form("~/farmOut/%s_%s_%dkEv/%s_%s_hallRad.root", sim.c_str(), conf2.c_str(), n_events, sim.c_str(), conf2.c_str()));

  TH1D *h1=(TH1D*)f1->Get("hSummary_neilLogX");
  TH1D *h2=(TH1D*)f2->Get("hSummary_neilLogX");

  h1->SetLineWidth(4);
  h1->DrawCopy();
  h2->SetLineColor(2);
  h2->SetLineWidth(2);
  h2->DrawCopy("same");
  f2->Close();
  f1->Close();
}

void printHEneutrons(string sim, string conf1, string conf2, int n_events){
  cout<<endl<<endl<<"Print HE neutrons"<<endl;
  TFile *fc=TFile::Open(Form("~/farmOut/%s_%s_%dkEv/%s_%s_hallRad.root", sim.c_str(), conf1.c_str(), n_events, sim.c_str(), conf1.c_str()));
  TFile *f2=TFile::Open(Form("~/farmOut/%s_%s_%dkEv/%s_%s_hallRad.root", sim.c_str(), conf2.c_str(), n_events, sim.c_str(), conf2.c_str()));

  TH1D *hc=(TH1D*)fc->Get("Det_1006/ha_1006_n_enerLinX");
  TH1D *h2=(TH1D*)f2->Get("Det_1006/ha_1006_n_enerLinX");

  int nb1=hc->GetXaxis()->FindBin(10);
  int nb2=hc->GetXaxis()->FindBin(1000);
  double nc=hc->Integral(nb1,nb2);

  nb1=h2->GetXaxis()->FindBin(10);
  nb2=h2->GetXaxis()->FindBin(1000);
  double n2=h2->Integral(nb1,nb2);

  cout<<nc<<"\t"<<n2<<"\t"<<n2/nc<<endl;
}
