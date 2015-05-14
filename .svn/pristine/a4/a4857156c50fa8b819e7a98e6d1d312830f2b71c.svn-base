

void detDesigner2() {



  const Int_t NUM_RANGES = 11;
  const Int_t NUM_DETS = 6;

  const Int_t E_START = 2750;
  const Int_t E_STEP = 250;

  Double_t zTarg[NUM_DETS] = {14010,15510,17260,19510,21260,25000};
  //Double_t zTarg[NUM_DETS] = {17260,20010,21010,21510,22760,25000};

  Double_t rmin[NUM_DETS] = { 77.5,100,115,135,160,20};
  Double_t rmax[NUM_DETS] = {107.5,120,132.5,155,200,250};


  Double_t rad[NUM_DETS][NUM_RANGES], rad_rms[NUM_DETS][NUM_RANGES],
    low[NUM_DETS][NUM_RANGES], high[NUM_DETS][NUM_RANGES],
    num[NUM_DETS][NUM_RANGES];
    

  TH1D *hist[NUM_DETS][NUM_RANGES];

  TH1D *dets[NUM_DETS];


  TString cut = "ion&&z==11965&&";

  cut+="kineE>";
  cut+=E_START;
  cut+="&&kineE<5500";


  geant->Draw("num",cut,"goff");

  Double_t *id = geant->GetV1();

  Int_t tot = htemp->GetEntries();
  const Int_t N = tot;

  cout << N << endl;

  Double_t index[N];

  //List of Moller particles that make it to the detectors
  for (int i=0; i<N; i++) {
    index[i]= *(id +i);
  }


  //This will be a list of particles that have already been seen.
  Double_t hit_list[N];

  Int_t count;


  TCanvas *c1 = new TCanvas();
  c1->Divide(3,2);

  for (int j=0; j<NUM_DETS; j++) {

    TString title = "det";

    title+=(j+1);

    dets[j] = new TH1D(title,"",100,E_START-E_STEP,5500+E_STEP);

    for (int i=0; i<NUM_RANGES; i++) {
      
      low[j][i] = E_START + i*E_STEP;
      high[j][i] = low[j][i] + E_STEP;
      
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


      Double_t radius[2000], particle[2000], energy[2000];



      geant->Draw("num>>h1",cut,"goff");

      Int_t tot = h1->GetEntries();

     
      Double_t *id = geant->GetV1();

      for (int k=0; k<tot; k++) {
	particle[k]= *(id + k);
      }
      
      geant->Draw("r>>h1",cut,"goff");
      
      Double_t *r = geant->GetV1();

      for (int k=0; k<tot; k++) {
	radius[k]= *(r + k);
      }

      
      geant->Draw("kineE>>h1",cut,"goff");
      
      Double_t *en = geant->GetV1();

      for (int k=0; k<tot; k++) {
	energy[k]= *(en + k);
      }


      hist[j][i] = new TH1D("hist","",100,rmin[j],rmax[j]);

      
      for (int k=0; k<tot; k++) {
	if (notInList(particle[k],hit_list,N)) {
	  hist[j][i]->Fill(radius[k]);
	  dets[j]->Fill(energy[k]);

	  hit_list[count]=particle[k];
	  count++;
	}
      }
	
	
      rad[j][i]=hist[j][i]->GetMean();
      rad_rms[j][i]=hist[j][i]->GetRMS();
      num[j][i]=hist[j][i]->GetEntries();

      hist[j][i]->Delete();
      
    }
    c1->cd(j+1);
    dets[j]->Draw();

    //dets[j]->Delete();
    
  }


  Double_t total;


  TCanvas *c2 = new TCanvas();
  c2->Divide(3,2);


  for (int j=0; j<NUM_DETS; j++) {

    TGraph *gr = new TGraph();

    for (int i=0; i<NUM_RANGES; i++) {
      total+=num[j][i];

      gr->SetPoint(i,low[j][i],num[j][i]);
      gr->SetMarkerStyle(20);

     

    }

    c2->cd(j+1);

    gr->Draw("apc");

    }

  
  
  for (int j=0; j<NUM_DETS; j++) {
    
    cout << "\nDet " << (j+1) << " at z=" << zTarg[j] << "\n" << endl;

    for (int i=0; i<NUM_RANGES; i++) {
      
      cout << "N = " << num[j][i] 
	   << " For kineE0=[" << low[j][i] 
	   << "," << high[j][i] 
	   << "]   <R>=\t" << rad[j][i]/10 
	   << "\t\t+-\t" << rad_rms[j][i]/10 << endl;
      
    }
  }

  cout << total << endl;

}


Int_t notInList(Double_t thisid, Double_t* index, Int_t N) {

  Int_t isHere = 0;

  for (int i=0; i<N; i++) {
    if (index[i]==thisid) isHere=1;
  }

  return isHere-1;
  
}
