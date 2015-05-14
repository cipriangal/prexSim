

void getDetRad() {



  const Int_t NUM_RANGES = 11;
  const Int_t NUM_DETS = 5;

  Double_t zTarg[NUM_DETS] = {14010,15510,17260,19510,21260};
  //Double_t zTarg[NUM_DETS] = {17260,20010,21010,21510,22760};



  TCanvas *c1 = new TCanvas();
  //TCanvas *c2 = new TCanvas();
    
  c1->Divide(2,1);
  //c2->Divide(3,2);
    
  Double_t rad[NUM_DETS][NUM_RANGES], rad_rms[NUM_DETS][NUM_RANGES],
    low[NUM_DETS][NUM_RANGES], high[NUM_DETS][NUM_RANGES],
    num[NUM_DETS][NUM_RANGES];
    
  TMultiGraph *mg = new TMultiGraph();

  for (int j=0; j<NUM_DETS; j++) {

    TGraphErrors *gr = new TGraphErrors(NUM_RANGES);

    for (int i=0; i<NUM_RANGES; i++) {
      
      low[j][i] = 2750 + i*250;
      high[j][i] = low[j][i] + 250;
      
      TString cut = "ion&&z==";
      cut+=zTarg[j];
      cut+="&&kineE>";
      cut+=low[j][i];
      cut+="&&kineE<";
      cut+=high[j][i];
      
      if (i<6) {
	//c1->cd(i+1);
	
	geant->Draw("r",cut,"goff");
	
	rad[j][i]=htemp->GetMean();
	rad_rms[j][i]=htemp->GetRMS();
	num[j][i]=htemp->GetEntries();

	gr->SetPoint(i,low[j][i],num[j][i]);
      }
      else {
	//c2->cd(i+1-6);
	
	geant->Draw("r",cut,"goff");
	
	rad[j][i]=htemp->GetMean();
	rad_rms[j][i]=htemp->GetRMS();
	num[j][i]=htemp->GetEntries();

	gr->SetPoint(i,low[j][i],num[j][i]);
      }

      mg->Add(gr);
      
    }
  }

  
  c1->cd(1);
  mg->Draw("apc*");


  TMultiGraph *mg1 = new TMultiGraph();
  
  for (int i=0; i<NUM_RANGES; i++) {
    
    TGraphErrors *gr1 = new TGraphErrors(NUM_DETS);
    

    for (int j=0; j<NUM_DETS; j++) {
      

      gr1->SetPoint(j,zTarg[j],rad[j][i]);
      gr1->SetPointError(j,0,rad_rms[j][i]);

      gr1->SetMarkerStyle(20+i);
      gr1->SetMarkerColor(i+1);
      gr1->SetLineColor(i+1);


    }
    
    mg1->Add(gr1);

  }
  
  c1->cd(2);
  mg1->Draw("apc");

  
  for (int j=0; j<NUM_DETS; j++) {
    
    cout << "\nDet " << (j+1) << " at z=" << zTarg[j] << "\n" << endl;

    for (int i=0; i<NUM_RANGES; i++) {
      
      cout << "N = " << num[j][i] << " For kineE0=[" << low[j][i] << "," << high[j][i] << "]   <R>=\t" << rad[j][i]/10 << "\t\t+-\t" << rad_rms[j][i]/10 << "\tcm." << endl;
      
    }
  }

}
