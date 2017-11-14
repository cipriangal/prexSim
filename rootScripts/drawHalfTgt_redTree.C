void drawHalfTgt_redTree(){
  TFile *fin_half=TFile::Open("o_anaHalfTgt_c5_newHRS_newDet.root","READ");
  TFile *fin_full=TFile::Open("o_anaHalfTgt_p2_newHRS_newDet.root","READ");
  // TFile *fin_full=TFile::Open("o_anaHalfTgt_fullLead_4inDonut.root","READ");
  // TFile *fin_half=TFile::Open("o_anaHalfTgt_halfLead_4inDonut.root","READ");

  gStyle->SetOptStat(0);
  TCanvas *c1=new TCanvas("c1","c1");
  drawHeatMap((TH2D*)fin_full->Get("h1"),(TH2D*)fin_half->Get("h1"),c1);

  TCanvas *c2=new TCanvas("c2","c2");
  drawHeatMap((TH2D*)fin_full->Get("h2"),(TH2D*)fin_half->Get("h2"),c2);

  int nH[6]={0,1,2,3,6,7};
  TCanvas *c3=new TCanvas("c3","c3");
  c3->Divide(3,2);
  for(int i=0;i<6;i++){
    c3->cd(i+1);
    string histNm=Form("h3_%d",nH[i]);
    drawRdist((TH1D*)fin_full->Get(histNm.c_str()),(TH1D*)fin_half->Get(histNm.c_str()));
    c3->Modified();
    c3->Update();
  }

  TCanvas *c5=new TCanvas("c5","c5");
  c5->Divide(3,2);
  for(int i=0;i<6;i++){
    c5->cd(i+1);
    string histNm=Form("h3_%d",nH[i]);
    drawRdist(removeArea((TH1D*)fin_full->Get(histNm.c_str())),
	      removeArea((TH1D*)fin_half->Get(histNm.c_str())));
    c5->Modified();
    c5->Update();
  }

  // neil weighting doesn't show much more
  // TCanvas *c4=new TCanvas("c4","c4");
  // c4->Divide(3,2);
  // for(int i=0;i<6;i++){
  //   c4->cd(i+1);
  //   string histNm=Form("h4_%d",nH[i]);
  //   drawRdist((TH1D*)fin_full->Get(histNm.c_str()),(TH1D*)fin_half->Get(histNm.c_str()));
  //   c4->Modified();
  //   c4->Update();
  // }
}

void drawRdist(TH1D *f1,TH1D *h1){

  f1->SetLineColor(2);
  h1->SetLineColor(4);
  h1->SetTitle(Form("%s R:full B:half",h1->GetTitle()));
  h1->DrawCopy();
  f1->DrawCopy("same");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  TPaveText *txt=new TPaveText(0.4,0.7,0.9,0.9,"NDC");
  //txt->AddText(Form("Entries(R/B): %f | %f",f1->GetEntries(),h1->GetEntries()));
  txt->AddText(Form("Mean(R/B): %4.2f | %4.2f",f1->GetMean(),h1->GetMean()));
  txt->AddText(Form("RMS(R/B): %4.2f | %4.2f",f1->GetRMS(),h1->GetRMS()));
  txt->Draw();
}

void drawHeatMap(TH2D *f1,TH2D *h1,TCanvas *can){
  can->Divide(2);
  can->cd(1);

  f1->SetTitle(Form("%s PREX2 current",f1->GetTitle()));
  //f1->SetTitle(Form("%s fullTgt 4inDonut",f1->GetTitle()));
  f1->DrawCopy("colz");
  gPad->SetLogz(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  printIntegrals(f1);

  can->cd(2);
  h1->SetTitle(Form("%s CREX5 current",h1->GetTitle()));
  //h1->SetTitle(Form("%s halfTgt 4inDonut",h1->GetTitle()));
  h1->DrawCopy("colz");
  gPad->SetLogz(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  printIntegrals(h1);
}

TH1D *removeArea(TH1D *h){
  TH1D *hc=(TH1D*)h->Clone(Form("%s_a",h->GetName()));
  int bn=hc->GetXaxis()->GetNbins();
  const double pi = acos(-1);
  for(int i=1;i<=bn;i++){
    double rl = hc->GetXaxis()->GetBinLowEdge(i);
    double rh = hc->GetXaxis()->GetBinUpEdge(i);
    hc->SetBinContent(i,h->GetBinContent(i)/(pi*rh*rh - pi*rl*rl));
  }
  return hc;
}

void printIntegrals(TH2D *h){
  double xrange[6]={25000,26400,29150,29300,31400,36000};
  cout<<"Integrals:\t"<<h->GetTitle()<<endl;
  for(int i=0;i<5;i++){
    int bl=h->GetXaxis()->FindBin(xrange[i]);
    int bh=h->GetXaxis()->FindBin(xrange[i+1]);
    cout<<"\t"<<xrange[i]<<"\tto\t"<<xrange[i+1]<<"\t("<<bl<<","<<bh<<")\t"
	<<h->Integral(bl,bh)<<endl;
  }
}
