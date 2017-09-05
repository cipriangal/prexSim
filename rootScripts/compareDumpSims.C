void compareDumpSims(){
  gStyle->SetOptStat(0);
  // string config="beamPipeVacuum";
  //string config="noDonut";
  string config="smallerCollMidDonut";
  //string config="smallerCollPipe2Donut";
  //string config="smallerCollNeck";
  //string config="largerHall";
  //string config="newHRS";
  //string config="newHRS_2ftIronDumpShield";
  TCanvas *c1=new TCanvas("c1","c1",1400,600);
  c1->Divide(2);
  c1->cd(1);
  compareOne("prexII",config);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  c1->cd(2);
  compareOne("crex5",config);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);

}

void compareOne(string sim, string conf){
  TFile *fc=TFile::Open(Form("../output/dump_%s_current/file_hallRad.root",sim.c_str()));
  TFile *f2=TFile::Open(Form("../output/dump_%s_%s/file_hallRad.root",sim.c_str(),conf.c_str()));

  TH1D *hc=(TH1D*)fc->Get("hSummary_neilLogX");
  TH1D *h2=(TH1D*)f2->Get("hSummary_neilLogX");

  hc->SetLineWidth(4);
  hc->DrawCopy();
  h2->SetLineColor(2);
  h2->SetLineWidth(2);
  h2->DrawCopy("same");
  f2->Close();
  fc->Close();
}
