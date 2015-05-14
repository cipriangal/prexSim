#include "G4ParticleGun.hh"
#include "G4Electron.hh"
#include "G4Event.hh"
#include "globals.hh"

#include "MollerPrimaryGenAction.hh"
#include "RootAnalysis.hh"


// typedef struct {
//   double theta;
//   double y;
//   double xs;
// }cms_cross_section_t;

// typedef struct {
//   double theta;
//   double Eprime;
//   double xs;
// }double_differential_t;

// #define nbinIneEP 100
// double_differential_t DDtable[nbinIneEP][nbinIneEP];

// #define nbin 1000
// cms_cross_section_t XStable[nbin];

#define r0 2.818  //electron radius in fm
#define pi 3.14159
#define me 0.511E-3 //electron restmass (GeV)
#define Euler 0.5772157
#define d2r pi/180.
#define r2d 180/pi

//#define mp 0.9382796 //proton restmass (GeV)
#define gevfm 0.1973
#define pmu 2.79
#define alpha 0.007299

double SigT(double k);

void MollerPrimaryGenAction::GeneratePrimaries_inelasticep_dustin(G4Event* anEvent)
{
  //----------------------
  // GenerateEvent
  //----------------------
  
  double initial_x = 0.0;//*cm;
  double initial_y = 0.0;//*cm;
  double initial_z = 0.0;//*cm;

  double initial_px = 0.0;//*GeV;
  double initial_py = 0.0;//*GeV;
  double initial_pz = 0.0;//*GeV;

  //Find the scattering point in the target
  double vertex; //scattering point in target (cm) measured from the upstream end of target
  double t = 150.0; // t = thickness of target in cm
  double rho = 0.0715; // rho = density of target in g/cm**3 (from KK)
  double radiationLength = 63.04; // g/cm**2 (from PDG)
  double t_rad = radiationLength/rho; // t_rad = radiation length in cm
  double z = G4UniformRand();
  vertex = z*t;//just do this for now
  
  double Evertex = ebeam;
  //Now we know how mush material the beam travels through before moller interaction
  //Use this to calculate energy loss
  double cut=0.000001; //lower limit of bremsstrahlung (1 keV)
  double  Ekin = ebeam - me;
  double fracrl = vertex/t_rad; //total radiation lenght fraction travelled through
  double bt = fracrl*4./3.;

  double prob, prob_sample, sample, eloss, env, value, ref;
  //Calculation of probability to have bremsstrahlung effect above 1 keV
  prob = 1.- pow(cut/Ekin,bt) - bt/(bt+1.)*(1.- pow(cut/Ekin,bt+1.))
    + 0.75*bt/(2.+bt)*(1.- pow(cut/Ekin,bt+2.));
  prob = prob/(1.- bt*Euler + bt*bt/2.*(Euler*Euler+pi*pi/6.)); /* Gamma function */

  prob_sample = G4UniformRand();	/* Random sampling */

  if (prob_sample <= prob) {//Bremsstrahlung has taken place!
    //Generate photon energy with sample and reject using 1/x as envelope 
    do {
      sample = G4UniformRand();
      eloss = cut*pow(Ekin/cut,sample);
      
      env = 1./eloss;
      value = 1./eloss*(1.-eloss/Ekin+0.75*pow(eloss/Ekin,2))*pow(eloss/Ekin,bt);
      
      sample = G4UniformRand();
      ref = value/env;
    } while (sample > ref);
    
    Evertex = ebeam - eloss;
  }

  double vertex_x = (-0.25 + (0.5*(G4UniformRand())));
  double vertex_y = (-0.25 + (0.5*(G4UniformRand())));
  initial_x = vertex_x;
  initial_y = vertex_y;
  initial_z = vertex - t/2.; //cm  
  
  //Make realistic beam angle (Not yet)
  //double theta_div = 0.000125*((double)random()/(double)RAND_MAX);
  //double phi_div = 2.0*M_PI*((double)random()/(double)RAND_MAX);
  ////double theta_div = 0.0;
  ////double phi_div = 0.0;
  ////cout << "theta = "<<theta_div*180/M_PI<<", phi = "<<phi_div*180/M_PI<<endl;
    
  initial_px = 0.0;
  initial_py = 0.0;
  initial_pz = Evertex; // GeV
  //cout <<"------------inelastic ep Event-------------------"<<endl;
  //cout <<" vertex = "<<vertex<<" , Evertex = "<<Evertex<<endl;
    
  /*based in fit to sigma_T and assuming R=sig_L/sig_T=q2 (good to q2=0.2 or so) */

  //Lab System
  double thetaIneEP[2];
  thetaIneEP[0] = 0.1;//0.2578;//degree in lab
  thetaIneEP[1] = 2.0;//0.5157;//degree in lab

  double e = Evertex;
  double eprime_max = e - 0.150;
  double eprime_min = eprime_max/nbinIneEP;
  if(Evertex<eprime_min) G4cout <<"Evertex is less than eprime_min..."<<"\n";
  else if(Evertex<eprime_max) eprime_max = Evertex;

  for(int i=0;i<nbinIneEP;i++) {//loop over theta
    for (int j=0;j<nbinIneEP;j++) {//loop over eprime
      double theta = (thetaIneEP[1] - thetaIneEP[0])/(nbinIneEP-1)*i + thetaIneEP[0];
      double costh = cos(theta*d2r/2.);//Note theta/2 here
      double sinth = sin(theta*d2r/2.);//and here
      double sin2 = sinth*sinth;//(1. - costh)/2.;
      double cos2 = costh*costh;//1. - sin2;
      double eprime = (eprime_max - eprime_min)/(nbinIneEP-1)*j + eprime_min;
      double q2 = 4.*e*eprime*sin2;
      double x = q2/2./mp/(e-eprime);
      double nu = q2/2./mp/x;
      //double wsq = mp*mp + 2.*mp*(e-eprime) - q2;
      double k = nu - q2/2./mp;
      double sigt=SigT(k);/* get sigma_T */
      /* Get F_1 using virtual photon flux */
      double f1 = (nu - q2 / 2. / mp) * mp / 4. / (pi*pi) / alpha * sigt /0.389 ;
      //Note that 0.389 = (hbar*c)^2 in GeV^2 mbarn, a conversion constant
      /* calculate f_2 using R=q2 */
      double r = q2;
      double f2 = f1 / ( (1. + (2. * mp * x) * (2. * mp * x) /q2) / 2. / x / (1. + r )) ; 
      /* sig is in barns/GeV/sr */
      double sig=5.2E-9/e/e/sin2/sin2* (cos2 * f2 / nu + sin2 * 2. * f1 / mp); 
      sig *= sin(theta*d2r);//with phase space factor 
      DDtable[i][j].Eprime = eprime;
      DDtable[i][j].theta = theta;
      DDtable[i][j].xs = sig;
    }
  }

  double_differential_t XStable_theta[nbinIneEP];
  double_differential_t XStable_Eprime[nbinIneEP];
  //---- Convert cross section table to integral fractions ----
  // First, numerically integrate
  double Im=0.0;
  for(int i = 0;i<nbinIneEP;i++) {
    XStable_theta[i].xs = 0.0;
    XStable_theta[i].theta = (thetaIneEP[1] - thetaIneEP[0])/(nbinIneEP-1)*i + thetaIneEP[0];
    for(int j = 0;j<nbinIneEP;j++) {XStable_theta[i].xs += DDtable[i][j].xs;}//Integrate over Eprime
    Im += XStable_theta[i].xs;
    XStable_theta[i].xs = Im;
  }
  double totXS = Im*(thetaIneEP[1]-thetaIneEP[0])*d2r*(eprime_max - eprime_min)/nbinIneEP/nbinIneEP*2*pi;
  double factor = 6.0221415*0.1*1.0E-6/1.602176487E-19;
  double rate = totXS*t*rho/1.008*1.0*factor; //Event rate/sec/ uA
  gRootAnalysis->SetTotXS(totXS);
  gRootAnalysis->SetRate(rate);

  // Next, normalize tables
  for(int k=0;k<nbinIneEP;k++){
    XStable_theta[k].xs /= Im;
  }
  // Determine scattering angle using ss
  double ss = (G4UniformRand());
  int index;
  double slope, b;
  for(index=0;index<nbinIneEP-1;index++) if(ss <= XStable_theta[index].xs) break;
  slope = (XStable_theta[index+1].xs - XStable_theta[index].xs)/(XStable_theta[index+1].theta - XStable_theta[index].theta);
  b = XStable_theta[index].xs;
  double lab_theta = XStable_theta[index].theta + (ss - b)/slope;
  int theIndex = 0;
  double bestDiff = 999.0;
  //Find the array index corresponding to this angle
  for(int f=0;f<nbinIneEP;f++) {
    double theta_my = thetaIneEP[0] + f*(thetaIneEP[1]-thetaIneEP[0])/nbinIneEP;
    if(fabs(lab_theta-theta_my) < bestDiff) {
      bestDiff = fabs(lab_theta-theta_my);
      theIndex = f;
    }
  } 

  //Now do the Eprime 
  //For this, simply use the array of the selected theta
  double Im2=0.0;
  for(int j = 0;j<nbinIneEP;j++) {
    Im2 += DDtable[theIndex][j].xs;
    XStable_Eprime[j].xs = Im2;
    XStable_Eprime[j].Eprime = (eprime_max - eprime_min)/(nbinIneEP-1)*j + eprime_min;
  }
  //double_differential_t *ineEP_Eprime = XStable_Eprime;
  // Next, normalize tables
  for(int k=0;k<nbinIneEP;k++){
    XStable_Eprime[k].xs /= Im2;
  }
  // Determine Eprime using ss2
  double ss2 = (G4UniformRand());
  int index2;
  double slope2, b2;
  for(index2=0;index2<nbinIneEP-1;index2++) if(ss2 <= XStable_Eprime[index2].xs) break;
  slope2 = (XStable_Eprime[index2+1].xs - XStable_Eprime[index2].xs)/(XStable_Eprime[index2+1].Eprime - XStable_Eprime[index2].Eprime);
  b2 = XStable_Eprime[index2].xs;
  double lab_Eprime = XStable_Eprime[index2].Eprime + (ss2 - b2)/slope2;

  //Now do phi
  double lab_phi = (2.0*pi*(G4UniformRand())) - pi;
    
  //Now add unknown beam divergence
  //pi0_theta += theta_div;
  //pi0_phi += phi_div;

  // Define kinematic conditions in lab
  double costh = cos(lab_theta*d2r);
  double sinth = sin(lab_theta*d2r);
  e = lab_Eprime;
  double p = sqrt(e*e - me*me);
  //double ep = Evertex + mp - e;
  double cosphie = cos( lab_phi );
  double sinphie = sin( lab_phi );
  //double cosphip = cos( lab_phi - pi);
  //double sinphip = sin( lab_phi - pi);

  //G4cout <<"Evertex + mp = "<<(Evertex+mp)<<", theta = "<<lab_theta<<", p = "<<p<<", e = "<<e<<", pp = "<<pp<<", ep = "<<ep<<", e+ep = "<<(e+ep)<<"\n";
  //G4cout <<"Evertex = "<<(Evertex)<<", theta = "<<lab_theta<<", p = "<<p<<", e = "<<e<<"\n";

  double e1, p1[3];
  e1 = e;
  for(int i=0;i<3;i++) {
    p1[i] = 0.0;
  }
    
  p1[2] = p*costh;
  p1[0] = p*sinth*cosphie;
  p1[1] = p*sinth*sinphie;
    
  double pmag1 = p;//sqrt(p1[0]*p1[0]+p1[1]*p1[1]+p1[2]*p1[2]);
  double pmag0 = sqrt(initial_px*initial_px+initial_py*initial_py+initial_pz*initial_pz);
    
  // ------------ Final state particles ---------------
    
  //G4int n_particle = 1;
  //particleGun = new G4ParticleGun(n_particle);
    
  G4ParticleDefinition* particle_e = G4Electron::ElectronDefinition();
  //We will only throw the electron for now    
  particleGun->SetParticleDefinition(particle_e);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1));
  particleGun->SetParticleEnergy(e1*1000);
  particleGun->SetParticlePosition(G4ThreeVector(initial_x*10, initial_y*10, initial_z*10));
  particleGun->GeneratePrimaryVertex(anEvent);
    
  gRootAnalysis->SetMomentum0(Evertex*1000,initial_px/pmag0,initial_py/pmag0,initial_pz/pmag0);
  gRootAnalysis->SetMomentum1(e1*1000,p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1);

  //anEvent->SetEventID(0);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);
    
}


double SigT(double k) {
  double sigt ; 
  sigt=0 ;
  /* histogram-type fit to world data from PDG over Delta, S11, tc.*/
  if(k > 0.175) sigt=0.08 ;
  if(k > 0.196) sigt=0.12 ;
  if(k > 0.214) sigt=0.17 ;
  if(k > 0.233) sigt=0.21 ;
  if(k > 0.251) sigt=0.32 ;
  if(k > 0.251) sigt=0.32 ;
  if(k > 0.251) sigt=0.32 ;
  if(k > 0.271) sigt=0.40 ;
  if(k > 0.290) sigt=0.50 ;
  if(k > 0.310) sigt=0.53 ;
  if(k > 0.330) sigt=0.50 ;
  if(k > 0.350) sigt=0.45 ;
  if(k > 0.370) sigt=0.41 ;
  if(k > 0.390) sigt=0.32 ;
  if(k > 0.410) sigt=0.24 ;
  if(k > 0.430) sigt=0.22 ;
  if(k > 0.450) sigt=0.18 ;
  if(k > 0.470) sigt=0.17 ;
  if(k > 0.520) sigt=0.18 ;
  if(k > 0.550) sigt=0.21 ;
  if(k > 0.600) sigt=0.22 ;
  if(k > 0.650) sigt=0.24 ;
  if(k > 0.700) sigt=0.27 ;
  if(k > 0.750) sigt=0.25 ;
  if(k > 0.800) sigt=0.25 ;
  if(k > 0.750) sigt=0.25 ;
  if(k > 0.800) sigt=0.22 ;
  if(k > 0.900) sigt=0.21 ;
  if(k > 0.950) sigt=0.215;
  if(k > 1.000) sigt=0.225;
  if(k > 1.050) sigt=0.215;
  if(k > 1.100) sigt=0.185;
  if(k > 1.150) sigt=0.165;
  if(k > 1.200) sigt=0.15 ;
  if(k > 1.300) sigt=0.16 ;
  if(k > 1.400) sigt=0.15 ;
  /* use fit to sigt (in mb) from Caldwell */
  if(k > 1.5) sigt=0.0987 + 0.0649/sqrt(k) ;
  return sigt ;
}      

