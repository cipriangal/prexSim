TCanvas *c1=new TCanvas("c1","c1",1800,600);
TFile *fin;
string onm="y_drawFace_Happex2_1e6.pdf";

void drawFace(){
  c1->Print(Form("%s[",onm.c_str()),"pdf");

  string hnm[5]={"HRShut","Septum","Lpower","Rpower","HallDe"};
  fin=TFile::Open("../output/anaRad_HAPPEX2_1e6.root","READ");

  for(int i=0;i<5;i++){
    string hn=Form("%s_DetFace",hnm[i].c_str());
    cout<<hn<<endl;
    if(fin->GetListOfKeys()->Contains(hn.c_str()))
      drawOne(hn);
  }
      
  fin->Close();
  c1->Print(Form("%s]",onm.c_str()),"pdf");
}

void drawOne(string hnm){
  c1->Clear();
  c1->Divide(3);
  gStyle->SetOptStat("moue");
  TH3F *h=(TH3F*)fin->Get(hnm.c_str());
  cout<<hnm<<" "<<h->GetEntries()<<endl;
  TH2F *h1=h->Project3D("xy");
  TH2F *h2=h->Project3D("xz");
  TH2F *h3=h->Project3D("yz");
  c1->cd(1);
  h1->DrawCopy("colz");
  c1->cd(2);
  h2->DrawCopy("colz");
  c1->cd(3);
  h3->DrawCopy("colz");

  c1->Print(onm.c_str(),"pdf");
}
