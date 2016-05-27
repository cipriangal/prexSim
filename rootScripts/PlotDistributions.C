void PlotDistributions(){
  //cut for particle origin anywhere in  entire target chamber region
  TString targetchcut = "z0>-1800&&z0<-800&&abs(x0)<2000&&abs(y0)<2000";
  TString collimatorcut = "z0>-800&&z0<100&&abs(x0)<1000&&abs(y0)<1000";
  TString lidcut = "volume==10013&&type==5&&Edeposit>10";

  //  TH1F *h1 = new TH1F("h1","my histogram",100,-3,3);

  TCanvas *c1=new TCanvas("c1","c1",0,0,700,700);
  c1->Divide(1,2);
  c1->cd(1);
  geant->Draw("Edeposit",Form("%s",lidcut.Data()));
  geant->Draw("Edeposit>>ht",Form("%s&&%s",targetchcut.Data(),lidcut.Data()),"same");
  ht->SetTitle("Neutron Distribution in Hall Lid,E>10MeV from target chamber region");
  ht->SetFillColor(2);
  ht->SetLineColor(2);
  //  ht->GetXaxis()->SetRange(0,1000);
  ht->Draw();
c1->cd(2);
geant->Draw("Edeposit",Form("%s",lidcut.Data()));
geant->Draw("Edeposit>>hc",Form("%s&&%s",collimatorcut.Data(),lidcut.Data),"same");
  hc->SetTitle("Neutron Distribution in Hall Lid,E>10MeV from collimator chamber region");
  hc->SetFillColor(1);
  hc->SetLineColor(1);
  hc->Draw("same");

  //scale hint1 to the pad coordinates
  // Float_t rightmax = 1.1*hint1->GetMaximum();
  //Float_t scale = gPad->GetUymax()/rightmax;
  //hint1->SetLineColor(kRed);
  //hint1->Scale(scale);
  //hint1->Draw("same")

  //  c1->Update()
}
