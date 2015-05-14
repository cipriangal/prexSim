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

void MollerPrimaryGenAction::GeneratePrimaries_elasticep_dustin(G4Event* anEvent)
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
  double tt = 150.0; // t = thickness of target in cm
  double rho = 0.0715; // rho = density of target in g/cm**3 (from KK)
  double radiationLength = 63.04; // g/cm**2 (from PDG)
  double t_rad = radiationLength/rho; // t_rad = radiation length in cm
  double z = G4UniformRand();
  vertex = z*tt;//just do this for now
  
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
  initial_z = vertex - tt/2.; //cm  
  
  //Make realistic beam angle (Not yet)
  //double theta_div = 0.000125*((double)random()/(double)RAND_MAX);
  //double phi_div = 2.0*M_PI*((double)random()/(double)RAND_MAX);
  ////double theta_div = 0.0;
  ////double phi_div = 0.0;
  ////cout << "theta = "<<theta_div*180/M_PI<<", phi = "<<phi_div*180/M_PI<<endl;
    
  initial_px = 0.0;
  initial_py = 0.0;
  initial_pz = Evertex; // GeV
  //cout <<"-------------elastic ep Event-------------------"<<endl;
  //cout <<" vertex = "<<vertex<<" , Evertex = "<<Evertex<<endl;

  //Lab System
  double thetaMott[2];
  thetaMott[0] = 0.1;//degree in lab
  thetaMott[1] = 2.0;//degree in lab
    
  double p0 = sqrt(Evertex*Evertex - me*me);

  for(int i =0;i<nbin;i++) {
    double theta = thetaMott[0] + i*(thetaMott[1]-thetaMott[0])/nbin;
    double costh = cos(theta*d2r);
    double sinth = sin(theta*d2r);
    double sin2 = (1. - costh)/2.;
    double cos2 = 1. - sin2;
    double p = mp*Evertex/(Evertex*(1. - costh) + mp);
    double e = sqrt(p*p+me*me);
    double q2 = -2.*(me*me - Evertex*e + p0*p*costh);
    double t = q2/4./(mp*mp);
    double ge = 1./pow((1. + q2/0.71),2);
    double gm = ge*pmu;
    double cang = (ge*ge + t*gm*gm)/(1. + t) + t*t*gm*gm*sin2/cos2;
    double crmott = alpha*alpha*cos2/4./(p0*p0)/(sin2*sin2)/(1. + 2.*p0/mp*sin2)*gevfm*gevfm/100.;
    double cross = crmott*cang;
    double csmott = cross*sinth;
    XStable[i].y = theta;
    XStable[i].theta = theta;
    XStable[i].xs = csmott;
  }

  cms_cross_section_t *mott = XStable;
    
  //---- Convert cross section table to integral fractions ----
  // First, numerically integrate
  double Im=0.0;
  for(int j = 0;j<nbin;j++) {
    Im += mott[j].xs;
    mott[j].xs = Im;
  }
  // Next, normalize tables
  mott = XStable;
  for(int k=0;k<nbin;k++){
    mott[k].xs /= Im;
  }
  double totXS = Im*(thetaMott[1]-thetaMott[0])*d2r/nbin*2*pi;
  double factor = 6.0221415*0.1*1.0E-6/1.602176487E-19;
  double rate = totXS*tt*rho/1.008*1.0*factor; //Event rate/sec/ uA
  gRootAnalysis->SetTotXS(totXS);
  gRootAnalysis->SetRate(rate);
    
  // Determine mott cms scattering angle using ss
  double ss = (G4UniformRand());
    
  //cms_cross_section_t *c=XStable;
  int index;
  double slope, b;
  for(index=0;index<nbin-1;index++) if(ss <= mott[index].xs) break;
  slope = (mott[index+1].xs - mott[index].xs)/(mott[index+1].y - mott[index].y);
  b = mott[index].xs;
    
  double lab_theta = mott[index].theta + (ss - b)/slope;
  double lab_phi = (2.0*pi*(G4UniformRand())) - pi;
    
  //Now add unknown beam divergence
  //pi0_theta += theta_div;
  //pi0_phi += phi_div;
        
  // Define kinematic conditions in lab
  double costh = cos(lab_theta*d2r);
  double sinth = sin(lab_theta*d2r);
  double p = mp*Evertex/(Evertex*(1. - costh) + mp);    
  double e = sqrt(p*p + me*me);
  double ep = Evertex + mp - e;
  //double ep_alt = sqrt(mp*mp+p0*p0+p*p-2*p0*p*costh);
  double pp = sqrt(ep*ep - mp*mp);
  double sinp = p*sinth/pp;
  //double thetp = asin(sinp);
  double cosp = sqrt(1.- sinp*sinp);
  double cosphie = cos( lab_phi );
  double sinphie = sin( lab_phi );
  double cosphip = cos( lab_phi - pi);
  double sinphip = sin( lab_phi - pi);

  //G4cout <<"Evertex + mp = "<<(Evertex+mp)<<", theta = "<<lab_theta<<", p = "<<p<<", e = "<<e<<", pp = "<<pp<<", ep = "<<ep<<", e+ep = "<<(e+ep)<<"\n";

  double e1, p1[3], e2, p2[3];
  e1 = e;
  e2 = ep;
  for(int i=0;i<3;i++) {
    p1[i] = 0.0;
    p2[i] = 0.0;
  }
    
  p1[2] = p*costh;
  p2[2] = pp*cosp;
  p1[0] = p*sinth*cosphie;
  p1[1] = p*sinth*sinphie;
  p2[0] = pp*sinp*cosphip;
  p2[1] = pp*sinp*sinphip;
    
  double pmag1 = p;//sqrt(p1[0]*p1[0]+p1[1]*p1[1]+p1[2]*p1[2]);
  //double pmag2 = pp;//sqrt(p2[0]*p2[0]+p2[1]*p2[1]+p2[2]*p2[2]);
  double pmag0 = sqrt(initial_px*initial_px+initial_py*initial_py+initial_pz*initial_pz);
    
  // ------------ Final state particles ---------------
    
  //G4int n_particle = 1;
  //particleGun = new G4ParticleGun(n_particle);
    
  G4ParticleDefinition* particle = G4Electron::ElectronDefinition();
  //We will only throw the electron for now

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1));
  particleGun->SetParticleEnergy(e1*1000);
  particleGun->SetParticlePosition(G4ThreeVector(initial_x*10, initial_y*10, initial_z*10));
  particleGun->GeneratePrimaryVertex(anEvent);

  gRootAnalysis->SetMomentum0(Evertex*1000,initial_px/pmag0,initial_py/pmag0,initial_pz/pmag0);
  gRootAnalysis->SetMomentum1(e1*1000,p1[0]/pmag1,p1[1]/pmag1,p1[2]/pmag1);

  //anEvent->SetEventID(0);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);

}
