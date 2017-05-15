//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MollerGlobalMagnetField.hh"
#include "TGraph.h"
#include "radDamage.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace CLHEP;

MollerGlobalMagnetField::MollerGlobalMagnetField()
{

  G4cout << G4endl << "###### Calling MollerGlobalMagnetField::MollerGlobalMagnetField() " << G4endl << G4endl;

  fMagneticField_MainMagnet = NULL;
  fMagneticField_MiniMagnet = NULL;

  fieldMessenger = new MollerGlobalMagnetFieldMessenger(this);
  //=======================================================
  //    configuration values of the QTOR field map
  //=======================================================

  BFieldScalingFactor_MainMagnet =   0.0;    // according to Juliette Mammei
  fMagneticField_MainMagnet = new MollerMagnetFieldMap();


  if(fMagneticField_MainMagnet){
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

  }

  //=======================================================
  //    configuration values of the MiniTorus field map
  //=======================================================

  BFieldScalingFactor_MiniMagnet =  0.0;
  fMagneticField_MiniMagnet = new MollerMagnetFieldMap();

  if(fMagneticField_MiniMagnet){
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

  }
  configuration = "crex";
  addQ1fringe=false;

  G4cout << G4endl
         << "Magnetic Shielding low limit= " << mg_field_low
         << "   Magnetic Shielding high limit= " << mg_field_high << G4endl;
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

void MollerGlobalMagnetField::SetConfiguration(const G4String val){
  configuration = val;

  if( val == "prex1" ){
    magScaleFactor = 1.;
    mg_field_low = -44*cm;
    mg_field_high = 44*cm;
  }else if( val == "prex2" ){
    magScaleFactor = 1.;
    mg_field_low = -74*cm;
    mg_field_high = 74*cm;
  }else if( val == "crex" ){
    magScaleFactor = 0.219;
    mg_field_low = -69*cm;
    mg_field_high = 61*cm;
  }else{
    G4cerr<<__LINE__<<"\t"<<__PRETTY_FUNCTION__
          <<"\n\tUnknown magnetic field configuration! Quitting!\n";
    exit(1);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerGlobalMagnetField::GetFieldValue(const G4double Point[4], G4double *Bfield ) const
{
  if( configuration == "prex1" ){
    GetFieldValuePREX2(Point,Bfield);
  }else if( configuration == "prex2" ){
    GetFieldValuePREX2(Point,Bfield);
  }else if( configuration == "crex" ){
    GetFieldValueCREX(Point,Bfield);
  }

  if(addQ1fringe)
    GetFieldValueQ1(Point,Bfield);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerGlobalMagnetField::GetFieldValuePREX2(const G4double Point[4], G4double *Bfield ) const
{

  G4double myLocalPointInMainMagnet[4];
  G4double myLocalBfieldInMainMagnet[3];

  G4double dBxdy = 0.*gauss/cm;
  G4double dBydx = 0.*gauss/cm;

  // gives integral 7003.4 G @ 1 cm
  G4double a = magScaleFactor*158.*gauss/cm;
  G4double shielded = magScaleFactor*0.78 *gauss/cm;
  // bu and bd define center of gaussian approximations of fringe field along z-axis
  G4double bu = -44*cm;
  G4double bd = 44*cm;
  // c1 is gaussian width inside the shield
  G4double c1 = 4.2*cm;
  // c2 is gaussian width outside the shield
  G4double c2 = 14.15*cm;

  // Worksite for new gaussian - Ricky
  // Calculating the new peaks au and ad
  G4double au = a*(exp(-((pow(mg_field_low-bu,2))/(2*pow(c2,2)))));
  G4double ad = a*(exp(-((pow(mg_field_high-bd,2))/(2*pow(c2,2)))));
  G4double z_intercept_low = mg_field_low + pow( (2*pow(c1,2)*log(a*exp(-(pow(mg_field_low-bu,2)/(2*pow(c2,2))))/shielded)),0.5);
  G4double z_intercept_high =  mg_field_high - pow( (2*pow(c1,2)*log(a*exp(-(pow(mg_field_high-bd,2)/(2*pow(c2,2))))/shielded)),0.5);

  //---------------------------------------------------------------
  // translation from global Point[4] into local magnet coordinates
  //---------------------------------------------------------------
  myLocalPointInMainMagnet[0] = Point[0];             // x-pos
  myLocalPointInMainMagnet[1] = Point[1];             // y-pos
  myLocalPointInMainMagnet[2] = Point[2]-69.91*cm;    // z-pos
  myLocalPointInMainMagnet[3] = Point[3];             // time

  // Parameterizing the gradient as two gaussians at each end of the septum, defined for different z values
  // within the radius of the beampipe through the septum, r = 4.128 cm.
  if(sqrt(pow(myLocalPointInMainMagnet[0],2)+pow(myLocalPointInMainMagnet[1],2))<4.128*cm){
    if ((myLocalPointInMainMagnet[2]>-100*cm)&&(myLocalPointInMainMagnet[2]<100*cm)){
      if (myLocalPointInMainMagnet[2]<mg_field_low){
        dBxdy = a*(exp(-((pow(myLocalPointInMainMagnet[2]-bu,2))/(2*pow(c2,2)))));
      } else if ((myLocalPointInMainMagnet[2]>=mg_field_low)&&(myLocalPointInMainMagnet[2]<z_intercept_low)){
        dBxdy = a*(exp(-(pow(mg_field_low-bu                         ,2)/(2*pow(c2,2)))))*
          (exp(-(pow(myLocalPointInMainMagnet[2]-mg_field_low,2)/(2*pow(c1,2)))));
      } else if ((myLocalPointInMainMagnet[2]>=z_intercept_low)&&(myLocalPointInMainMagnet[2]<=z_intercept_high)){
        dBxdy = shielded;
      } else if ((myLocalPointInMainMagnet[2]<=mg_field_high)&&(myLocalPointInMainMagnet[2]>z_intercept_high)){
        dBxdy = a*(exp(-(pow(mg_field_high-bd                         ,2)/(2*pow(c2,2)))))*
          (exp(-(pow(myLocalPointInMainMagnet[2]-mg_field_high,2)/(2*pow(c1,2)))));
      } else if ((myLocalPointInMainMagnet[2]>mg_field_high)){
        dBxdy = a*(exp(-((pow(myLocalPointInMainMagnet[2]-bd,2))/(2*pow(c2,2)))));
      } else {
        dBxdy=0;
      }
      dBydx=dBxdy;
      Bfield[0] = -myLocalPointInMainMagnet[1]*dBxdy;
      Bfield[1] = -myLocalPointInMainMagnet[0]*dBydx;
      Bfield[2] = 0.;
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerGlobalMagnetField::GetFieldValueQ1(const G4double Point[4], G4double *Bfield ) const
{

  G4double myLocalPointInMainMagnet[4];
  G4double dBydx = 0.*gauss/cm;

  //---------------------------------------------------------------
  // translation from global Point[4] into local magnet coordinates
  //---------------------------------------------------------------
  myLocalPointInMainMagnet[0] = Point[0];             // x-pos
  myLocalPointInMainMagnet[1] = Point[1];             // y-pos
  myLocalPointInMainMagnet[2] = Point[2]-69.91*cm;    // z-pos
  myLocalPointInMainMagnet[3] = Point[3];             // time

  std::vector<double> z {-200, -199.5, -199, -198.5, -198, -197.5, -197, -196.5, -196, -195.5, -195, -194.5, -194, -193.5, -193, -192.5, -192, -191.5, -191, -190.5, -190, -189.5, -189, -188.5, -188, -187.5, -187, -186.5, -186, -185.5, -185, -184.5, -184, -183.5, -183, -182.5, -182, -181.5, -181, -180.5, -180, -179.5, -179, -178.5, -178, -177.5, -177, -176.5, -176, -175.5, -175, -174.5, -174, -173.5, -173, -172.5, -172, -171.5, -171, -170.5, -170, -169.5, -169, -168.5, -168, -167.5, -167, -166.5, -166, -165.5, -165, -164.5, -164, -163.5, -163, -162.5, -162, -161.5, -161, -160.5, -160, -159.5, -159, -158.5, -158, -157.5, -157, -156.5, -156, -155.5, -155, -154.5, -154, -153.5, -153, -152.5, -152, -151.5, -151, -150.5, -150, -149.5, -149, -148.5, -148, -147.5, -147, -146.5, -146, -145.5, -145, -144.5, -144, -143.5, -143, -142.5, -142, -141.5, -141, -140.5, -140, -139.5, -139, -138.5, -138, -137.5, -137, -136.5, -136, -135.5, -135, -134.5, -134, -133.5, -133, -132.5, -132, -131.5, -131, -130.5, -130, -129.5, -129, -128.5, -128, -127.5, -127, -126.5, -126, -125.5, -125, -124.5, -124, -123.5, -123, -122.5, -122, -121.5, -121, -120.5, -120, -119.5, -119, -118.5, -118, -117.5, -117, -116.5, -116, -115.5, -115, -114.5, -114, -113.5, -113, -112.5, -112, -111.5, -111, -110.5, -110, -109.5, -109, -108.5, -108, -107.5, -107, -106.5, -106, -105.5, -105, -104.5, -104, -103.5, -103, -102.5, -102, -101.5, -101, -100.5, -100, -99.5, -99, -98.5, -98, -97.5, -97, -96.5, -96, -95.5, -95, -94.5, -94, -93.5, -93, -92.5, -92, -91.5, -91, -90.5, -90, -89.5, -89, -88.5, -88, -87.5, -87, -86.5, -86, -85.5, -85, -84.5, -84, -83.5, -83, -82.5, -82, -81.5, -81, -80.5, -80, -79.5, -79, -78.5, -78, -77.5, -77, -76.5, -76, -75.5, -75, -74.5, -74, -73.5, -73, -72.5, -72, -71.5, -71, -70.5, -70, -69.5, -69, -68.5, -68, -67.5, -67, -66.5, -66, -65.5, -65, -64.5, -64, -63.5, -63, -62.5, -62, -61.5, -61, -60.5, -60, -59.5, -59, -58.5, -58, -57.5, -57, -56.5, -56, -55.5, -55, -54.5, -54, -53.5, -53, -52.5, -52, -51.5, -51, -50.5, -50, -49.5, -49, -48.5, -48, -47.5, -47, -46.5, -46, -45.5, -45, -44.5, -44, -43.5, -43, -42.5, -42, -41.5, -41, -40.5, -40, -39.5, -39, -38.5, -38, -37.5, -37, -36.5, -36, -35.5, -35, -34.5, -34, -33.5, -33, -32.5, -32, -31.5, -31, -30.5, -30, -29.5, -29, -28.5, -28, -27.5, -27, -26.5, -26, -25.5, -25, -24.5, -24, -23.5, -23, -22.5, -22, -21.5, -21, -20.5, -20, -19.5, -19, -18.5, -18, -17.5, -17, -16.5, -16, -15.5, -15, -14.5, -14, -13.5, -13, -12.5, -12, -11.5, -11, -10.5, -10, -9.5, -9, -8.5, -8, -7.5, -7, -6.5, -6, -5.5, -5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15, 15.5, 16, 16.5, 17, 17.5, 18, 18.5, 19, 19.5, 20, 20.5, 21, 21.5, 22, 22.5, 23, 23.5, 24, 24.5, 25, 25.5, 26, 26.5, 27, 27.5, 28, 28.5, 29, 29.5, 30, 30.5, 31, 31.5, 32, 32.5, 33, 33.5, 34, 34.5, 35, 35.5, 36, 36.5, 37, 37.5, 38, 38.5, 39, 39.5, 40, 40.5, 41, 41.5, 42, 42.5, 43, 43.5, 44, 44.5, 45, 45.5, 46, 46.5, 47, 47.5, 48, 48.5, 49, 49.5, 50, 50.5, 51, 51.5, 52, 52.5, 53, 53.5, 54, 54.5, 55, 55.5, 56, 56.5, 57, 57.5, 58, 58.5, 59, 59.5, 60, 60.5, 61, 61.5, 62, 62.5, 63, 63.5, 64, 64.5, 65, 65.5, 66, 66.5, 67, 67.5, 68, 68.5, 69, 69.5, 70, 70.5, 71, 71.5, 72, 72.5, 73, 73.5, 74, 74.5, 75, 75.5, 76, 76.5, 77, 77.5, 78, 78.5, 79, 79.5, 80, 80.5, 81, 81.5, 82, 82.5, 83, 83.5, 84, 84.5, 85, 85.5, 86, 86.5, 87, 87.5, 88, 88.5, 89, 89.5, 90, 90.5, 91, 91.5, 92, 92.5, 93, 93.5, 94, 94.5, 95, 95.5, 96, 96.5, 97, 97.5, 98, 98.5, 99, 99.5, 100, 100.5, 101, 101.5, 102, 102.5, 103, 103.5, 104, 104.5, 105, 105.5, 106, 106.5, 107, 107.5, 108, 108.5, 109, 109.5, 110, 110.5, 111, 111.5, 112, 112.5, 113, 113.5, 114, 114.5, 115, 115.5, 116, 116.5, 117, 117.5, 118, 118.5, 119, 119.5, 120, 120.5, 121, 121.5, 122, 122.5, 123, 123.5, 124, 124.5, 125, 125.5, 126, 126.5, 127, 127.5, 128, 128.5, 129, 129.5, 130, 130.5, 131, 131.5, 132, 132.5, 133, 133.5, 134, 134.5, 135, 135.5, 136, 136.5, 137, 137.5, 138, 138.5, 139, 139.5, 140, 140.5, 141, 141.5, 142, 142.5, 143, 143.5, 144, 144.5, 145, 145.5, 146, 146.5, 147, 147.5, 148, 148.5, 149, 149.5, 150, 150.5, 151, 151.5, 152, 152.5, 153, 153.5, 154, 154.5, 155, 155.5, 156, 156.5, 157, 157.5, 158, 158.5, 159, 159.5, 160, 160.5, 161, 161.5, 162, 162.5, 163, 163.5, 164, 164.5, 165, 165.5, 166, 166.5, 167, 167.5, 168, 168.5, 169, 169.5, 170, 170.5, 171, 171.5, 172, 172.5, 173, 173.5, 174, 174.5, 175, 175.5, 176, 176.5, 177, 177.5, 178, 178.5, 179, 179.5, 180, 180.5, 181, 181.5, 182, 182.5, 183, 183.5, 184, 184.5, 185, 185.5, 186, 186.5, 187, 187.5, 188, 188.5, 189, 189.5, 190, 190.5, 191, 191.5, 192, 192.5, 193, 193.5, 194, 194.5, 195, 195.5, 196, 196.5, 197, 197.5, 198, 198.5, 199, 199.5, 200};
  std::vector<double> b {-0.00430807, -0.00434033, -0.00437288, -0.00440573, -0.00443886, -0.00447229, -0.00450602, -0.00454005, -0.00457438, -0.00460903, -0.00464399, -0.00467926, -0.00471486, -0.00475077, -0.00478701, -0.00482358, -0.00486048, -0.00489772, -0.00493529, -0.00497321, -0.00501147, -0.00505009, -0.00508906, -0.00512838, -0.00516807, -0.00520812, -0.00524854, -0.00528933, -0.0053305, -0.00537205, -0.00541399, -0.00545632, -0.00549903, -0.00554215, -0.00558503, -0.00562805, -0.0056734, -0.00572536, -0.00568739, -0.00582419, -0.00587284, -0.00591746, -0.00596254, -0.00600679, -0.00605403, -0.00610222, -0.00615086, -0.00619996, -0.00624953, -0.00629958, -0.00635009, -0.0064011, -0.00645259, -0.00650457, -0.00655706, -0.00661004, -0.00666354, -0.00671755, -0.00677208, -0.00682714, -0.00688274, -0.00693887, -0.00699555, -0.00705278, -0.00711056, -0.00716799, -0.00722854, -0.007288, -0.00734805, -0.00740869, -0.00746993, -0.00753177, -0.00759422, -0.00765729, -0.00772099, -0.00778532, -0.00785029, -0.00791591, -0.00798218, -0.00804912, -0.00811672, -0.008185, -0.00825397, -0.00832363, -0.008394, -0.00846507, -0.00853686, -0.00860938, -0.00868263, -0.00875663, -0.00883137, -0.00890688, -0.00898316, -0.00906022, -0.00917457, -0.00920469, -0.00927722, -0.00927508, -0.009441, -0.00943532, -0.00960808, -0.00969183, -0.00977645, -0.00986194, -0.00994833, -0.0100356, -0.0101238, -0.0102129, -0.0103029, -0.0103939, -0.0104859, -0.0105788, -0.0106726, -0.0107675, -0.0108634, -0.0109603, -0.0110582, -0.0111571, -0.0112571, -0.0113582, -0.0114603, -0.0115635, -0.0116679, -0.0117733, -0.0118799, -0.0119876, -0.0120965, -0.0122065, -0.0123178, -0.0124302, -0.0125438, -0.0126587, -0.0127749, -0.0128923, -0.0130109, -0.0131309, -0.0132522, -0.0133748, -0.0134988, -0.0136241, -0.0137508, -0.0138789, -0.0139842, -0.01413, -0.0142632, -0.0143979, -0.014534, -0.0146715, -0.0148105, -0.014951, -0.015093, -0.0152365, -0.015394, -0.0155393, -0.0156863, -0.0158351, -0.0159857, -0.0161381, -0.0162923, -0.0164483, -0.0166061, -0.0167658, -0.0169273, -0.0170908, -0.0172563, -0.0174236, -0.0175928, -0.0177641, -0.0179376, -0.0181128, -0.0182901, -0.0184694, -0.0186513, -0.0188342, -0.01902, -0.019208, -0.0193981, -0.0195881, -0.0197841, -0.0199811, -0.0201804, -0.020382, -0.0205859, -0.0207924, -0.0210012, -0.0212126, -0.0214265, -0.0216429, -0.0218619, -0.0220835, -0.0223078, -0.0225347, -0.0227644, -0.0229968, -0.023232, -0.0234701, -0.0237111, -0.0239549, -0.0242017, -0.0244516, -0.0247044, -0.0249604, -0.0252194, -0.0254817, -0.0257471, -0.0260158, -0.0262878, -0.0265723, -0.0268434, -0.0271255, -0.0274111, -0.0277003, -0.0279931, -0.0282895, -0.0285896, -0.0288935, -0.0292012, -0.0295127, -0.0298282, -0.0301476, -0.0304711, -0.0307986, -0.0311303, -0.0314662, -0.0318063, -0.0321508, -0.0324997, -0.032853, -0.0332108, -0.0335732, -0.0339403, -0.0343121, -0.0346887, -0.0350701, -0.0354565, -0.0358478, -0.0362442, -0.0366458, -0.0370526, -0.0374647, -0.0378822, -0.0383051, -0.0387336, -0.0391677, -0.0396075, -0.0400531, -0.0405046, -0.040962, -0.0414255, -0.0418951, -0.042371, -0.0428532, -0.0433418, -0.043837, -0.0443387, -0.0448472, -0.0453625, -0.0458848, -0.046414, -0.0469504, -0.0474941, -0.0480451, -0.0486036, -0.0491696, -0.0497434, -0.050325, -0.0509145, -0.0515121, -0.0521178, -0.0527319, -0.0533544, -0.0539855, -0.0546252, -0.0552738, -0.0559314, -0.0565981, -0.057274, -0.0579594, -0.0586543, -0.0593588, -0.0600732, -0.0607976, -0.0615322, -0.0622771, -0.0630324, -0.0637984, -0.0645752, -0.065363, -0.0661619, -0.0669721, -0.0677939, -0.0686273, -0.0694726, -0.0703299, -0.0711995, -0.0720816, -0.0729763, -0.0738838, -0.0748044, -0.0757383, -0.0766856, -0.0776465, -0.0786214, -0.0796104, -0.0806138, -0.0816317, -0.0826645, -0.0837123, -0.0847754, -0.085854, -0.0869484, -0.0880589, -0.0891857, -0.090329, -0.0914891, -0.0926666, -0.0938611, -0.0950957, -0.0963004, -0.0975486, -0.0988155, -0.100101, -0.101406, -0.10273, -0.104074, -0.105438, -0.106822, -0.108227, -0.109654, -0.111101, -0.11257, -0.114062, -0.115576, -0.117112, -0.118672, -0.120256, -0.121863, -0.123495, -0.125151, -0.126833, -0.12854, -0.130273, -0.132033, -0.13382, -0.135632, -0.137473, -0.139342, -0.141239, -0.143153, -0.145127, -0.14712, -0.14913, -0.15118, -0.153259, -0.155371, -0.157515, -0.159694, -0.1619, -0.164145, -0.166424, -0.168736, -0.171087, -0.173473, -0.175896, -0.178356, -0.180855, -0.183399, -0.185968, -0.188584, -0.19124, -0.193938, -0.196681, -0.199875, -0.202279, -0.205146, -0.208058, -0.211014, -0.214017, -0.217065, -0.22016, -0.223303, -0.226156, -0.228827, -0.233025, -0.236366, -0.239759, -0.243203, -0.2467, -0.250251, -0.253856, -0.257516, -0.261232, -0.265004, -0.268834, -0.272722, -0.276669, -0.280675, -0.284742, -0.288871, -0.293061, -0.297314, -0.301631, -0.306012, -0.310458, -0.314971, -0.31955, -0.324197, -0.328912, -0.333697, -0.338551, -0.343476, -0.348472, -0.353541, -0.358682, -0.363897, -0.369186, -0.374551, -0.379991, -0.385508, -0.391101, -0.396772, -0.402522, -0.40835, -0.414258, -0.420246, -0.426314, -0.432464, -0.438693, -0.445002, -0.451395, -0.457866, -0.464426, -0.472291, -0.477785, -0.484595, -0.492415, -0.498864, -0.505515, -0.513149, -0.520132, -0.527192, -0.534584, -0.542059, -0.549205, -0.55688, -0.564594, -0.57245, -0.580746, -0.588702, -0.59675, -0.604508, -0.612649, -0.620863, -0.629192, -0.637603, -0.645863, -0.654533, -0.663513, -0.672234, -0.679987, -0.688756, -0.697703, -0.706613, -0.715505, -0.724515, -0.733552, -0.742587, -0.751717, -0.760862, -0.770056, -0.779256, -0.788475, -0.797715, -0.806965, -0.816221, -0.825481, -0.834736, -0.843983, -0.853214, -0.862426, -0.871612, -0.880762, -0.889883, -0.898987, -0.908044, -0.916817, -0.925819, -0.934597, -0.943385, -0.952039, -0.960347, -0.969057, -0.977197, -0.985404, -0.993763, -1.00109, -1.00899, -1.01731, -1.02475, -1.03214, -1.03941, -1.04648, -1.05333, -1.05879, -1.06526, -1.07295, -1.07919, -1.08522, -1.091, -1.0966, -1.102, -1.10722, -1.11125, -1.11609, -1.12072, -1.12526, -1.1311, -1.13532, -1.13939, -1.14476, -1.14863, -1.15367, -1.15712, -1.16119, -1.16394, -1.16762, -1.17133, -1.17568, -1.17906, -1.18309, -1.18811, -1.19114, -1.19506, -1.19982, -1.20573, -1.21276, -1.21511, -1.22155, -1.22806, -1.23559, -1.2438, -1.25609, -1.26549, -1.27557, -1.28751, -1.29746, -1.30933, -1.32235, -1.33639, -1.34699, -1.36114, -1.37597, -1.39085, -1.4064, -1.41968, -1.43307, -1.4453, -1.45602, -1.46511, -1.47124, -1.47608, -1.47404, -1.4689, -1.45942, -1.44382, -1.41773, -1.382, -1.33534, -1.2773, -1.20683, -1.11842, -1.00989, -0.884558, -0.731281, -0.553083, -0.345267, -0.104138, 0.173261, 0.490877, 0.856397, 1.26589, 1.72961, 2.25038, 2.83411, 3.48157, 4.20122, 4.9981, 5.8756, 6.83587, 7.88444, 9.02529, 10.2617, 11.5945, 13.0273, 14.5642, 16.2023, 17.9408, 19.7771, 21.7116, 23.7398, 25.8572, 28.0586, 30.3358, 32.6853, 35.0972, 37.5631, 40.0749, 42.621, 45.1928, 47.7805, 50.3745, 52.9637, 55.5425, 58.0968, 60.6257, 63.1188, 65.5688, 67.9704, 70.3182, 72.6066, 74.8342, 76.9967, 79.0928, 81.1217, 83.0791, 84.9636, 86.7797, 88.5233, 90.1944, 91.7979, 93.3323, 94.7999, 96.2011, 97.5362, 98.8077, 100.018, 101.168, 102.258, 103.292, 104.269, 105.193, 106.064, 106.883, 107.654, 108.377, 109.053, 109.684, 110.271, 110.819, 111.323, 111.786, 112.211, 112.597, 112.949, 113.264, 113.542, 113.785, 113.999, 114.182, 114.334, 114.455, 114.546, 114.61, 114.646, 114.656, 114.638, 114.594, 114.527, 114.438, 114.324, 114.189, 114.03, 113.849, 113.646, 113.424, 113.18, 112.918, 112.633, 112.333, 112.012, 111.676, 111.318, 110.942, 110.554, 110.145, 109.718, 109.274, 108.811, 108.335, 107.844, 107.337, 106.81, 106.271, 105.716, 105.146, 104.56, 103.958, 103.344, 102.712, 102.065, 101.401, 100.723, 100.029, 99.3202, 98.5969, 97.855, 97.0954, 96.325, 95.5378, 94.732, 93.912, 93.0738, 92.219, 91.3482, 90.4575, 89.5515, 88.6302, 87.6907, 86.7331, 85.7577, 84.7659, 83.7582, 82.7324, 81.6874, 80.6261, 79.5495, 78.4544, 77.343, 76.2157, 75.0706, 73.908, 72.7329, 71.5433, 70.3384, 69.1182, 67.8873, 66.6441, 65.3897, 64.1239, 62.8501, 61.568, 60.2801, 58.9849, 57.6857, 56.3835, 55.0787, 53.7737, 52.469, 51.1655, 49.8666, 48.5737, 47.2869, 46.0074, 44.7368, 43.4766, 42.2291, 40.9953, 39.776, 38.5715, 37.3843, 36.2153, 35.0653, 33.9353, 32.8255, 31.7377, 30.6721, 29.6294, 28.6099, 27.6143, 26.6427, 25.6956, 24.7731, 23.8754, 23.0034, 22.1553, 21.3319, 20.5332, 19.7589, 19.0089, 18.2828, 17.5803, 16.9001, 16.2432, 15.6093, 14.9974, 14.407, 13.8383, 13.2895, 12.7607, 12.2515, 11.7612, 11.2893, 10.8353, 10.3986, 9.97871, 9.57508, 9.18717, 8.81447, 8.45602, 8.11111, 7.7815};

  // within the radius of the beampipe through the septum, r = 4.128 cm.
  if(sqrt(pow(myLocalPointInMainMagnet[0],2)+pow(myLocalPointInMainMagnet[1],2))<4.128*cm){
    if(abs(myLocalPointInMainMagnet[2])<200*cm){
      dBydx = interpolate(z,b,myLocalPointInMainMagnet[2]/cm)*gauss/cm;
    }
  }
  Bfield[0] += -myLocalPointInMainMagnet[1]*dBydx;
  Bfield[1] += -myLocalPointInMainMagnet[0]*dBydx;
}

G4double MollerGlobalMagnetField::interpolate(std::vector<double> xV,
					      std::vector<double> yV,
					      G4double val) const
{
  if( val <= xV.front() ) return 0;
  else if( val >= xV.back() ) return 0;

  int lowIndex  = int(std::lower_bound(xV.begin(),xV.end(),val) - xV.begin() ) - 1;
  int highIndex = lowIndex + 1;

  double lowVal  = yV[lowIndex];
  double highVal = yV[highIndex];
  double lowX  = xV[lowIndex];
  double highX = xV[highIndex];

  double returnVal = ( lowVal + (val - lowX) * (highVal - lowVal) / (highX - lowX) );
  return returnVal;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerGlobalMagnetField::GetFieldValueCREX(const G4double Point[4], G4double *Bfield ) const
{
  G4double myLocalPointInMainMagnet[4];
  G4double myLocalBfieldInMainMagnet[3];

  G4double dBxdy = 0.*gauss/cm;
  G4double dBydx = 0.*gauss/cm;

  // gives integral 7003.4 G @ 1 cm
  G4double a = magScaleFactor*158.*gauss/cm;
  G4double shielded = .78 *gauss/cm;
  // bu and bd define center of gaussian approximations of fringe field along z-axis
  G4double bu = -69*cm;
  G4double bd = 61*cm;
  // c1 is gaussian width inside the shield
  G4double c1 = 3.40*cm;
  G4double c2 = 6.94*cm;
  // c2 is gaussian width outside the shield
  G4double c_mid = 18.69*cm;

  G4double z_intercept_low = mg_field_low + 11.7*cm;
  G4double z_intercept_high = mg_field_high - 8.5*cm;

  //---------------------------------------------------------------
  // translation from global Point[4] into local magnet coordinates
  //---------------------------------------------------------------
  myLocalPointInMainMagnet[0] = Point[0];             // x-pos
  myLocalPointInMainMagnet[1] = Point[1];             // y-pos
  myLocalPointInMainMagnet[2] = Point[2] - 69.91*cm;  // z-pos
  myLocalPointInMainMagnet[3] = Point[3];             // time

  // Parameterizing the gradient as two gaussians at each end of the septum, defined for different z values
  // within the radius of the beampipe through the septum, r = 4.128 cm.
  if( sqrt(pow(myLocalPointInMainMagnet[0],2)+pow(myLocalPointInMainMagnet[1],2))<4.128*cm){
    if ((myLocalPointInMainMagnet[2]>-100*cm)&&(myLocalPointInMainMagnet[2]<100*cm)){
      if (myLocalPointInMainMagnet[2]<mg_field_low){
        dBxdy = shielded+(0.667)*a*(exp(-((pow(myLocalPointInMainMagnet[2]-bu,2))/(2*pow(c2,2)))));
      } else if ((myLocalPointInMainMagnet[2]>=mg_field_low)&&(myLocalPointInMainMagnet[2]<z_intercept_low)){
        dBxdy = shielded+(0.667)*a*(exp(-(pow(mg_field_low-bu                         ,2)/(2*pow(c2,2)))))*
          (exp(-(pow(myLocalPointInMainMagnet[2]-mg_field_low,2)/(2*pow(c1,2)))));
      } else if ((myLocalPointInMainMagnet[2]>=z_intercept_low)&&(myLocalPointInMainMagnet[2]<=z_intercept_high)){
        dBxdy = shielded+a*(exp(-(pow(myLocalPointInMainMagnet[2],2)/(2*pow(c_mid,2)))));
      } else if ((myLocalPointInMainMagnet[2]<=mg_field_high)&&(myLocalPointInMainMagnet[2]>z_intercept_high)){
        dBxdy = shielded+(1.58)*a*(exp(-(pow(mg_field_high-bd                         ,2)/(2*pow(c2,2)))))*
          (exp(-(pow(myLocalPointInMainMagnet[2]-mg_field_high,2)/(2*pow(c1,2)))));
      } else if ((myLocalPointInMainMagnet[2]>mg_field_high)){
        dBxdy = shielded+(1.58)*a*(exp(-((pow(myLocalPointInMainMagnet[2]-bd,2))/(2*pow(c2,2)))));
      } else {
        dBxdy=0;
      }
      dBydx=dBxdy;
      Bfield[0] = -myLocalPointInMainMagnet[1]*dBxdy;
      Bfield[1] = -myLocalPointInMainMagnet[0]*dBydx;
      Bfield[2] = 0.;
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
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerGlobalMagnetField::WriteMagField()
{
  G4cout << G4endl
         << "###### Printing Septum field on file ######"
         << G4endl << G4endl;
  G4cout << G4endl
         << "Magnetic Shielding low limit= " << mg_field_low
         << "   Magnetic Shielding high limit= " << mg_field_high << G4endl;

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
  double gr_dy = 0.001*cm;
  point2[0] = 1.*cm;
  point2[1] = gr_dy;
  point2[3] = 0.*ns;
  for (int i=0; i<201; i++) {
    z[i] = 175.211-105.3+double(i-100);
    point[2] = z[i] *cm;
    point2[2] = z[i] *cm;
    GetFieldValue(point,g4grB[i]);
    for (int j=0; j<3; j++) grB[i][j]=(double)g4grB[i][j]/gauss;
    GetFieldValue(point2,g4grB[i]);
    for (int j=0; j<3; j++) grB2[i][j]=(double)g4grB[i][j]/gauss;
    gr_dBxdy[i] = (grB2[i][0]- grB[i][0])/(gr_dy/cm);
    G4cout << z[i] << " " << point[2]/cm << " " << gr_dBxdy[i] << G4endl;
  }
  TGraph * graph_Mag_field = new TGraph(201,z,gr_dBxdy);
  graph_Mag_field->SetTitle("Magnetic Field #frac{dBx}{dy} at (-1cm,0cm,z);z(cm);gauss/cm");
  graph_Mag_field->Write();
}
