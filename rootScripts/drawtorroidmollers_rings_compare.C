// 
//
//  Mark Dalton 2010/08

using namespace std;

void drawtorroidmollers_rings_compare(TString infilename1="0", TString infilename2="0", 
									  TString outfilenamestem="0", TString usercut="1")
{
	if (infilename1=="0" || infilename2=="0" || outfilenamestem=="0") {
		printf("Usage:\n\t.x drawtorroidmollers_rings_stamps.C(infilename1, infilename2, outfilename, [cut])\n\n");
		printf("Where infilenames is a .root file \n");
		printf("      outfilenamestem is the base name of an image file, and\n");
		printf("      cut is a cut string applied to the tree.\n");
		return;
	}  

	gROOT->Reset();
	Bool_t debug=1;
	Double_t deltaZwindowsize=25; //in mm

	const Int_t totplots=4;
	Double_t distance[totplots]={14500,20000,24000,28250};

// 	const Int_t totplots=1;
// 	Double_t distance[totplots]={28479};

	char pstitle[200];

	printf("Using data files %s\n             and %s \n\n",infilename1.Data(), infilename2.Data());
	gROOT->SetStyle("Plain");
	gStyle->SetPalette(1);
	gStyle->SetOptStat(kFALSE);
	TFile *file1 = TFile::Open(infilename1.Data());
	TTree *tree1 = (TTree*)file1->Get("geant");
	TFile *file2 = TFile::Open(infilename2.Data());
	TTree *tree2 = (TTree*)file2->Get("geant");
	TH2F* file1histos[totplots];
	TH2F* file2histos[totplots];

	Int_t Palette1[100],Palette2[100];
	UInt_t Number = 2;
	Int_t nb=20;

	Double_t Red[]    = { 1.00, 1.00};
	Double_t Green[]  = { 0.00, 0.00};
	Double_t Blue[]   = { 1.00, 0.00};
	Double_t Length[] = { 0.00, 1.00};
	Int_t FI = TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
	for (int i=0;i<nb;i++) Palette1[i] = FI+i;
	
	Double_t Red[]    = { 0.00, 0.00};
	Double_t Green[]  = { 1.00, 0.00};
	Double_t Blue[]   = { 1.00, 1.00};
	TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
	Int_t FI = TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
	for (int i=0;i<nb;i++) Palette2[i] = FI+i;
	
	TCanvas *canvas = new TCanvas("canvas","Moller plots",800,800);
	char title[100];
	char weight[200];
	TGraph* scat[totplots];
	TLine line;
	TArc arc;
	arc.SetFillStyle(4000);
	arc.SetNoEdges(kFALSE);
	Double_t pi=3.14159265358979323846;
	Double_t ymin, ymax, xmin, xmax;

	tree1->SetMarkerStyle(20);
	tree1->SetMarkerSize(0.15);
	tree2->SetMarkerStyle(20);
	tree2->SetMarkerSize(0.15);
	for (int i=0; i<totplots; i++) {
		char drawstring[400];
		gStyle->SetPalette(nb, Palette1);
		sprintf(drawstring,"y/1000:x/1000:1000*acos(pz2/sqrt(px2*px2+py2*py2+pz2*pz2))>>file1hist%i",i);
//		sprintf(drawstring,"y/1000:x/1000:5>>file1hist%i",i);
		sprintf(weight,"(z>(%.0f)&&z<(%.0f)&&type==0)&&(%s)",distance[i]-deltaZwindowsize,distance[i]+deltaZwindowsize,usercut.Data());
		if (debug) printf("%s\n%s\n",drawstring,weight);
		Int_t num1 = tree2->Draw(drawstring,weight,"col");
		file1histos[i]=(TH2F*)gPad->GetPrimitive(Form("file1hist%i",i));

		gStyle->SetPalette(nb, Palette2);
		sprintf(drawstring,"y/1000:x/1000:1000*acos(pz2/sqrt(px2*px2+py2*py2+pz2*pz2))>>file2hist%i",i);
//		sprintf(drawstring,"y/1000:x/1000:3>>file2hist%i",i);
		if (debug) printf("%s\n%s\n",drawstring,weight);
		Int_t num2 = tree1->Draw(drawstring,weight,"col");
		file2histos[i]=(TH2F*)gPad->GetPrimitive(Form("file2hist%i",i));

		if (num1<=0 || num2<=0) {
			printf("%i and %i successful events\n",num1,num2);
			printf("\n%.0f doesn't seem to be a good choice!\n\n",distance[i]);
		} else {
			if (debug) printf("%i and %i successful events\n",num1,num2);
			gPad->SetRightMargin(0.1);
			gPad->SetLeftMargin(0.1);
			gPad->SetTopMargin(0.1);
			gPad->SetBottomMargin(0.1);
			canvas->Update();
			ymin=gPad->GetUymin();
			ymax=gPad->GetUymax();
			xmin=gPad->GetUxmin();
			xmax=gPad->GetUxmax();
			printf("xmin %f, xmax %f, ymin %f, ymax %f, deltax %f, deltay %f\n",xmin,xmax,ymin,ymax,xmax-xmin,ymax-ymin);
// 			if (xmin>0) {
// // 				file1histos[i]->SetAxisRange(0,xmax,"X");
// // 				file2histos[i]->SetAxisRange(0,xmax,"X");
// 				file1histos[i]->GetXaxis()->SetRangeUser(0,xmax);
// 				file2histos[i]->GetXaxis()->SetRangeUser(0,xmax);
// 			}
			sprintf(title,"z=%.2f m;x  (m);y   (m)", distance[i]/1000.);
			file1histos[i]->SetTitle(title);
			file2histos[i]->SetTitle(title);
			canvas->Clear();
			file1histos[i]->Draw("colz");
// 			if (xmin>0) //file1histos[i]->GetXaxis()->SetRangeUser(0,xmax);
// 				gPad->SetUxmin(0);//file1histos[i]->SetAxisRange(0,xmax,"X");
			canvas->Update();
			canvas->Print(Form("%s_%.2fm_1.png",outfilenamestem.Data(),distance[i]/1000.));
			file2histos[i]->Draw("col,same");
			canvas->Update();
			canvas->Print(Form("%s_%.2fm.png",outfilenamestem.Data(),distance[i]/1000.));
			canvas->Clear();
			file2histos[i]->Draw("colz");
// 			if (xmin>0) //file2histos[i]->GetXaxis()->SetRangeUser(0,xmax);
// 				gPad->SetUxmin(0);//file1histos[i]->SetAxisRange(0,xmax,"X");
			canvas->Update();
			canvas->Print(Form("%s_%.2fm_2.png",outfilenamestem.Data(),distance[i]/1000.));
		}
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
