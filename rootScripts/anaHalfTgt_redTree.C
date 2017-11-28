void anaHalfTgt_redTree(){

  // anaHalfTgt_redTreeOne("../output/dump_crex5_newHRS_newDet/file_redTree.root");
  // anaHalfTgt_redTreeOne("../output/dump_prexII_fullLead_4inDonut/file_redTree.root");
  //anaHalfTgt_redTreeOne("../output/dump_prexII_halfLead/file_redTree.root");
  //anaHalfTgt_redTreeOne("/home/ciprian/hdd2/prexOutput/dump_prexII_newHRS_newDet/file_redTree.root");
  anaHalfTgt_redTreeOne("../output/dump_prexII_0p75Tgt/file5_redTree.root");
}
void anaHalfTgt_redTreeOne(string fnm){
  cout<<"opening "<<fnm<<endl;
  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TTree *t=(TTree*)fin->Get("t");
  string ofnm=Form("%s_anaRedTTree.root",fnm.substr(0,fnm.find_last_of(".")).c_str());
  cout<<"\twriting to "<<ofnm<<endl;
  TFile *fout=new TFile(ofnm.c_str(),"RECREATE");

  TH2D *h1=new TH2D("h1","heatmap for volID=1001;z [mm];x [mm]"     ,200,25000,36000,200,-2000,2000);
  TH2D *h2=new TH2D("h2","NEIL heatmap for volID=1001;z [mm];x [mm]",200,25000,36000,200,-2000,2000);
  TH1D *h3[8],*h4[8];
  for(int i=0;i<8;i++){
    h3[i]=new TH1D(Form("h3_%d",i),Form("radial distribution for det %d; r[cm]",2201+i),600,0,300);
    h4[i]=new TH1D(Form("h4_%d",i),Form("radial distribution for e^#pm and #gamma det %d (E>100MeV); r[cm]",2201+i),600,0,300);
  }
  TH1D *h5=new TH1D("h5","NEIL weighted distribution for volID=1001;origin z[mm]",
		    200,25000,36000);
  TH1D *h6=new TH1D("h6","NEIL weighted distribution for volID=1001 e^#pm;origin z[mm]",
		    200,25000,36000);
  TH1D *h7=new TH1D("h7","NEIL weighted distribution for volID=1001 neutrons;origin z[mm]",
		    200,25000,36000);

  float x0,z0,x,y,E,neil;
  int pdgID,volID,tID,pID;
  t->SetBranchAddress("pdgID",&pdgID);
  t->SetBranchAddress("trackID",&tID);
  t->SetBranchAddress("parentID",&pID);
  t->SetBranchAddress("volID",&volID);
  t->SetBranchAddress("x0",&x0);
  t->SetBranchAddress("z0",&z0);
  t->SetBranchAddress("x",&x);
  t->SetBranchAddress("y",&y);
  t->SetBranchAddress("E",&E);
  t->SetBranchAddress("neil",&neil);

  long nEntries=t->GetEntries();
  float currentProc=1,procStep=10;
  for(long k=0;k<nEntries;k++){
    t->GetEntry(k);
    if( float(k+1)/nEntries*100 > currentProc){
      cout<<"at tree entry\t"<<k<<"\t"<< float(k+1)/nEntries*100<<" %"<<endl;
      currentProc+=procStep;
    }
    if(E<=0.1) continue;

    if(volID==1001){
      h1->Fill(z0,x0);
      if(neil>0){
	h2->Fill(z0,x0,neil);
	h5->Fill(z0,neil);
	if(abs(pdgID)==11)
	  h6->Fill(z0,neil);
	else if(pdgID==2112)
	  h7->Fill(z0,neil);
      }
    }else if(volID>2200 && volID<2209){
      int det = volID - 2201;
      if(tID==1 && pID==0 && pdgID==11)
	h3[det]->Fill(sqrt(x*x+y*y));
      else if(E>100 && (abs(pdgID)==11 || pdgID==22))
	h4[det]->Fill(sqrt(x*x+y*y));
    }
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
}
