void PlotRatios(Int_t volumeN){

  Double_t mean1;
  Double_t N1;
  Double_t deposit1;
  Double_t err1;
  Double_t mean2;
  Double_t N2;
  Double_t deposit2;
  Double_t err2;
  Double_t ratio;
  Double_t err_ratio;

  geant->Draw("kineE>>ha",Form("volume==%d&&z0>-1800&&z0<-800&&kineE>30&&abs(x0)<1000",volumeN));
  mean1 = ha->GetMean();
  N1 = ha->GetEntries();
  deposit1 = N1*mean1;
  err1 = ha->GetMeanError();
  err1=N1*err1;
  cout<< volumeN <<" E>30 :from target :" <<deposit1<< "+-" <<err1<<endl;
  geant->Draw("kineE>>hb",Form("volume==%d&&kineE>30",volumeN));
  mean2 = hb->GetMean();
  N2 = hb->GetEntries();
  deposit2 = N2*mean2;
  err2 = hb->GetMeanError();
  err2=N2*err2;
  cout<< volumeN <<" E>30 :from all :" <<deposit2<< "+-" <<err2<<endl;
  ratio=100*deposit1/deposit2;
  err_ratio=100*sqrt(pow(err1/deposit2,2)+pow(err2*deposit1/deposit2/deposit2,2));
  cout<< volumeN <<" E>30 :" <<ratio<< "+-" <<err_ratio<< "% from target" <<endl;

  geant->Draw("kineE>>ha1",Form("volume==%d&&z0>-1800&&z0<-800&&kineE>30&&abs(x0)<1000&&((type==5||type==4)||type==0)",volumeN));
  mean1 = ha1->GetMean();
  N1 = ha1->GetEntries();
  deposit1 = N1*mean1;
  err1 = ha1->GetMeanError();
  err1=N1*err1;
  cout<< volumeN <<" E>30 g,n,e :from target :" <<deposit1<< "+-" <<err1<<endl;
  geant->Draw("kineE>>hb1",Form("volume==%d&&kineE>30&&((type==5||type==4)||type==0)",volumeN));
  mean2 = hb1->GetMean();
  N2 = hb1->GetEntries();
  deposit2 = N2*mean2;
  err2 = hb1->GetMeanError();
  err2=N2*err2;
  cout<< volumeN <<" E>30 g,n,e :from all :" <<deposit2<< "+-" <<err2<<endl;
  ratio=100*deposit1/deposit2;
  err_ratio=100*sqrt(pow(err1/deposit2,2)+pow(err2*deposit1/deposit2/deposit2,2));
  //  err_ratio=100*sqrt((pow,err1/deposit2,2)+(pow,err2*deposit1/deposit2/deposit2,2));
  cout<< volumeN <<" E>30 g,n,e :" <<ratio<< "+-" <<err_ratio<< "% from target" <<endl;

geant->Draw("kineE>>ha2",Form("volume==7001&&z0>-1800&&z0<-800&&kineE>30&&abs(x0)<1000&&(type==5)",volumeN));
  mean1 = ha2->GetMean();
  N1 = ha2->GetEntries();
  deposit1 = N1*mean1;
  err1 = ha2->GetMeanError();
  err1=N1*err1;
  cout<< volumeN <<" E>30 n :from target :" <<deposit1<< "+-" <<err1<<endl;
  geant->Draw("kineE>>hb2",Form("volume==7001&&kineE>30&&(type==5)",volumeN));
  mean2 = hb2->GetMean();
  N2 = hb2->GetEntries();
  deposit2 = N2*mean2;
  err2 = hb2->GetMeanError();
  err2=N2*err2;
  cout<< volumeN <<" E>30 n :from all :" <<deposit2<< "+-" <<err2<<endl;
  ratio=100*deposit1/deposit2;
  err_ratio=100*sqrt(pow(err1/deposit2,2)+pow(err2*deposit1/deposit2/deposit2,2));
  //  err_ratio=100*sqrt((pow,err1/deposit2,2)+(pow,err2*deposit1/deposit2/deposit2,2));
  cout<< volumeN<< " E>30 n :" <<ratio<< "+-" <<err_ratio<< "% from target" <<endl;

}
