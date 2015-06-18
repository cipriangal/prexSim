//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
              
#include "MollerGlobalMagnetField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


MollerGlobalMagnetField::MollerGlobalMagnetField() 
{	
    
  G4cout << G4endl << "###### Calling MollerGlobalMagnetField::MollerGlobalMagnetField() " << G4endl << G4endl;

  fMagneticField_MainMagnet = NULL;
  fMagneticField_MiniMagnet = NULL;

  // mg_field_low = -44.0*cm;
  // mg_field_high = 44.0*cm;
  fieldMessenger = new MollerGlobalMagnetFieldMessenger(this);  
  //=======================================================
    //    configuration values of the QTOR field map
    //=======================================================

    BFieldScalingFactor_MainMagnet =   0.0;    // according to Juliette Mammei
    fMagneticField_MainMagnet = new MollerMagnetFieldMap();


    if(fMagneticField_MainMagnet){
      if (1) {
	BFieldScalingFactor_MainMagnet = 0.0;
	fMagneticField_MainMagnet->SetFieldMap_RMin(0.015*m);      
	fMagneticField_MainMagnet->SetFieldMap_RMax(0.375*m); 
	fMagneticField_MainMagnet->SetFieldMap_RStepsize(0.002*m);
	
	fMagneticField_MainMagnet->SetFieldMap_ZMin(9.5*m); 
	fMagneticField_MainMagnet->SetFieldMap_ZMax(17.0*m);
	fMagneticField_MainMagnet->SetFieldMap_ZStepsize(0.1*m);
	
	fMagneticField_MainMagnet->SetFieldMap_PhiMin(-25*degree);
	fMagneticField_MainMagnet->SetFieldMap_PhiMax(25*degree);
	fMagneticField_MainMagnet->SetFieldMap_PhiStepsize(1*degree);
	
	fMagneticField_MainMagnet->InitializeGrid();
	
	if (1) {
	  //fMagneticField_MainMagnet->ReadFieldMap("fieldmap_4seg_Oct24.txt");
	  //	  fMagneticField_MainMagnet->ReadFieldMap(hybridFieldMap);
	} else {
	  //	  fMagneticField_MainMagnet->ReadFieldMap("fieldtest_back_rphi_coarse.dat");
	}
	//fMagneticField_MainMagnet->PrintFieldCheck("magfieldcheck_back.txt");
      } else {
	BFieldScalingFactor_MainMagnet = 0.0;
	fMagneticField_MainMagnet->SetFieldMap_RMin(0.045*m);      
	fMagneticField_MainMagnet->SetFieldMap_RMax(0.375*m); 
	fMagneticField_MainMagnet->SetFieldMap_RStepsize(0.002*m);
	
	fMagneticField_MainMagnet->SetFieldMap_ZMin(9.5*m); 
	fMagneticField_MainMagnet->SetFieldMap_ZMax(17.0*m);
	fMagneticField_MainMagnet->SetFieldMap_ZStepsize(0.05*m);
	
	fMagneticField_MainMagnet->SetFieldMap_PhiMin(-25.7*degree);
	fMagneticField_MainMagnet->SetFieldMap_PhiMax(25.7*degree);
	fMagneticField_MainMagnet->SetFieldMap_PhiStepsize(0.2*degree);
	
	fMagneticField_MainMagnet->InitializeGrid();
	
	//	fMagneticField_MainMagnet->ReadFieldMap("fieldmap_back_6seg_rphi.txt");
      }
    }
    //=======================================================
    //    configuration values of the MiniTorus field map
    //=======================================================
        
    BFieldScalingFactor_MiniMagnet =  0.0;
    fMagneticField_MiniMagnet = new MollerMagnetFieldMap();

    if(fMagneticField_MiniMagnet){
      if (1) {
	// 100% of maxiumum current density (560 A/cm^2)
	BFieldScalingFactor_MiniMagnet =  0.0; // according to Juliette Mammei: 84% (470 A/cm^2)
	
	fMagneticField_MiniMagnet->SetFieldMap_RMin(0.015*m);      
	fMagneticField_MiniMagnet->SetFieldMap_RMax(0.375*m); 
	fMagneticField_MiniMagnet->SetFieldMap_RStepsize(0.002*m);
	
	fMagneticField_MiniMagnet->SetFieldMap_ZMin(6.0*m); 
	fMagneticField_MiniMagnet->SetFieldMap_ZMax(8.0*m);
	fMagneticField_MiniMagnet->SetFieldMap_ZStepsize(0.1*m);
	
	fMagneticField_MiniMagnet->SetFieldMap_PhiMin(-25*degree);
	fMagneticField_MiniMagnet->SetFieldMap_PhiMax(25*degree);
	fMagneticField_MiniMagnet->SetFieldMap_PhiStepsize(1*degree);
	
	fMagneticField_MiniMagnet->InitializeGrid();
	
	if (1) {
	  //fMagneticField_MiniMagnet->ReadFieldMap("fieldmap_front_4seg_Oct24.txt");
	  //	  fMagneticField_MiniMagnet->ReadFieldMap(upstreamFieldMap);
	} else {
	  //	  fMagneticField_MiniMagnet->ReadFieldMap("fieldtest_front_rphi_coarse.dat");
	}
	//fMagneticField_MiniMagnet->PrintGridCheck("maggridcheck_front.txt");
      } else {
	BFieldScalingFactor_MiniMagnet =  0.0; // according to Juliette Mammei: 84% (470 A/cm^2)
	
	fMagneticField_MiniMagnet->SetFieldMap_RMin(0.025*m);      
	fMagneticField_MiniMagnet->SetFieldMap_RMax(0.375*m); 
	fMagneticField_MiniMagnet->SetFieldMap_RStepsize(0.002*m);
	
	fMagneticField_MiniMagnet->SetFieldMap_ZMin(6.0*m); 
	fMagneticField_MiniMagnet->SetFieldMap_ZMax(8.0*m);
	fMagneticField_MiniMagnet->SetFieldMap_ZStepsize(0.05*m);
	
	fMagneticField_MiniMagnet->SetFieldMap_PhiMin(-25.7*degree);
	fMagneticField_MiniMagnet->SetFieldMap_PhiMax(25.7*degree);
	fMagneticField_MiniMagnet->SetFieldMap_PhiStepsize(0.2*degree);
	
	fMagneticField_MiniMagnet->InitializeGrid();
	
	//	fMagneticField_MiniMagnet->ReadFieldMap("fieldmap_front_6seg_rphi.txt");
      }
    }

    // G4cout << G4endl << "###### Printing Septum field on file ######" << G4endl << G4endl;
    // double z[201];
    // double grB[201][3];
    // double grB2[201][3];
    // double gr_dBxdy[201];
    // G4double g4grB[201][3];
    // G4double point[4];
    // point[0] = 1.*cm;
    // point[1] = 0.*cm;
    // point[3] = 0.*ns;
    // G4double point2[4];
    // double gr_dy = 0.001;
    // point2[0] = 1.*cm;
    // point2[1] = gr_dy *=cm;
    // point2[3] = 0.*ns;
    // for (int i=0; i<201; i++) {
    //   z[i] = double(i-100);
    //   point[2] = z[i] *=cm;
    //   point2[2] = z[i] *=cm;
    //   GetFieldValue(point,g4grB[i]);
    //   for (int j=0; j<3; j++) grB[i][j]=(double)g4grB[i][j]/gauss;
    //   GetFieldValue(point2,g4grB[i]);
    //   for (int j=0; j<3; j++) grB2[i][j]=(double)g4grB[i][j]/gauss;
    //   gr_dBxdy[i] = (grB2[i][0]- grB[i][0])/gr_dy;
    // }
    // TGraph * graph_Mag_field = new TGraph(201,z,gr_dBxdy);
    // graph_Mag_field->SetTitle("Magnetic Field #frac{dBx}{dy};z(cm);gauss/cm");
    // graph_Mag_field->Write();
    G4cout << G4endl << "Magnetic Shielding low limit= " << mg_field_low << "   Magnetic Shielding high limit=" << mg_field_high << G4endl;

    G4cout << G4endl << "###### Leaving MollerGlobalMagnetField::MollerGlobalMagnetField " << G4endl << G4endl;
}		
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MollerGlobalMagnetField::~MollerGlobalMagnetField()
{  
    G4cout << G4endl << "###### Calling MollerGlobalMagnetField::~MollerGlobalMagnetField() " << G4endl << G4endl;

    if (fMagneticField_MainMagnet) delete fMagneticField_MainMagnet;	
    if (fMagneticField_MiniMagnet) delete fMagneticField_MiniMagnet;
    delete fieldMessenger;
    
    G4cout << G4endl << "###### Leaving MollerGlobalMagnetField::~MollerGlobalMagnetField() " << G4endl << G4endl;
}	
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerGlobalMagnetField::GetFieldValue(const G4double Point[4], G4double *Bfield ) const
{

  //  G4cout << G4endl << "###### Calling MollerGlobalMagnetField::GetFieldValue() " << G4endl << G4endl;

  G4double myLocalPointInMainMagnet[4];
  G4double myLocalPointInMiniMagnet[4];
  
  G4double myLocalBfieldInMainMagnet[3];
  G4double myLocalBfieldInMiniMagnet[3];

  //G4bool readfrommap = true;

  G4bool readfrommap = false;
  G4double dBxdy = 0.*gauss/cm;
  G4double dBydx = 0.*gauss/cm;

  // gives integral 7003.4 G @ 1 cm
  G4double a = 158.*gauss/cm;
  G4double shielded = 0.78 *gauss/cm;
  G4double bu = -44*cm;
  // G4double mg2_field_low = -44*cm;
  // G4double mg2_field_high = 44*cm;
  //  G4double bu =  mg_field_low;
  G4double bd = 44*cm;
  //  G4double bd =  mg_field_high;
  G4double c1 = 4.2*cm;   
  G4double c2 = 14.15*cm;   

  G4double z_intercept_low = mg_field_low + pow( (2*pow(c1,2)*log(a*exp(-(pow(mg_field_low-bu,2)/(2*pow(c2,2))))/shielded)),0.5);
  G4double z_intercept_high =  mg_field_high - pow( (2*pow(c1,2)*log(a*exp(-(pow(mg_field_high-bd,2)/(2*pow(c2,2))))/shielded)),0.5);

  // G4cout << "z_intercept_low =" <<  z_intercept_low/cm << "  z_intercept_high=" << z_intercept_high/cm << " " <<  pow( (2*pow(c1,2)*log(a*exp(-(pow(mg_field_low-bu,2)/(2*pow(c2,2))))/shielded)),0.5)/cm <<  " " <<  pow( (2*pow(c1,2)*log(a*exp(-(pow(mg_field_high-bd,2)/(2*pow(c2,2))))/shielded)),0.5)/cm << G4endl;
  //---------------------------------------------------------------
  // translation from global Point[4] into local magnet coordinates
  //---------------------------------------------------------------
  //
  myLocalPointInMainMagnet[0] = Point[0];             // x-pos
  myLocalPointInMainMagnet[1] = Point[1];             // y-pos
  myLocalPointInMainMagnet[2] = Point[2]-69.91*cm;    // z-pos
  myLocalPointInMainMagnet[3] = Point[3];             // time
  //
  // (2) transformation into local MiniMagnet coordinate is easy
  //     since MiniMagnet is shifted by 505*cm in z
  myLocalPointInMiniMagnet[0] = Point[0];             // x-pos
  myLocalPointInMiniMagnet[1] = Point[1];             // y-pos
  myLocalPointInMiniMagnet[2] = Point[2];             // z-pos
  myLocalPointInMiniMagnet[3] = Point[3];             // time


  //  G4cout << "reading septum magnetic field" << G4endl;
  // Parameterizing the gradient as two gaussians at each end of the septum, defined for different z values
  // within the radius of the beampipe through the septum, r = 4.128 cm.
  if(!readfrommap&&sqrt(pow(myLocalPointInMainMagnet[0],2)+pow(myLocalPointInMainMagnet[1],2))<4.128*cm){
    
    if ((myLocalPointInMainMagnet[2]>-100*cm)&&(myLocalPointInMainMagnet[2]<100*cm)){

      if (myLocalPointInMainMagnet[2]<mg_field_low){ 

	dBxdy = a*(exp(-((pow(myLocalPointInMainMagnet[2]-bu,2))/(2*pow(c2,2)))));

      } else if ((myLocalPointInMainMagnet[2]>=mg_field_low)&&(myLocalPointInMainMagnet[2]<z_intercept_low)){

	dBxdy = a*(exp(-(pow(mg_field_low-bu,2)/(2*pow(c2,2)))))*(exp(-((pow(myLocalPointInMainMagnet[2]-mg_field_low,2))/(2*pow(c1,2)))));

      } else if ((myLocalPointInMainMagnet[2]>=z_intercept_low)&&(myLocalPointInMainMagnet[2]<=z_intercept_high)){

	dBxdy = shielded;

      } else if ((myLocalPointInMainMagnet[2]<=mg_field_high)&&(myLocalPointInMainMagnet[2]>z_intercept_high)){

	dBxdy = a*(exp(-(pow(mg_field_high-bd,2)/(2*pow(c2,2)))))*(exp(-((pow(myLocalPointInMainMagnet[2]-mg_field_high,2))/(2*pow(c1,2)))));

      } else if ((myLocalPointInMainMagnet[2]>mg_field_high)){ 

	dBxdy = a*(exp(-((pow(myLocalPointInMainMagnet[2]-bd,2))/(2*pow(c2,2)))));
  
      } else {

	dBxdy=0;

      }

      dBydx=dBxdy;
      
      Bfield[0] = -myLocalPointInMainMagnet[1]*dBxdy;
      Bfield[1] = -myLocalPointInMainMagnet[0]*dBydx;
      Bfield[2] = 0.;
      
      /*
      G4cout<<"Xloc = "<<myLocalPointInMainMagnet[0]/cm<<" cm, Yloc = "<<myLocalPointInMainMagnet[1]/cm<<" cm, Zloc = "<<myLocalPointInMainMagnet[2]/cm<<" cm"<<G4endl;
      G4cout<<"dBydx = "<<dBydx*cm/gauss<<" G/cm, dBxdy = "<<dBydx*cm/gauss<<" G/cm"<<G4endl;
      G4cout<<"B_x = "<<Bfield[0]/gauss<<" G, B_y = "<<Bfield[1]/gauss<<" G, B_z = "<<Bfield[2]/gauss<<" G"<<G4endl;
      */
    } else {
	
      Bfield[0] = 0.;
      Bfield[1] = 0.;
      Bfield[2] = 0.;
    }
  } else {
	
      Bfield[0] = 0.;
      Bfield[1] = 0.;
      Bfield[2] = 0.;
  }
  

  if(readfrommap){

    //     G4cout << " X pos [cm] = " << Point[0]/cm << " ,Y pos [cm] = " << Point[1]/cm << " ,Z pos [cm] = " << Point[2]/cm << G4endl;


    if(fMagneticField_MiniMagnet && fMagneticField_MainMagnet){
      fMagneticField_MiniMagnet->GetFieldValue( myLocalPointInMiniMagnet, myLocalBfieldInMiniMagnet );
      fMagneticField_MainMagnet->GetFieldValue( myLocalPointInMainMagnet, myLocalBfieldInMainMagnet );
    
      Bfield[0] = myLocalBfieldInMainMagnet[0]*BFieldScalingFactor_MainMagnet + myLocalBfieldInMiniMagnet[0]*BFieldScalingFactor_MiniMagnet;
      Bfield[1] = myLocalBfieldInMainMagnet[1]*BFieldScalingFactor_MainMagnet + myLocalBfieldInMiniMagnet[1]*BFieldScalingFactor_MiniMagnet;
      Bfield[2] = myLocalBfieldInMainMagnet[2]*BFieldScalingFactor_MainMagnet + myLocalBfieldInMiniMagnet[2]*BFieldScalingFactor_MiniMagnet;
      //     printf("%f %f %f %f %f %f %f %f %f %f %f %f\n",Point[0],Point[1],Point[2],
      // 	   myLocalBfieldInMainMagnet[0]*100,myLocalBfieldInMiniMagnet[0]*100,Bfield[0]*100,
      // 	   myLocalBfieldInMainMagnet[1]*100,myLocalBfieldInMiniMagnet[1]*100,Bfield[1]*100,
      // 	   myLocalBfieldInMainMagnet[2]*100,myLocalBfieldInMiniMagnet[2]*100,Bfield[2]*100);
    }
    else if(fMagneticField_MainMagnet && !fMagneticField_MiniMagnet){
      fMagneticField_MainMagnet->GetFieldValue( myLocalPointInMainMagnet, myLocalBfieldInMainMagnet );
    
      Bfield[0] = myLocalBfieldInMainMagnet[0]*BFieldScalingFactor_MainMagnet;
      Bfield[1] = myLocalBfieldInMainMagnet[1]*BFieldScalingFactor_MainMagnet;
      Bfield[2] = myLocalBfieldInMainMagnet[2]*BFieldScalingFactor_MainMagnet;
    }
    else if(!fMagneticField_MainMagnet && fMagneticField_MiniMagnet){
      fMagneticField_MiniMagnet->GetFieldValue( myLocalPointInMiniMagnet, myLocalBfieldInMiniMagnet );

      Bfield[0] =  myLocalBfieldInMiniMagnet[0]*BFieldScalingFactor_MiniMagnet;
      Bfield[1] =  myLocalBfieldInMiniMagnet[1]*BFieldScalingFactor_MiniMagnet;
      Bfield[2] =  myLocalBfieldInMiniMagnet[2]*BFieldScalingFactor_MiniMagnet;    
    }
    else{
      Bfield[0] = 0.0*kilogauss;
      Bfield[1] = 0.0*kilogauss;
      Bfield[2] = 0.0*kilogauss;
    }
  }
//   G4cout << "------------------------------------------------------------------------" << G4endl;
//   G4cout << " X pos [cm] = " << Point[0]/cm << "  , with X BField [kG] = " << Bfield[0] << G4endl;
//   G4cout << " Y pos [cm] = " << Point[1]/cm << "  , with Y BField [kG] = " << Bfield[1] << G4endl;
//   G4cout << " Z pos [cm] = " << Point[2]/cm << "  , with Z BField [kG] = " << Bfield[2] << G4endl;

//G4cout << G4endl << "###### Leaving MollerGlobalMagnetField::GetFieldValue() " << G4endl << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void MollerGlobalMagnetField::WriteMagField()
{
    G4cout << G4endl << "###### Printing Septum field on file ######" << G4endl << G4endl;
    G4cout << G4endl << "Magnetic Shielding low limit= " << mg_field_low << "   Magnetic Shielding high limit=" << mg_field_high << G4endl;

    double z[201];
    double grB[201][3];
    double grB2[201][3];
    double gr_dBxdy[201];
    G4double g4grB[201][3];
    G4double point[4];
    point[0] = 1.*cm;
    point[1] = 0.*cm;
    point[3] = 0.*ns;
    G4double point2[4];
    double gr_dy = 0.001;
    point2[0] = -1.*cm;
    point2[1] = gr_dy *cm;
    point2[3] = 0.*ns;
    for (int i=0; i<201; i++) {
      z[i] = 175.211-105.3+double(i-100);
      point[2] = z[i] *cm;
      point2[2] = z[i] *cm;
      GetFieldValue(point,g4grB[i]);
      for (int j=0; j<3; j++) grB[i][j]=(double)g4grB[i][j]/gauss;
      GetFieldValue(point2,g4grB[i]);
      for (int j=0; j<3; j++) grB2[i][j]=(double)g4grB[i][j]/gauss;
      gr_dBxdy[i] = (grB2[i][0]- grB[i][0])/gr_dy;
      G4cout << z[i] << " " << point[2]/cm << " " << gr_dBxdy[i] << G4endl;
    }
    TGraph * graph_Mag_field = new TGraph(201,z,gr_dBxdy);
    graph_Mag_field->SetTitle("Magnetic Field #frac{dBx}{dy} at (-1cm,0cm,z);z(cm);gauss/cm");
    graph_Mag_field->Write();




}

//=======================================================
//   -----------------------
//  | CVS File Information |
//  -----------------------
// 
//      $Revisions$  
//      $Log: MollerGlobalMagnetField.cc,v $
//      Revision 1.3  2006/03/01 17:03:16  grimm
//      Changed BFieldScalingFactor_MiniMagnet =  -0.15434; // according to Juliette Mammei
//      Before the scaling factor was ~0.07
//
//      Revision 1.2  2005/12/27 19:10:21  grimm
//      - Redesign of Doxygen header containing CVS info like revision and date
//      - Added CVS revision log at the end of file
//
// 
