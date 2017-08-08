#include <map>
#include <string>

map <int,int> detArea;//cm2

int printHallRad(string fnm,string simType){

  detArea[1001] = 400000;
  detArea[1002] =  60000;
  detArea[1003] = 160000;
  detArea[1004] = 100000;
  detArea[1005] =  55000;
  detArea[1101] =  60000;
  detArea[1102] =  60000;

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
  TH1D *h1=(TH1D*)fin->Get("hSummary_neilLogX");
  TH1D *h2=(TH1D*)fin->Get("hSummary_mRemLogX");
  doOne(h1,runFactor[simType]);
  doOne(h2,runFactor[simType]);
  fin->Close();
}

void doOne(TH1D *h, double runV){
  int nb=h->GetXaxis()->GetNbins();
  string title=h->GetTitle();
  cout<<endl<<endl<<title<<endl;
  double ev2uA=1e13/1.6;
  for(int i=2;i<=nb;i+=2){
    title=h->GetXaxis()->GetBinLabel(i);
    int det = atoi( title.substr( 0, title.find(" ")).c_str());
    if(det>1102) continue;
    double totFactor = ev2uA/detArea[det]*runV;
    cout<<title<<"\t"<<h->GetBinContent(i)<<"\t"<<h->GetBinError(i)
	<<"\t"<<h->GetBinContent(i)*totFactor<<"\t"<<h->GetBinError(i)*totFactor<<endl;
  }
}

