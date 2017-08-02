void parseBtable(){
  TGraph *g=new TGraph();
  double x,y,z,bx,by,bz;
  //ifstream fin("../Q1FieldMap-Beamline.table");
  //ifstream fin("../PREX_grid.table");
  //ifstream fin("../CREX_grid.table");
  //ifstream fin("../septum3od012inPipeInSquare.table");
  //ifstream fin("../prex_septumShieldPlus3odPipe_quadsWithConicalShieldPipe.table");
  //ifstream fin("../crex_septumShieldPlus3odPipe_quadsWithConicalShieldPipe.table");

  //ifstream fin("../prex_all_on_170609_properSeptumShield.table");
  //ifstream fin("../crex_all_on_170609_properSeptumShield.table");
  ifstream fin("../crex_all_on_Gavalya.table");
  // ifstream fin("../prex_Q1Q2off_170609_properSeptumShield.table");
  // ifstream fin("../crex_Q1Q2off_170609_properSeptumShield.table");
  // ifstream fin("../prex_Q2off_170609_properSeptumShield.table");
  // ifstream fin("../crex_Q2off_170609_properSeptumShield.table");

  char data[500];
  for(int i=0;i<8;i++){
    fin.getline(data,500);
    cout<<data<<endl;
  }
  int ng(0);
  int i(0);
  std::vector<double> zPos;
  std::vector<double> bX;
  while(fin>>x>>y>>z>>bx>>by>>bz){
    i++;
    if(fabs(x)<0.03 && fabs(y-1)<0.03){
      // cout<<x<<" "<<y<<" "<<z<<" "<<bx<<" "<<by<<" "<<bz<<endl;
      // cout<<endl<<"\t"<<abs(x)<<" -> "<<(abs(x)<0.03)<<" | "<<abs(y-1)<<" -> "<<(abs(y-1)<0.03)<<endl;
      zPos.push_back(z);
      bX.push_back(bx);
      g->SetPoint(ng,z,bx);
      ng++;
    }
  }
  cout<<" zPosition = {";
  for(int i=0;i<zPos.size();i++)
    cout<<" "<<zPos[i]<<",";
  cout<<"};\n";

  cout<<" bXcomponent = {";
  for(int i=0;i<zPos.size();i++)
    cout<<" "<<bX[i]<<",";
  cout<<"};\n";


  gStyle->SetOptFit(1);
  g->Draw("AL");
  fin.close();
}
