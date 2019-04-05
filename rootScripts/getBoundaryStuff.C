void getP1mW(){
  TFile *fin=TFile::Open("prex2_hallRad.root","READ");
  //TFile *fin=TFile::Open("happex2_hallRad.root","READ");
  //TFile *fin=TFile::Open("crex_hallRad.root","READ");
  //TFile *fin=TFile::Open("prex1_hallRad.root","READ");
  TH1D *h=(TH1D*)fin->Get("Det_1006/ha_1006_n_enerLogX");
  //auto c1=new TCanvas("c1","c1");
  //h->DrawCopy();
  cout<<"Looking from (bins 75 to 93)\t"<<h->GetBinLowEdge(75)<<"\tto\t"
      <<h->GetBinLowEdge(93)+h->GetBinWidth(93)<<endl;
  double hits(0);
  double power(0);
  for(int i=75;i<=93;i++){
    double e = h->GetBinCenter(i);
    double hit = h->GetBinContent(i);
    hits += hit;
    power += e*hit;
  }
  double perCoulomb = 1/1.6 *1e19;
  //double charge = 82; //p1 Coulombs 
  double charge = 178; //p2 Coulombs
  //double charge = 467; //c Coulombs
  //double charge = 87; //h2 Coulombs
  double kJfactor = 6.24e15; // 1kJ = 6.24e15 MeV
  cout<<"Hits per electron on target\t"<<hits<<endl;
  cout<<"Hits per uA\t"<<hits* perCoulomb*1e-6 <<endl;
  cout<<"Hits integrated \t"<<hits*perCoulomb*charge<<endl;
  cout<<"Energy(MeV) per electron on target\t"<<power<<endl;
  cout<<"Power(mW) per uA\t"<<power*perCoulomb*1e-6 / (kJfactor*1e-6)<<endl;
  cout<<"Energy(kJ) integrated \t"<<power*charge*perCoulomb/kJfactor<<endl;
  fin->Close();
}
