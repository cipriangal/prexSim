#include "G4Event.hh"
#include "G4ParticleGun.hh"
//#include "G4ParticleTable.hh"
//#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "globals.hh"
//#include "Randomize.hh"

#include "MollerPrimaryGenAction.hh"
#include "RootAnalysis.hh"


void MollerPrimaryGenAction::GeneratePrimaries_phasespace_mollers(G4Event* anEvent)
{
	//This generator throws events in phase space

	G4int numsectors = 7; 
	G4double sectorhalfopeningangle = pi/numsectors/2.;
	G4double etheta=0., ephi=0., zpos=0.;

	G4int numthetas=17, numphis=13, numzpos=2, numphicentre=7;
	G4double thetamin=0.004, thetamax=0.020;
	G4double zposmin=-0.75, zposmax=0.75;
	G4double phicentre=0;
	G4double phimin=0, phimax=2*pi;
	G4double phicentremin=0, phicentremax=12./7*pi;

	int thetanum, ephinum, zposnum, phicentrenum;
	double thetafrac, ephifrac, zposfrac, phicentrefrac;
	// step through theta then phi then target position z then sector   
	thetanum = eventnumber%(numthetas);  // % gives remainder after division
	ephinum = (eventnumber/(numthetas))%(numphis);
	zposnum = (eventnumber/(numthetas*numphis))%(numzpos);
	phicentrenum = (eventnumber/(numthetas*numphis*numzpos))%(numphicentre);

	thetafrac = ((double)thetanum)/(numthetas-1);
	ephifrac = ((double)ephinum)/(numphis-1);
	zposfrac = ((double)zposnum)/(numzpos-1);
	phicentrefrac = ((double)phicentrenum)/(numphicentre-1);

	etheta = thetamin + thetafrac*(thetamax-thetamin);
	phicentre = phicentremin + phicentrefrac*(phicentremax-phicentremin);
	phimin = phicentre - sectorhalfopeningangle*0.999;
	phimax = phicentre + sectorhalfopeningangle*0.999;
	ephi = phimin + ephifrac*(phimax-phimin);
	zpos = zposmin + zposfrac*(zposmax-zposmin);

	G4double eenergy;
	eenergy = (2*ebeam*0.000511)/(ebeam*(etheta*etheta) + 2*0.000511); // Moller electrons

	G4double xdir = sin(etheta)*cos(ephi);
	G4double ydir = sin(etheta)*sin(ephi);
	G4double zdir = cos(etheta);
	printf("Primaries:%5i phi:%2i %.2f zpos:%2i %.2f; phicentre:%2i %.2f; theta:%2i %.2f =%.3f deg =%4.1f mrad  eenergy=%.3f GeV  phi=%6.1f deg, xdir=%.3f, ydir=%.3f, zdir=%.3f\n", 
		   eventnumber, ephinum, ephifrac, zposnum, zposfrac, phicentrenum, phicentrefrac, 
		   thetanum, thetafrac, etheta/degree, etheta/rad*1000, eenergy, ephi/degree, xdir, ydir, zdir);
  
	particleGun->SetParticleEnergy(eenergy*GeV);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(xdir,ydir,zdir));
	particleGun->SetParticlePosition(G4ThreeVector(0.*cm, 0.*cm, zpos*m));
	particleGun->GeneratePrimaryVertex(anEvent);
	eventnumber++;

	gRootAnalysis->SetMomentum0(11000,0,0,11000);
	gRootAnalysis->SetMomentum2(eenergy*MeV,eenergy*MeV*xdir,eenergy*MeV*ydir,eenergy*MeV*zdir);
	gRootAnalysis->SetMomentum1(eenergy*MeV,eenergy*MeV*xdir,eenergy*MeV*ydir,eenergy*MeV*zdir);
	gRootAnalysis->SetProcess(1);

	if (eventnumber==(numphis*numthetas*numzpos*7)) {
		printf("\nFully completed one set of optics rays.\n\n");
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
