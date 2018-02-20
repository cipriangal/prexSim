#include <map>
#include <string>

map <int,int> detArea;//cm2
void doOne(TH1D *h, double runV);
void doLid(TH1D *h,double runV);

int printHallRad(string fnm,string simType){

  //detArea[1001] = 240000;//Maduka HRS detector 6x200x200 cm2
  //detArea[1001] = 400000;//Old PREX area (designed by Rakitha)
  detArea[1001] = 987292;//Current maximal area detector
  detArea[1002] =  60000;
  detArea[1003] = 160000;
  detArea[1004] = 100000;
  detArea[1005] =  55000;
  detArea[1101] =  60000;
  detArea[1102] =  60000;
  //for old simulations
  detArea[10008] = 400000;
  detArea[10009] =  60000;
  detArea[10010] = 160000;
  detArea[10011] = 100000;
  detArea[10012] =  55000;
  detArea[10101] =  60000;
  detArea[10102] =  60000;

  double runFactor(-1);
  if(simType=="prex1")
    runFactor = 8.2e7;
  else if(simType=="prex2")
    runFactor = 1.7e8;
  else if(simType=="crex")
    runFactor = 4.67e8;
  else
    return 0;

  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  // TH1D *h1=(TH1D*)fin->Get("hSummary_neil");
  // TH1D *h2=(TH1D*)fin->Get("hSummary_mRem");
  TH1D *h1=(TH1D*)fin->Get("hSummary_neilLogX");
  TH1D *h2=(TH1D*)fin->Get("hSummary_mRemLogX");
  TH1D *h3=(TH1D*)fin->Get("hSummary_enerLogX");
  cout<<"NEIL results:\n";
  doOne(h1,runFactor);
  cout<<"mRem results:\n";
  doOne(h2,runFactor);
  cout<<"Energy results:\n";
  doOne(h3,runFactor);

  TH1D *hL=(TH1D*)fin->Get("Det_1006/ha_1006_n_enerLinX");
  if(hL)
    doLid(hL,runFactor);
  fin->Close();
  return 0;
}

void doLid(TH1D *h,double runV){
  int nb=h->GetXaxis()->GetNbins();
  string title=h->GetTitle();
  cout<<endl<<endl<<title<<endl;
  double ev2uA=1e13/1.6;
  double totFactor = ev2uA*runV;

  double b10=h->GetXaxis()->FindBin(10);
  double bEnd=h->GetXaxis()->GetNbins();
  cout<<endl<<"For this histogram the edge of the 10MeV bin is "<<h->GetXaxis()->GetBinLowEdge(b10)<<endl;
  double dx=-1;
  cout<<"\tHall Lid:\t"<<h->IntegralAndError(b10,bEnd,dx)*totFactor;
  cout<<"\t"<<dx*totFactor<<endl;
}

void doOne(TH1D *h, double runV){
  int nb=h->GetXaxis()->GetNbins();
  string title=h->GetTitle();
  cout<<endl<<endl<<title<<endl;
  double ev2uA=1e13/1.6;
  for(int i=2;i<=nb;i+=2){
    title=h->GetXaxis()->GetBinLabel(i);
    int det = atoi( title.substr( 0, title.find(" ")).c_str());
    if(det==1001 || det==1002 || det==1005 || det==1101 || det==1102){
      double totFactor = ev2uA/detArea[det]*runV;
      cout<<title<<"\t"<<h->GetBinContent(i)<<"\t"<<h->GetBinError(i)
          <<"\t"<<h->GetBinContent(i)*totFactor<<"\t"<<h->GetBinError(i)*totFactor<<endl;
    }else if(det==2101 || det==2102 || det==3201){
      double totFactor = ev2uA*runV;
      cout<<title<<"\t"<<h->GetBinContent(i)<<"\t"<<h->GetBinError(i)
          <<"\t"<<h->GetBinContent(i)*totFactor<<"\t"<<h->GetBinError(i)*totFactor<<endl;
    }
  }
}
