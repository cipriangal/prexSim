using namespace std;

void drawPlots(){
  gROOT->Reset();

  Int_t linetype=1;

  TCanvas *c1= new TCanvas("c1","Moller plots",800,800);
  c1->Divide(1,3);
  c1->cd(1);

  TFile *_file0 = TFile::Open("output/test_upgrade/mollersim_nowgt_up_ee_geo_10_sim_9.root");
  linetype=1;
  Eprime_vert_0_4->SetLineStyle(linetype);
  Eprime_vert_0_4->SetLineColor(7);
  Eprime_vert_0_4->Draw();
  Eprime_vert_0_3->SetLineStyle(linetype);
  Eprime_vert_0_3->Draw("same");
  Eprime_vert_0_2->SetLineStyle(linetype);
  Eprime_vert_0_2->Draw("same");
  Eprime_vert_0_1->SetLineStyle(linetype);
  Eprime_vert_0_1->Draw("same");
  Eprime_vert_0_0->SetLineStyle(linetype);
  Eprime_vert_0_0->Draw("same");
  

  TFile *_file0 = TFile::Open("output/test_upgrade/mollersim_nowgt_devel_ee_geo_10_sim_9.root");
  linetype=2;
  Eprime_vert_0_4->SetLineStyle(linetype);
  Eprime_vert_0_4->SetLineColor(7);
  Eprime_vert_0_4->Draw("same");
  Eprime_vert_0_3->SetLineStyle(linetype);
  Eprime_vert_0_3->Draw("same");
  Eprime_vert_0_2->SetLineStyle(linetype);
  Eprime_vert_0_2->Draw("same");
  Eprime_vert_0_1->SetLineStyle(linetype);
  Eprime_vert_0_1->Draw("same");
  Eprime_vert_0_0->SetLineStyle(linetype);
  Eprime_vert_0_0->Draw("same");
  
  c1->cd(2);

  TFile *_file0 = TFile::Open("output/test_upgrade/mollersim_up_ee_geo_10_sim_9.root");
  Int_t linetype =1;
  Eprime_vert_0_4->SetLineStyle(linetype);
  Eprime_vert_0_4->SetLineColor(1);
  Eprime_vert_0_4->Draw();

  TFile *_file0 = TFile::Open("output/test_upgrade/mollersim_devel_ee_geo_10_sim_9.root");

  linetype =2;
  Eprime_vert_0_4->SetLineStyle(linetype);
  Eprime_vert_0_4->SetLineColor(1);
  Eprime_vert_0_4->Draw("same");

  c1->cd(3);
  Float_t total_up_rate=0;
  Float_t total_devel_rate=0;
  Float_t rate_diff=0;

  Float_t mean_up=0;
  Float_t mean_devel=0;
  Float_t rate_diff=0;

  TFile *_file0 = TFile::Open("output/test_upgrade/mollersim_up_ee_geo_10_sim_9.root");
  Int_t linetype =1;

  total_up_rate=Eprime_vert_0_4->Integral();
  mean_up=Eprime_vert_0_3->GetMean();

  TH1F *hist_new=(TH1F*)Eprime_vert_0_4->Clone();
  hist_new->SetName("hist_new");

  TFile *_file1 = TFile::Open("output/test_upgrade/mollersim_devel_ee_geo_10_sim_9.root");

  linetype =2;
  
  total_devel_rate=Eprime_vert_0_4->Integral();
  mean_devel=Eprime_vert_0_3->GetMean();
  Eprime_vert_0_4->Scale(-1);
  //Eprime_vert_0_4->Draw("");

  hist_new->Add(Eprime_vert_0_4);
  hist_new->SetLineColor(1);
  hist_new->Draw("");

  rate_diff=hist_new->Integral();

  cout<<total_up_rate<<" - "<<total_devel_rate<<" = "<<rate_diff<<endl;

  cout<<mean_up<<" - "<<mean_devel<<endl;

};
