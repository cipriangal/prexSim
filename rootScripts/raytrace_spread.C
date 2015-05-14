

void raytrace_spread() {


  const Int_t NUM_RANGES = 11;

  const Double_t E_START = 2.75;
  const Double_t E_STEP = 0.25;

  TCanvas *c1 = new TCanvas();

  c1->Divide(4,3);

  
  for (int i=0; i<NUM_RANGES; i++) {

    c1->cd(i+1);

    raytraces(2.75+i*E_STEP,2.75+(i+1)*E_STEP);

  }


}

void raytraces(Double_t min, Double_t max) {

  TString cut = "ion&&z==11965&&";

  cut+="kineE>";
  cut+=min*1000;
  cut+="&&kineE<";
  cut+=max*1000;

  geant->Draw("theta0:kineE:num",cut,"goff");

  Double_t *id = geant->GetV3();
  Double_t *en = geant->GetV2();
  Double_t *th = geant->GetV1();

  Int_t tot = htemp->GetEntries();
  const Int_t N = tot;

  cout << N << endl;

  Double_t index[N];
  Double_t kineE[N];
  Double_t theta[N];

  Double_t e_min=5000000, e_max=-500000, id_min=5000000, id_max=-5000000;


  for (int i=0; i<N; i++) {
    index[i]= *(id +i);
    theta[i]= *(th +i);
    kineE[i]= *(en +i);

    if (kineE[i]>e_max) {e_max=kineE[i]; id_max=index[i];}
    if (kineE[i]<e_min) {e_min=kineE[i]; id_min=index[i];}
    
  }

  
  TString cut = "num==";
  cut+=id_max;
  
  geant->Draw("r:z",cut,"C");

  for (int i=1; i<N; i++) {
    
    TString cut = "num==";
    cut+=index[i];
    
    geant->Draw("r:z",cut,"Csame");

  }

  //htemp->GetXaxis()->SetRangeUser(-350,350);

  TString title = "E = {";
  
  title+=min;
  title+=", ";
  title+=max;
  title+="} GeV";

  htemp->SetTitle(title);
  htemp->GetYaxis()->SetRangeUser(0,350);
  htemp->GetXaxis()->SetRangeUser(0,25000);


}
