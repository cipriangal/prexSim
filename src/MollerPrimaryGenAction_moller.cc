#include "G4ParticleGun.hh"
#include "G4Electron.hh"
#include "G4Event.hh"
#include "globals.hh"

#include "MollerPrimaryGenAction.hh"
#include "RootAnalysis.hh"
#include <cassert>

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
#define Euler 0.5772157
#define d2r pi/180.
#define r2d 180/pi
#define me 0.511E-3 //electron restmass (GeV)

#define gevfm 0.1973
#define pmu 2.79
#define alpha 0.007299

void MollerPrimaryGenAction::GeneratePrimaries_moller_dustin(G4Event* anEvent)
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
  double t = 40.0; // t = thickness of target in cm (length of SoLID PVDIS target)
  double rho = 0.162; // rho = density of target in g/cm**3 (density of Liquid Deuterium)
  double radiationLength = 122.4; // g/cm**2 (from PDG) (liquid deuterim)
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

  if( Evertex < me ) return;

  double vertex_x = (-0.25 + (0.5*(G4UniformRand())));
  double vertex_y = (-0.25 + (0.5*(G4UniformRand())));
  initial_x = vertex_x;
  initial_y = vertex_y;
  initial_z = vertex - t/2. +10.; //cm (Solid +10 cm)  
  
  //Make realistic beam angle (Not yet)
  //double theta_div = 0.000125*((double)random()/(double)RAND_MAX);
  //double phi_div = 2.0*M_PI*((double)random()/(double)RAND_MAX);
  ////double theta_div = 0.0;
  ////double phi_div = 0.0;
  ////cout << "theta = "<<theta_div*180/M_PI<<", phi = "<<phi_div*180/M_PI<<endl;
    
  initial_px = 0.0;
  initial_py = 0.0;
  initial_pz = Evertex; // GeV
  
  //cout <<"-------------Moller Event-------------------"<<endl;
  //cout <<" vertex = "<<vertex<<" , Evertex = "<<Evertex<<endl;
        
  //Center of Mass System
  double thetaMoller[2];
  thetaMoller[0] = 53.13010;//costheta_cm = +0.6
  thetaMoller[1] = 126.8699;//costheta_cm = -0.6
  double x1 = cos(thetaMoller[1]*d2r);
  double x2 = cos(thetaMoller[0]*d2r);
  double y2 = (1 - x1)/2.;
  double y1 = (1 - x2)/2.;
  double ds = me*me+me*me+2.*me*Evertex;
  double ecm = sqrt(ds);
  double decm[2];
  decm[0] = ds/2./ecm;
  double dps = pow(decm[0],2)-me*me;
  double dp = sqrt(dps);
  decm[1] = sqrt(dps+me*me);

  for(int i =0;i<nbin;i++) {
    double y = y1 + ((y2 - y1)/nbin*i)+(y2 - y1)/nbin/2.;
    double dt = -1.*y*ds;
    double ct = (dt-me*me-me*me+2.*pow(decm[0],2))/(2.*pow(dp,2));
    if( !( -1.0 < ct && ct < 1.0 )){ 
	// SPR 9/20/12
	// Something's wrong here for very low beam energies
	// Not sure how to handle it, but at least this point
	// is kinematically forbidden
	return;
    }
    double ct2 = pow(ct,2);
    double st2 = 1. - ct2;
    double st = sqrt(st2);
    double th = acos(ct);
      
    double ecmp = ecm/2.;
    double vcmp = sqrt(pow(ecmp,2)-me*me)/ecmp;
    double dsig = 0.;
      
    double eom = ecmp/me;
    double eom2 = pow(eom,2);
    double d1 = pow(r0,2)*pow((2.*eom2-1.),2)/4./pow(vcmp,4)/pow(eom2,3);
    double d2 = 4./pow(st2,2)-3./st2+pow((eom2-1.),2)/pow((2.*eom2-1),2)*(1+4./pow(st,2));
      
    dsig = d1*d2;

    assert( dsig > 0.0 );

    dsig = dsig*2.; //Convert from dsigma/dOmega to dsigma/dy/dphi
    dsig = dsig/100; //Convert from fm**2 to Barns
    XStable[i].y = y;
    XStable[i].theta = th;
    XStable[i].xs = dsig;
  }

  cms_cross_section_t *moller = XStable;
    
  //---- Convert cross section table to integral fractions ----
  // First, numerically integrate
  double Im=0.0;
  double save = moller[0].xs;
  moller[0].xs = Im;

  for(int j = 1;j<nbin;j++) {
      Im += (save + moller[j].xs)/2.0;
      save = moller[j].xs;
      moller[j].xs = Im;
  }

  double totXS = Im*(y2-y1)/(nbin-1.0)*2*pi;
  double factor = 6.0221415*0.1*1.0E-6/1.602176487E-19; // 0.1 is barn (10^-24 cm2) times avogadro number  (10^23)
  double rate = totXS*t*rho/2.014*1.0*factor; //Event rate/sec/ uA (atomic weight of deuterium is 2.014)
  gRootAnalysis->SetTotXS(totXS);
  gRootAnalysis->SetRate(rate);

  if( !(Im > 0.0) ){ 
      printf("Im = %f, Evertex = %f\n", Im, Evertex);
  }
  assert( Im > 0.0 ); // idiot check

  // Next, normalize tables
  moller = XStable;
  for(int k=0;k<nbin;k++){
    moller[k].xs /= Im;
  }
    
  // Determine moller cms scattering angle using ss
  double ss = (G4UniformRand());
    
  //cms_cross_section_t *c=XStable;
  int index;
  double slope, b;
  for(index=0;index<nbin-1;index++) if(ss <= moller[index+1].xs) break;
  assert( index < nbin-1 ); // idiot check
  slope = (moller[index+1].xs - moller[index].xs)/(moller[index+1].theta - moller[index].theta);
  b = moller[index].xs;
    
  double cms_theta = moller[index].theta + (ss - b)/slope;

  double cms_phi = (2.0*pi*(G4UniformRand())) - pi;
    
  //Now add unknown beam divergence
  //pi0_theta += theta_div;
  //pi0_phi += phi_div;
        
  // Define kinematic conditions in cms
  double gammap = Evertex/ecm;
  double betap = 1/gammap*sqrt(pow(gammap,2)-1);
    
  double e1cm = gammap*me;
  double e2cm = e1cm;
    
  //Primary (beam) electron
  double costheta1 = cos( cms_theta );
  double sintheta1 = sin( cms_theta );
    
  double cosphi1 = cos( cms_phi );
  double sinphi1 = sin( cms_phi );
    
  double p1cm[3];


  p1cm[0] = e1cm * sintheta1 * cosphi1;
  p1cm[1] = e1cm * sintheta1 * sinphi1;
  p1cm[2] = e1cm * costheta1;
    
  //Secondary (target) electron
  double costheta2 = cos( pi - cms_theta );
  double sintheta2 = sin( pi - cms_theta );
    
  double cosphi2 = cos( cms_phi - pi);
  double sinphi2 = sin( cms_phi - pi);
  if (cms_phi<0.0){
    cosphi2 = cos( cms_phi + pi);
    sinphi2 = sin( cms_phi + pi);
  } else {
      
  }
    
  double p2cm[3];  
  p2cm[0] = e2cm * sintheta2 * cosphi2;
  p2cm[1] = e2cm * sintheta2 * sinphi2;
  p2cm[2] = e2cm * costheta2;
    
    
  // Transform to the lab frame.
  double e1, p1[3], e2, p2[3];
  e1 = 0.0;
  e2 = 0.0;
  for(int i=0;i<3;i++) {
    p1[i] = 0.0;
    p2[i] = 0.0;
  }
    
  p1[2] = gammap * (p1cm[2] + betap*e1cm);
  p2[2] = gammap * (p2cm[2] + betap*e2cm);
  p1[0] = p1cm[0];
  p1[1] = p1cm[1];
  p2[0] = p2cm[0];
  p2[1] = p2cm[1];
    
  double pmag1 = sqrt(p1[0]*p1[0]+p1[1]*p1[1]+p1[2]*p1[2]);
  double pmag2 = sqrt(p2[0]*p2[0]+p2[1]*p2[1]+p2[2]*p2[2]);
  double pmag0 = sqrt(initial_px*initial_px+initial_py*initial_py+initial_pz*initial_pz);
    
  e1 = gammap * ( e1cm + betap*p1cm[2] );
  e2 = gammap * ( e2cm + betap*p2cm[2] );
    
  // Calculate the laboratory polar angles for moller electrons.  
  //double th1 = acos( p1[2] / e1 );
  //double ph1 = atan2( p1[1], p1[0] );
    
  //double th2 = acos( p2[2] / e2 );
  //double ph2 = atan2( p2[1], p2[0] );
    
    
  // ------------ Final state electrons ---------------
    
  //G4int n_particle = 1;
  //particleGun = new G4ParticleGun(n_particle);
  // G4cout << "totXS= " << totXS << G4endl;   
  G4ParticleDefinition* particle = G4Electron::ElectronDefinition();

  assert( pmag1 > 0.0 && pmag2 > 0.0 && pmag0 > 0.0 );

  assert( e1 > 0.0 && e2 > 0.0 );
    
  if(e1>=e2) {
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1));
    particleGun->SetParticleEnergy(e1*1000);
    particleGun->SetParticlePosition(G4ThreeVector(initial_x*10, initial_y*10, initial_z*10));
    particleGun->GeneratePrimaryVertex(anEvent);
      
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(p2[0]/pmag2,p2[1]/pmag2,p2[2]/pmag2));
    particleGun->SetParticleEnergy(e2*1000);
    particleGun->SetParticlePosition(G4ThreeVector(initial_x*10, initial_y*10, initial_z*10));
    particleGun->GeneratePrimaryVertex(anEvent);
      
    gRootAnalysis->SetMomentum0(Evertex*1000,initial_px/pmag0,initial_py/pmag0,initial_pz/pmag0);
    gRootAnalysis->SetMomentum1(e1*1000,p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1);
    gRootAnalysis->SetMomentum2(e2*1000,p2[0]/pmag2,p2[1]/pmag2,p2[2]/pmag2);
  } else {
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(p2[0]/pmag2,p2[1]/pmag2,p2[2]/pmag2));
    particleGun->SetParticleEnergy(e2*1000);
    particleGun->SetParticlePosition(G4ThreeVector(initial_x*10, initial_y*10, initial_z*10));
    particleGun->GeneratePrimaryVertex(anEvent);
      
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1));
    particleGun->SetParticleEnergy(e1*1000);
    particleGun->SetParticlePosition(G4ThreeVector(initial_x*10, initial_y*10, initial_z*10));
    particleGun->GeneratePrimaryVertex(anEvent);    
      
    gRootAnalysis->SetMomentum0(Evertex*1000,initial_px/pmag0,initial_py/pmag0,initial_pz/pmag0);
    gRootAnalysis->SetMomentum2(e1*1000,p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1);
    gRootAnalysis->SetMomentum1(e2*1000,p2[0]/pmag2,p2[1]/pmag2,p2[2]/pmag2);
  }
  //anEvent->SetEventID(0);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);

}
