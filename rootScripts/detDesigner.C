

void detDesigner2() {



  const Int_t NUM_RANGES = 11;
  const Int_t NUM_DETS = 5;

  Double_t zTarg[NUM_DETS] = {14010,15510,17260,19510,21260};
  //Double_t zTarg[NUM_DETS] = {17260,20010,21010,21510,22760};

  Double_t rmin[NUM_DETS] = {85,100,110,125,150};
  Double_t rmax[NUM_DETS] = {100,120,130,145,170};


  Double_t rad[NUM_DETS][NUM_RANGES], rad_rms[NUM_DETS][NUM_RANGES],
    low[NUM_DETS][NUM_RANGES], high[NUM_DETS][NUM_RANGES],
    num[NUM_DETS][NUM_RANGES];
    


  TString cut = "ion&&volume==71&&";

  cut+="kineE>2750&&kineE<5500";


  geant->Draw("num",cut,"goff");

  Double_t *id = geant->GetV1();

  Int_t tot = htemp->GetEntries();
  const Int_t N = tot;

  cout << N << endl;

  Double_t index[N];


  for (int i=0; i<N; i++) {
    index[i]= *(id +i);
  }



  Double_t hit_list[N];



  for (int j=0; j<NUM_DETS; j++) {


    for (int i=0; i<NUM_RANGES; i++) {
      
      low[j][i] = 2750 + i*250;
      high[j][i] = low[j][i] + 250;
      
      TString cut;

      cut+="ion&&z==";
      cut+=zTarg[j];
      cut+="&&kineE>";
      cut+=low[j][i];
      cut+="&&kineE<";
      cut+=high[j][i];
      cut+="&&r>";
      cut+=rmin[j];
      cut+="&&r<";
      cut+=rmax[j];
      

      geant->Draw("r>>h1",cut,"goff");
      

      rad[j][i]=h1->GetMean();
      rad_rms[j][i]=h1->GetRMS();
      num[j][i]=h1->GetEntries();

      
      
    }
  }

  Double_t total;

  for (int j=0; j<NUM_DETS; j++) {
    for (int i=0; i<NUM_RANGES; i++) {
      total+=num[j][i];
    }
  }
  
  for (int j=0; j<NUM_DETS; j++) {
    
    cout << "\nDet " << (j+1) << " at z=" << zTarg[j] << "\n" << endl;

    for (int i=0; i<NUM_RANGES; i++) {
      
      cout << "N = " << num[j][i] << " For kineE0=[" << low[j][i] << "," << high[j][i] << "]   <R>=\t" << rad[j][i]/10 << "\t\t+-\t" << rad_rms[j][i]/10 << "\tcm." << endl;
      
    }
  }

  cout << total << endl;

}
