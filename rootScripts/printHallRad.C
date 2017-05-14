void printHallRad(string fnm){
  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TH1D *h1=(TH1D*)fin->Get("hSummary_neil");
  TH1D *h2=(TH1D*)fin->Get("hSummary_mRem");
  doOne(h1);
  doOne(h2);
  fin->Close();
}

void doOne(TH1D *h){
  int nb=h->GetXaxis()->GetNbins();
  cout<<endl<<endl<<h->GetTitle()<<endl;
  for(int i=2;i<=nb;i+=2)
    cout<<h->GetXaxis()->GetBinLabel(i)<<"\t"<<h->GetBinContent(i)<<"\t"<<h->GetBinError(i)<<endl;
}

