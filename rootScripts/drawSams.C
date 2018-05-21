void drawSams(){
  string fnm= "./v1_9_sams6inDonut_redTree.root";
  string opdf = "v1_9_sams6inDonut_redTree.pdf";
  TFile *fin = TFile::Open(fnm.c_str(),"READ");

  TCanvas *c1=new TCanvas("c1","c1",1400,800);
  c1->Print(Form("%s[",opdf.c_str()),"pdf");

  TTree *t=(TTree*)fin->Get("t");

  c1->Divide(2);
  gStyle->SetOptStat("eMRoui");

  c1->cd(1);
  t->Draw("E","volID==2211 && trackID==1 && pdgID==11 && parentID==0");
  gPad->SetLogy(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c1->cd(2);
  t->Draw("E","volID==2212 && trackID==1 && pdgID==11 && parentID==0");
  gPad->SetLogy(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c1->Print(opdf.c_str(),"pdf");

  c1->cd(1);
  t->Draw("sqrt(x*x+y*y)","volID==2211 && trackID==1 && pdgID==11 && parentID==0");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c1->cd(2);
  t->Draw("sqrt(x*x+y*y)","volID==2212 && trackID==1 && pdgID==11 && parentID==0");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c1->Print(opdf.c_str(),"pdf");

  c1->cd(1);
  t->Draw("sqrt(x*x+y*y)","E*(volID==2211 && trackID==1 && pdgID==11 && parentID==0)");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c1->cd(2);
  t->Draw("sqrt(x*x+y*y)","E*(volID==2212 && trackID==1 && pdgID==11 && parentID==0)");
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  c1->Print(opdf.c_str(),"pdf");

  c1->Print(Form("%s]",opdf.c_str()),"pdf");
  fin->Close();
}
