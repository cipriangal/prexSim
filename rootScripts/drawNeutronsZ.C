void drawNeutronsZ(){
  string fnms[6]={
    "o_CREX5_6mmTgt_woDefaultShield_skyshineTest_5e7.root",
    "o_CREX5_6mmTgt_fullColl_3e7.root",
    "o_CREX5_6mmTgt_krypAboveTgt_fullColl_3e7.root",
    "o_CREX5_6mmTgt_withDefaultShield_skyshineTest_3e7.root",
    "o_CREX5_6mmTgt_withExtendedShield_skyshineTest_3e7.root",
    "o_CREX5_6mmTgt_withExtendedShield8cmThick_skyshineTest_3e7.root"
  };
  
  double scale=5./3;
  int cols[6]={1,2,4,6,7,8};
  TCanvas *c1=new TCanvas("c1","c1");
  gStyle->SetOptStat(0);
  for(int i=0;i<3;i++){
    TFile *fin=TFile::Open(fnms[i].c_str(),"READ");
    TTree *t=(TTree*)fin->Get("geant");
    TH1D *h=new TH1D("h","h",20,-23000,23000);

    int nv=t->Project("h","z","Edeposit*(PDGid==2112 && volume==10013 && Edeposit>30)");
    //int nv=t->Project("h","z","(PDGid==2112 && volume==10013 && Edeposit>30)");
    h->SetLineColor(cols[i]);
    cout<<i<<" "<<nv<<" "<<h->GetEntries()<<endl;
    cout<<h->Integral()<<endl;
    c1->cd();
    if(i==0){
      h->GetYaxis()->SetRangeUser(1,32);
      h->DrawCopy();
    }else{
      h->Scale(scale);
      cout<<h->Integral()<<endl;
      h->DrawCopy("same");
    }
    delete h;
    fin->Close();
  }
  
}

void compare2(){
  string fnm1="o_CREX5_6mmTgt_woDefaultShield_skyshineTest_5e7.root";
  string fnm2="o_CREX5_6mmTgt_withDefaultShield_skyshineTest_3e7.root";

  TFile *f1=TFile::Open(fnm1.c_str(),"READ");
  TFile *f2=TFile::Open(fnm2.c_str(),"READ");
  TTree *t1=(TTree*)f1->Get("geant");
  TTree *t2=(TTree*)f2->Get("geant");

  double scale = 5/3.;
  TCanvas *c1=new TCanvas("c1","c1");
  c1->Divide(2,2);
  TH1D *h1=new TH1D("h1","h1",20,-23000,23000);
  TH1D *h2=new TH1D("h2","h2",20,-23000,23000);
  TH1D *hd=new TH1D("hd","hd",20,-23000,23000);

  int n1=t1->Project("h1","z","Edeposit*(PDGid==2112 && volume==10013 && Edeposit>30)");
  int n2=t2->Project("h2","z","Edeposit*(PDGid==2112 && volume==10013 && Edeposit>30)");
  cout<<n1<<" "<<n2<<" "<<h1->GetEntries()<<" "<<h2->GetEntries()<<endl;
  h2->Scale(scale);

  gStyle->SetOptStat(0);
  c1->cd(1);
  h1->DrawCopy();
  c1->cd(2);
  h2->DrawCopy();
  c1->cd(3);
  h1->SetLineColor(2);
  h1->DrawCopy();
  h2->DrawCopy("same");
  c1->cd(4);
  hd->Divide(h1,h2);
  hd->DrawCopy();
  
  f2->Close();
  f1->Close();
}
