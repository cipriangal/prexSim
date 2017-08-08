const int nFl=11;
TCanvas *c1[nFl];
TPaveText *rat[6][nFl];
double ref[6];

void drawHRSheatMaps(){
  string file[nFl]={
    "../output/p2_1GeV_noQ1_dumpV3_5e6.root",
    "../output/p2_1GeV_noQ1_dumpV3_4inDonut_5e6.root",
    "../output/p2_1GeV_noQ1_P1dumpV2_5e6.root",
    "../output/p2_1GeV_noQ1_dumpV3_dumpShield_5e6.root",
    "../output/p2_1GeV_noQ1_dumpV3_dumpShieldPoly_5e6.root",
    "../output/c5_2GeV_noQ1_dumpV3_5e6.root",
    "../output/c5_2GeV_noQ1_dumpV3_4inDonut_5e6.root",
    "../output/c5_2GeV_noQ1_P1dumpV2_5e6.root",
    "../output/c5_2GeV_noQ1_dumpV3_dumpShield_5e6.root",
    "../output/c5_2GeV_noQ1_dumpV3_dumpShieldPoly_5e6.root",
    "../output/c5_2GeV_2PipeSeptum_noQ1_dumpV3_5e6.root"
  };
  for(int i=0;i<nFl;i++){
  //for(int i=0;i<2;i++){
    c1[i]=new TCanvas(Form("c%d",i),file[i].c_str(),800,1000);
    c1[i]->Divide(2,3);
    //drawOneHRSheatMaps(file[i],c1[i],i);
    drawNEILdmgOne(file[i],c1[i],i);
  }
}

void drawOneHRSheatMaps(string fnm,TCanvas *can, int nF){
  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TNtuple *t=(TNtuple*)fin->Get("geant");

  TH2D *h=new TH2D("h",";z[mm];x[mm]",100,25000,32000,100,-2000,2000);

  string em="(PDGid==11)";
  //string em="(PDGid==11 || PDGid==22)";
  string cuts[6]={Form("%s && Edeposit>0 && Edeposit<=0.1",em.c_str()),
                  "(PDGid==2112) && Edeposit>0 && Edeposit<=0.1",
		  Form("%s && Edeposit>0.1 && Edeposit<=10",em.c_str()),
                  "(PDGid==2112) && Edeposit>0.1 && Edeposit<=10",
		  Form("%s && Edeposit>10 ",em.c_str()),
                  "(PDGid==2112) && Edeposit>10"};
  gStyle->SetOptStat("eou");
  gStyle->SetStatX(0.8);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.2);

  for(int i=0;i<6;i++){
    h->SetTitle(cuts[i].c_str());
    t->Project("h","x0:z0",Form("volume==1001 && %s",cuts[i].c_str()));
    can->cd(i+1);

    h->DrawCopy("colz");
    double entries=h->Integral();
    rat[i][nF]=new TPaveText(0.2,0.8,0.3,0.9,"NDC");
    if(nF==0 || nF==5){
      ref[i]=entries;
    }else{
      rat[i][nF]->Clear();
      rat[i][nF]->AddText(Form("%4.2f",entries/ref[i]));
      rat[i][nF]->Draw();
    }
    gPad->SetLogz(1);

    can->Modified();
    can->Update();
  }
  delete h;
  fin->Close();
}

void drawNEILdmgOne(string fnm,TCanvas *can, int nF){
  string fnmHalRad = fnm.substr(0, fnm.length()-5) + "_hallRad.root";
  cout<<fnmHalRad<<endl;
  TFile *fin=TFile::Open(fnmHalRad.c_str(),"READ");
  gStyle->SetOptStat("i");
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.2);

  TH1D *he=(TH1D*)fin->Get("Det_1001/ha_1001_e_neilLinX");
  TH1D *hn=(TH1D*)fin->Get("Det_1001/ha_1001_n_neilLinX");

  int nC[6]={1,3,5,2,4,6};
  double lims[4]={0,0.1,10,2000};
  for(int i=0;i<3;i++){
    can->cd(nC[i]);
    he->GetXaxis()->SetRangeUser(lims[i],lims[i+1]);
    he->GetYaxis()->SetLabelSize(0.05);
    he->DrawCopy();

    can->cd(nC[i+3]);
    hn->GetXaxis()->SetRangeUser(lims[i],lims[i+1]);
    hn->GetYaxis()->SetLabelSize(0.05);
    hn->DrawCopy();

    can->Modified();
    can->Update();
  }
  fin->Close();
}
