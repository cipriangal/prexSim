void parseBtable(){
  TGraph *g=new TGraph();
  double x,y,z,bx,by,bz;
  ifstream fin("../Q1FieldMap-Beamline.table");
  char data[500];
  for(int i=0;i<8;i++)
    fin.getline(data,500);
  int ng(0);
  int i(0);
  cout<<"double b[]={";
  while(fin>>x>>y>>z>>bx>>by>>bz){
    i++;
    //if(i%10000==1) cout<<i<<endl;
    // if(y==0 && x==1){
    //   g->SetPoint(ng,z,-by);
    //   ng++;
    // }
    if(x==0 && y==1){
      g->SetPoint(ng,z,-bx);
      cout<<bx<<", ";
      ng++;
    }
  }
  cout<<"};\n";
  gStyle->SetOptFit(1);
  g->Draw("AL");
  fin.close();
}
