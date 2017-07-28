void compareBeamPipe(){
  TFile *f1=TFile::Open("o_PREX_p1beamPipe_1e6.root","READ");
  TFile *f2=TFile::Open("o_PREX_p2beamPipe_1e6.root","READ");
  TNtuple *t1=(TNtuple*)f1->Get("geant");
  TNtuple *t2=(TNtuple*)f2->Get("geant");
  gStyle->SetOptStat("eMR");
  TCanvas *can[8];
  int dets[]={10001,10002,10003,10004,10005};
  double scales[]={50000,50000,50000,50000,50000};
  for(int i=0;i<5;i++){
    can[i]=getOne(t1,t2,dets[i],scales[i]);
  }
  f1->Close();
  f2->Close();
}

TCanvas *getOne(TNtuple *t1, TNtuple *t2, int detID,double maxScale){
  TCanvas *c1=new TCanvas(Form("c1_%d",detID),Form("c1_%d",detID),1300,600);
  c1->Divide(2);
  c1->cd(1);
  t1->Draw("x:y>>h1",Form("volume==%d && kineE>1",detID),"colz");
  h1->GetZaxis()->SetRangeUser(1,maxScale);
  h1->DrawCopy("colz");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogz(1);

  c1->cd(2);
  t2->Draw("x:y>>h2",Form("volume==%d && kineE>1",detID),"colz");
  h2->GetZaxis()->SetRangeUser(1,maxScale);
  h2->DrawCopy("colz");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogz(1);

  delete h1;
  delete h2;
  return c1;
}
