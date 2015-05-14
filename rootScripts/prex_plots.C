using namespace std;

void drawPlots(){
  gROOT->Reset();

  Int_t linetype=1;

  TCanvas *c1= new TCanvas("c1","Moller plots",800,800);
  c1->Divide(1,3);
  c1->cd(1);

  TFile *_file0 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_dump_coll.root");
  //  linetype=1;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(3);
  geant->Draw("type","","");

  TFile *_file1 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtargonly_5.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(1);
  geant->Draw("type","","same");

  TFile *_file2 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_coll_no_dump.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(2);
  geant->Draw("type","","same");

  TFile *_file3 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_w_dump.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(4);
  geant->Draw("type","","same");
   
  c1->cd(2);
  TFile *_file0 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_dump_coll.root");
  //  linetype=1;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(3);
  geant->Draw("kineE","(type==5)*(1/1000000)","");

  TFile *_file1 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtargonly_5.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(1);
  geant->Draw("kineE","(type==5)*(1/1000000)","same");

  TFile *_file2 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_coll_no_dump.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(2);
  geant->Draw("kineE","(type==5)*(1/1000000)","same");

  TFile *_file3 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_w_dump.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(4);
  geant->Draw("kineE","(type==5)*(1/1000000)","same");

  c1->cd(3);
  TFile *_file0 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_dump_coll.root");
  //  linetype=1;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(3);
  geant->Draw("kineE","(type==5&&kineE<30&&kineE>1)*(1/1000000)","");

  TFile *_file1 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtargonly_5.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(1);
  geant->Draw("kineE","(type==5&&kineE<30&&kineE>1)*(1/1000000)","same");

  TFile *_file2 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_coll_no_dump.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(2);
  geant->Draw("kineE","(type==5&&kineE<30&&kineE>1)*(1/1000000)","same");

  TFile *_file3 = TFile::Open("~/scratch/ROOTfiles/prex_1e6_QGSC_BERT_HP_5mmtgt_w_dump.root");
  //  linetype=2;
  //  geant->SetLineStyle(linetype);
  geant->SetLineColor(4);
  geant->Draw("kineE","(type==5&&kineE<30&&kineE>1)*(1/1000000)","same");

  /*
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
  */
};
