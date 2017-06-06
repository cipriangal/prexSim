void powerInHall(string fnm){
  int nEv=5e5;
  cout<<"I think this simulation has "<<nEv<<" events! Do you agree?"<<endl;
  std::cin.ignore();

  TCanvas *c1=new TCanvas("c1","c1",1500,600);
  c1->Divide(3);
  gStyle->SetOptStat("eMRi");
  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TNtuple *t=(TNtuple*)fin->Get("geant");
  c1->cd(1);
  t->Draw("kineE>>h1","kineE*(volume==8004)");
  cout<<h1->Integral()/nEv<<" W/uA"<<endl;
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  c1->cd(2);
  t->Draw("kineE>>h2","kineE*(volume==8005)");
  cout<<h2->Integral()/nEv<<" W/uA"<<endl;
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  c1->cd(3);
  t->Draw("kineE>>h3","kineE*(volume==10004)");
  cout<<h3->Integral()/nEv<<" W/uA"<<endl;
  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLogy(1);
  fin->Close();
}
