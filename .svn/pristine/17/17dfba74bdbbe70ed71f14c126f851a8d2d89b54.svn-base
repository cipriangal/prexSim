

using namespace std;

void drawtorroidmollers_rings(TString infilename="0", TString outfilenamestem="0")
{
	if (infilename=="0" || outfilenamestem=="0") {
		printf("Usage:\n\t.x drawtorroidmollers_rings_stamps.C(infilename, outfilename)\n\n");
		printf("Where infilename is a .root file \n");
		printf("  and outfilenamestem is the base name of an image file.\n");
		return;
	}  

	gROOT->Reset();
	char inifilename[200];//, outfilenamestem[200], parttype[100];

	Bool_t debug=0;

//	Bool_t doproton=0;
//	Bool_t dopostscript=0;
	Double_t deltaZwindowsize=20; //in mm

	sprintf(inifilename,"%s",infilename.Data());

	const Int_t totplots=6;
	Double_t distance[totplots]={6000,9000,14500,20000,24000,28250};
	TH2F* histos[totplots];

// 	const Int_t totplots=1;
// 	Double_t distance[totplots]={28479};

	char pstitle[200];

	printf("Using data file %s\n",inifilename);
	gROOT->SetStyle("Plain");
	gStyle->SetPalette(1);
	gStyle->SetOptStat(kFALSE);
	TFile *file0 = TFile::Open(inifilename);
	TCanvas *canvas = new TCanvas("canvas","Moller plots",800,800);

	char title[100];
	char weight[200];
	TGraph* scat[totplots];
	TLine line;
	TArc arc;
	arc.SetFillStyle(4000);
	arc.SetNoEdges(kFALSE);
	Double_t pi=3.14159265358979323846;
	Double_t length=600, openingangle=pi/14.;
	Double_t ymin, ymax, xmin, xmax;
	TTree *geant = (TTree*)file0->Get("geant");
	geant->SetMarkerStyle(20);
	geant->SetMarkerSize(0.15);
	for (int i=0; i<totplots; i++) {
		char drawstring[200];
		sprintf(drawstring,"y/1000:x/1000:1000*acos(pz2/sqrt(px2*px2+py2*py2+pz2*pz2))>>htemp%i",i);
		sprintf(weight,"(z>(%.0f)&&z<(%.0f)&&type==0)",distance[i]-deltaZwindowsize,distance[i]+deltaZwindowsize);
		if (debug) printf("%s\n%s\n",drawstring,weight);
		canvas->cd(i+1);
		Int_t num = geant->Draw(drawstring,weight,"colz");
		histos[i]=(TH2F*)gPad->GetPrimitive(Form("htemp%i",i));
		if (num<=0) {
			printf("\n%.0f doesn't seem to be a good choice!\n\n",distance[i]);
		} else {
			if (debug) printf("%i successful events\n",num);
			gPad->SetRightMargin(0.12);
			gPad->SetLeftMargin(0.12);
			gPad->SetTopMargin(0.12);
			gPad->SetBottomMargin(0.12);
			canvas->Update();
			ymin=gPad->GetUymin();
			ymax=gPad->GetUymax();
			xmin=gPad->GetUxmin();
			xmax=gPad->GetUxmax();
			printf("xmin %f, xmax %f, ymin %f, ymax %f, deltax %f, deltay %f\n",xmin,xmax,ymin,ymax,xmax-xmin,ymax-ymin);
		}
		sprintf(title,"z=%.2f m;x  (m);y   (m)", distance[i]/1000.);
		histos[i]->SetTitle(title);
		canvas->Update();
		canvas->Print(Form("%s_%.2fm.png",outfilenamestem.Data(),distance[i]/1000.));
	}	
}

/* emacs
 * Local Variables:
 * mode:C++
 * mode:font-lock
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 */
