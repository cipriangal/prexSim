void donutEnergy(){
  //TFile *fin=TFile::Open("../prexSim3/prexSim/o_crex_1e6_p2dump.root","READ");
  //TFile *fin=TFile::Open("o_PREX_p2beamPipeV2_1e6.root","READ");
  //TFile *fin=TFile::Open("o_H2target20cm_10GeV_1e7.root","READ");
  TFile *fin=TFile::Open("o_H2target20cm4cmR_1cmRight_10GeV_1e7.root","READ");
  TNtuple *t=(TNtuple*)fin->Get("geant");
  TH1D *hb=new TH1D("hb","Before donut",100,0,300);
  TH1D *ha=new TH1D("ha","After  donut",100,0,300);
  TH1D *hd=new TH1D("hd","Diff   donut",100,0,300);
  TH1D *hc=new TH1D("hc","CDF of energy absorption",100,0,300);

  t->Project("hb","sqrt(x*x+y*y)","kineE*(volume==10006)");
  t->Project("ha","sqrt(x*x+y*y)","kineE*(volume==10007)");
  // t->Project("hb","sqrt(x*x+y*y)","kineE*(volume==10006 && kineE>0.5)");
  // t->Project("ha","sqrt(x*x+y*y)","kineE*(volume==10007 && kineE>0.5)");
  hd->Add(hb,ha,1,-1);

  for(int i=1;i<=100;i++)
    hc->SetBinContent(i,hc->GetBinContent(i-1)+hd->GetBinContent(i));
  TCanvas *c1=new TCanvas("c1","c1",1600,600);
  c1->Divide(4);
  c1->cd(1);
  ha->SetLineColor(2);
  hb->SetTitle(";r[mm];Energy for 1e7 events [MeV]");
  hb->DrawCopy();
  ha->DrawCopy("same");
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  c1->cd(2);
  hd->SetTitle("Energy difference before and after apperture;r[mm];Energy for 1e7 events [MeV]");
  hd->DrawCopy();
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  c1->cd(3);
  hc->SetTitle("CDF;r [mm]; deposited power [W/uA]");
  hc->Scale(1/1e7);
  hc->SetStats(0);
  hc->DrawCopy();

  // TH1D *hc2=(TH1D*)hc->Clone("hc2");
  // hc2->GetYaxis()->SetTitle("percent of deposite energy");
  // hc2->Scale(1/hc2->GetBinContent(100));
  // hc2->Draw("Y+ SAME");
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  c1->cd(4);
  drawFrontFace(hb);
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  fin->Close();
}

void drawFrontFace(TH1D *h){
  TH1D *hf=(TH1D*)h->Clone("hf");
  int nb=hf->GetXaxis()->GetNbins();
  double pi=acos(-1);
  double ev=1e6;

  cout<<"Rmin[mm]\tRmax[mm]\tarea[mm2]\tEnergy for 1e6 events[MeV]\tPower[W/uA/mm2]\n";
  for(int i=1;i<=nb;i++){
    double xl = hf->GetBinLowEdge(i);
    double xh = xl + hf->GetBinWidth(i);
    double area = (pi*pow(xh,2)) -(pi*pow(xl,2));
    double power = hf->GetBinContent(i)/(ev*area);
    cout<<xl<<"\t"<<xh<<"\t"<<area<<"\t"<<hf->GetBinContent(i)<<"\t"<<power<<endl;
    hf->SetBinContent(i,  power );
  }
  hf->SetTitle(";r [mm]; Power [W/uA/mm2]");
  hf->SetStats(0);
  hf->DrawCopy();
}
