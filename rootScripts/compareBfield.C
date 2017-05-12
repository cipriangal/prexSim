void compareBfield(){
  TFile *fp1=TFile::Open("o_drawBfield_prex1.root","READ");
  TFile *fp2=TFile::Open("o_drawBfield_prex2.root","READ");
  TFile *fc1=TFile::Open("o_drawBfield_crex.root","READ");

  TGraph *gp1=(TGraph*)fp1->Get("xField");
  TGraph *gp2=(TGraph*)fp2->Get("xField");
  TGraph *gc1=(TGraph*)fc1->Get("xField");

  gp1->SetLineColor(42);
  gp1->SetLineWidth(2);
  gp2->SetLineColor(4);
  gp2->SetLineWidth(2);
  gc1->SetLineColor(1);
  gc1->SetLineWidth(2);
  gp1->Draw("AL");
  gp2->Draw("L");
  gc1->Draw("L");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  fp1->Close();
  fp2->Close();
  fc1->Close();
}
