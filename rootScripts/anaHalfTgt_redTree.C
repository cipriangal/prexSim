void anaHalfTgt_redTree(){

  anaHalfTgt_redTreeOne("../output/dump_crex5_newHRS_newDet/file_redTree.root");
  anaHalfTgt_redTreeOne("../output/dump_prexII_fullLead_4inDonut/file_redTree.root");
  anaHalfTgt_redTreeOne("../output/dump_prexII_halfLead/file_redTree.root");
  anaHalfTgt_redTreeOne("../output/dump_prexII_newHRS_newDet/file_redTree.root");
}
void anaHalfTgt_redTreeOne(string fnm){
  cout<<"opening "<<fnm<<endl;
  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TTree *t=(TTree*)fin->Get("t");
  string ofnm=Form("%s_anaRedTree.root",fnm.substr(0,fnm.find_last_of(".")).c_str());
  cout<<"\twriting to "<<ofnm<<endl;
  TFile *fout=new TFile(ofnm.c_str(),"RECREATE");

  TH2D *h1=new TH2D("h1","heatmap for volID=1001;z [mm];x [mm]"     ,200,25000,36000,200,-2000,2000);
  TH2D *h2=new TH2D("h2","NEIL heatmap for volID=1001;z [mm];x [mm]",200,25000,36000,200,-2000,2000);
  t->Project("h1","x0:z0","volID==1001 && E>0.1","");
  t->Project("h2","x0:z0","neil*(volID==1001 && E>0.1 && neil>0)");

  TH1D *h5=new TH1D("h5","NEIL weighted distribution for volID=1001;origin z[mm]",
		    200,25000,36000);
  t->Project("h5","z0","neil*(volID==1001 && E>0.1 && neil>0)");

  TH1D *h6=new TH1D("h6","NEIL weighted distribution for volID=1001 e^#pm;origin z[mm]",
		    200,25000,36000);
  t->Project("h6","z0","neil*(volID==1001 && E>0.1 && neil>0 && abs(pdgID)==11)");

  TH1D *h7=new TH1D("h7","NEIL weighted distribution for volID=1001 neutrons;origin z[mm]",
		    200,25000,36000);
  t->Project("h7","z0","neil*(volID==1001 && E>0.1 && neil>0 && pdgID==2112)");

  TH1D *h3[8];
  for(int i=0;i<8;i++){
    h3[i]=new TH1D(Form("h3_%d",i),Form("radial distribution for det %d; r[cm]",2201+i),600,0,300);
    t->Project(Form("h3_%d",i),"sqrt(x*x+y*y)",Form("volID==%d && trackID==1 && parentID==0 && pdgID==11",2201+i));
  }

  TH1D *h4[8];
  for(int i=0;i<8;i++){
    h4[i]=new TH1D(Form("h4_%d",i),Form("E weighted radial distribution for det %d; r[cm]",2201+i),600,0,300);
    t->Project(Form("h4_%d",i),"sqrt(x*x+y*y)",Form("E*(volID==%d && trackID==1 && parentID==0 && pdgID==11)",2201+i));
  }

  fout->cd();
  h1->Write();
  h2->Write();
  h5->Write();
  h6->Write();
  h7->Write();
  for(int i=0;i<8;i++){
    h3[i]->Write();
    h4[i]->Write();
  }
  fout->Close();
  // delete h1;
  // delete h2;
  // delete h5;
  // delete h6;
  // delete h7;
  // delete[] h3;
  // delete[] h4;
}
