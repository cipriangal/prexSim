

void g4analyze() {


  const Int_t NUM_VOLS = 7;
  const Int_t NUM_Z = 6;
  
  Double_t vol_num[NUM_VOLS] = {31,41,42,43,44,71,1};
  
  Double_t z_mother[NUM_Z] = {4745,5000,6340,9080,10420,14000};
  
  
  
  TCanvas *c1 = new TCanvas("washers_part1","",0);
  c1->Divide(3,3);
  TCanvas *c2 = new TCanvas("washers_part2","",0);
  c2->Divide(2,3);
  
  for (Int_t i=0 ; i<NUM_VOLS ; i++) {
    
    TString vol = "volume==";
    vol+=vol_num[i];
    vol+="&&ion";
    vol+="&&kineE0>2750";
    
    TString htitle = "Exit of volume #";
    htitle+=vol_num[i];
    TString hname = "xy";
    hname+=vol_num[i];
    
    TString draw = "x:y>>xy";
    draw+=vol_num[i];
    
    TH2F *xy = new TH2F(hname,htitle,1000,-200,200,1000,-200,200);
    
    c1->cd(i+1);
    xy->SetStats(0);
    xy->GetXaxis()->SetLabelSize(0.055);
    xy->GetYaxis()->SetLabelSize(0.055);
    geant->Draw(draw,vol);
    
  }
  
  
  for (Int_t i=0 ; i<NUM_Z ; i++) {
    
    TString cut = "z==";
    cut+=z_mother[i];
    cut+="&&ion";
    cut+="&&kineE0>2750";
    
    TString htitle = "Entrance of Volume at z=";
    htitle+=z_mother[i];
    TString hname = "xy";
    hname+=z_mother[i];
    
    TString draw = "x:y>>xy";
    draw+=z_mother[i];
    
    TH2F *xy = new TH2F(hname,htitle,1000,-200,200,1000,-200,200);
    
    c2->cd(i+1);
    xy->SetStats(0);
    xy->GetXaxis()->SetLabelSize(0.055);
    xy->GetYaxis()->SetLabelSize(0.055);
    geant->Draw(draw,cut);
    
  }
  
  
  /*TCanvas *c1 = new TCanvas("averages","X and Y Averaging",1);
    
  
  const Int_t NUM_VOLS = 7;
  Double_t vol_num[NUM_VOLS] = {31,41,42,43,44,71,1};
  
  
  
  
  Double_t xsum[NUM_VOLS], xnum[NUM_VOLS], xavg[NUM_VOLS], 
  ysum[NUM_VOLS], ynum[NUM_VOLS], yavg[NUM_VOLS],
  zloc[NUM_VOLS];
  
  for (Int_t i=0; i<NUM_VOLS; i++) {
  
  TString vol = "volume==";
  vol+=vol_num[i];

        geant->Draw("z",vol,"goff");

        Double_t *z = geant->GetV1();
	
	zloc[i] = *(z + 0);


        geant->Draw("x:y",vol,"goff");

        Int_t num_vals = geant->GetSelectedRows();
        const Int_t K = num_vals;
        Double_t *x = geant->GetV1();
        Double_t *y = geant->GetV2();
        Double_t xval[K],yval[K];


	for (Int_t k=0; k < K; k++) {
	    xval[k] = *(x + k);
	    yval[k] = *(y + k);
	}

	for (Int_t j=0; j<K; j++) {
	    if ( abs(xval[j])<10.0 && abs(yval[j])>30.0 && abs(yval[j])<140.0) {
		ysum[i]+=abs(yval[j]);
		ynum[i]++;
	    }
	    if ( abs(yval[j])<10.0 && abs(xval[j])>30.0 && abs(xval[j])<140.0) {
		xsum[i]+=abs(xval[j]);
		xnum[i]++;
	    }

        }

	xavg[i] = xsum[i] / xnum[i];
	yavg[i] = ysum[i] / ynum[i];


    }


    TGraph *gr1 = new TGraph(NUM_VOLS, zloc, xavg);
    gr1->SetMarkerColor(4);
    gr1->SetLineColor(4);

    TGraph *gr2 = new TGraph(NUM_VOLS, zloc, yavg);
    gr2->SetMarkerColor(2);
    gr2->SetLineColor(2);


    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gr1);
    mg->Add(gr2);

    mg->Draw("APC*");
    mg->SetTitle("Average Values of X and Y for GEANT Simulation");
    mg->GetXaxis()->SetTitle("Z");
    mg->GetYaxis()->SetTitle("X and Y averages");
    mg->GetXaxis()->CenterTitle();
    mg->GetYaxis()->CenterTitle();

    TLegend *leg = new TLegend(0.75,0.75,0.89,0.89);
    leg->AddEntry(gr1,"X","p");
    leg->AddEntry(gr2,"Y","p");
    leg->Draw();*/


}
