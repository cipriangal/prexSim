#include "TChain.h"
#include <stdio.h>

using namespace std;

void sumRootFiles(){
  int nfiles = 20;
  string fnms[nfiles];
  
  for(int i = 0; i < nfiles; i++){
  	char fbuf[100];
  	if(i < 10){sprintf(fbuf, "../output/crex5_500kEv_000%d_shieldless/o_prexSim.root", i); }
  	else{sprintf(fbuf, "../output/crex5_500kEv_00%d_shieldless/o_prexSim.root", i);}
  	fnms[i] = (string)fbuf;
  }
  
  TChain ch("geant");
  
  for(int i = 0; i < nfiles; i++){
  	printf(fnms[i].c_str()); printf("\n");
  	ch.Add(fnms[i].c_str());
  }
  
  ch.Merge("shieldless.root");
  
}
