void PlotCollNeutrons(){


  //Attaching file o_Caryn20150517_ConcreteAboveColl.root as _file0...
  //TCanvas* c2 = new TCanvas("c2","c2",0,0,1200,700)
  TCanvas* c2 = new TCanvas("c2","c2",0,0,1200,700);
  c2->Divide(3,2);
  c2->cd(1);
  geant->Draw("y:x:z","volume==5050 && type==5");
  c2->cd(2);
  geant->Draw("y:x:z","volume==5050 && type==5&&kineE<0.01");
  c2->cd(3);
  geant->Draw("y:x:z","volume==5050 && type==5&&kineE>0.01&&kineE<30");
  c2->cd(4);
  geant->Draw("y:x:z","volume==5050 && type==5&&kineE>0.01&&kineE>30");
  c2->cd(5);
  geant->Draw("y:x:z","volume==5050 && type==5&&kineE>10&&kineE<30");
  c2->cd(6);
  geant->Draw("y:x:z","volume==5050 && type==5&&kineE>1&&kineE<10");

 TCanvas* c2t = new TCanvas("c2t","c2t",0,0,1200,700);
 c2t->Divide(3,2);
 c2t->cd(1);
 geant->Draw("y:z","volume==5050 && type==5");
 c2t->cd(2);
 geant->Draw("y:z","volume==5050 && type==5&&y>113&&y<139");
 c2t->cd(3);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139");
 c2t->cd(4);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139&&kineE<0.01");
 c2t->cd(5);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139&&kineE>0.01&&kineE<30");
 c2t->cd(6);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139&&kineE>30");

 TCanvas* c2tc = new TCanvas("c2tc","c2tc",0,0,1200,700);
 c2tc->Divide(3,2);
 c2tc->cd(1);
 geant->Draw("y:z","volume==5050 && type==5","colz");
 c2tc->cd(2);
 geant->Draw("y:z","volume==5050 && type==5&&y>113&&y<139","colz");
 c2tc->cd(3);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139","colz");
 c2tc->cd(4);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139&&kineE<0.01","colz");
 c2tc->cd(5);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139&&kineE>0.01&&kineE<30","colz");
 c2tc->cd(6);
 geant->Draw("x:z","volume==5050 && type==5&&y>113&&y<139&&kineE>30","colz");

 TCanvas* c2b = new TCanvas("c2b","c2b",0,0,1200,700);
  c2b->Divide(3,2);
  c2b->cd(1);
  geant->Draw("y:z","volume==5050 && type==5");
  c2b->cd(2);
  geant->Draw("y:z","volume==5050 && type==5&&y<-113&&y>-139");
  c2b->cd(3);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139");
  c2b->cd(4);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139&&kineE<0.01");
  c2b->cd(5);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139&&kineE>0.01&&kineE<30");
  c2b->cd(6);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139&&kineE>30");    

  TCanvas* c2bc = new TCanvas("c2bc","c2bc",0,0,1200,700);
  c2bc->Divide(3,2);
  c2bc->cd(1);
  geant->Draw("y:z","volume==5050 && type==5","colz");
  c2bc->cd(2);
  geant->Draw("y:z","volume==5050 && type==5&&y<-113&&y>-139","colz");
  c2bc->cd(3);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139","colz");
  c2bc->cd(4);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139&&kineE<0.01","colz");
  c2bc->cd(5);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139&&kineE>0.01&&kineE<30","colz");
    c2bc->cd(6);
  geant->Draw("x:z","volume==5050 && type==5&&y<-113&&y>-139&&kineE>30","colz");

  TCanvas* c2l = new TCanvas("c2l","c2l",0,0,1200,700);
  c2l->Divide(3,2);
  c2l->cd(1);
  geant->Draw("y:x","volume==5050 && type==5");
  c2l->cd(2);
  geant->Draw("y:x","volume==5050&&type==5&&x<-198&&x>-235");
  c2l->cd(3);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235");
  c2l->cd(4);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235&&kineE<0.01");
  c2l->cd(5);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235&&kineE>0.01&&kineE<30");
  c2l->cd(6);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235&&kineE>30");
  
  TCanvas* c2lc = new TCanvas("c2lc","c2lc",0,0,1200,700);
  c2lc->Divide(3,2);
  c2lc->cd(1);
  geant->Draw("y:x","volume==5050 && type==5","colz");
  c2lc->cd(2);
  geant->Draw("y:x","volume==5050&&type==5&&x<-198&&x>-235","colz");
  c2lc->cd(3);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235","colz");
  c2lc->cd(4);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235&&kineE<0.01","colz");
  c2lc->cd(5);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235&&kineE>0.01&&kineE<30","colz");
  c2lc->cd(6);
  geant->Draw("y:z","volume==5050&&type==5&&x<-198&&x>-235&&kineE>30","colz");


  TCanvas* c2r = new TCanvas("c2r","c2r",0,0,1200,700);
  c2r->Divide(3,2);
  c2r->cd(1);
  geant->Draw("y:x","volume==5050 && type==5");
  c2r->cd(2);
  geant->Draw("y:x","volume==5050&&type==5&&x>198&&x<235");
  c2r->cd(3);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235");
  c2r->cd(4);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235&&kineE<0.01");
  c2r->cd(5);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235&&kineE>0.01&&kineE<30");
  c2r->cd(6);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235&&kineE>30");

  TCanvas* c2rc = new TCanvas("c2rc","c2rc",0,0,1200,700);
  c2rc->Divide(3,2);
  c2rc->cd(1);
  geant->Draw("y:x","volume==5050 && type==5","colz");
  c2rc->cd(2);
  geant->Draw("y:x","volume==5050&&type==5&&x>198&&x<235","colz");
  c2rc->cd(3);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235","colz");
  c2rc->cd(4);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235&&kineE<0.01","colz");
  c2rc->cd(5);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235&&kineE>0.01&&kineE<30","colz");
  c2rc->cd(6);
  geant->Draw("y:z","volume==5050&&type==5&&x>198&&x<235&&kineE>30","colz");


}
