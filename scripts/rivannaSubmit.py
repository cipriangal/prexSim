#!/usr/bin/python
from subprocess import call
import sys,os,time

import config_reader as cr

def main():

    ### always check that these are the options you want
    uva_id    = cr.uva_id()
    sourceDir = "/scratch/" + uva_id + "/prexSim"
    outputDir = "/scratch/" + uva_id + "/prexSim/output"
    email     = uva_id + "@virginia.edu"
    submit=1

    ##only crex5, prexII defined for now
    #configuration = "crex5"
    configuration = cr.config()
    nrEv   = cr.nr_events()
    nrStart= cr.start_run()
    nrStop = cr.end_run()
    identifier = cr.identifier()

    print('Running ' + str(nrEv*(nrStop - nrStart)) + ' events...')

    jobName=configuration+'_%03dkEv'%(nrEv/1000)

    for nr in range(nrStart,nrStop): # repeat for nr jobs
        print("Starting job setup for jobID: " + str(nr))

        jobFullName = jobName + '_%04d'%nr + '_' + identifier
        createMacFiles(configuration, outputDir+"/"+jobFullName, sourceDir, nrEv, nr, identifier)

        ###copy executable
        call(["cp",sourceDir+"/build/prexsim",
              outputDir+"/"+jobFullName+"/prexsim"])

        createBashScript(outputDir+"/"+jobFullName, email)

        ###submit job
        if submit==1:
            print "submitting ",jobFullName
            call(["sbatch",outputDir+"/"+jobFullName+"/myScript.sh"])
        else:
            print "Not submitting",jobFullName

    print "All done"


def createMacFiles(config,outDir,sourceDir,nrEv,jobNr,identifier):

    if not os.path.exists(outDir+"/geometry"):
        os.makedirs(outDir+"/geometry")

    call(["cp",sourceDir+"/geometry/materials.xml",outDir+"/geometry/"])
    call(["cp",sourceDir+"/geometry/kriptoniteDetectors.gdml",outDir+"/geometry/"])
    call(["cp",sourceDir+"/geometry/plasticDetectors.gdml",outDir+"/geometry/"])
    call(["cp","-r",sourceDir+"/geometry/schema",outDir+"/geometry"])
    if config=="crex5":
        call(["cp",sourceDir+"/geometry/crex5deg" + '_' + identifier + ".gdml",outDir+"/geometry"])
    elif config == 'prexII':
    	call(['cp',sourceDir + "/geometry/prex5deg" + '_' + identifier + ".gdml",outDir+"/geometry"])

    f=open(outDir+"/"+"/myRun.mac",'w')
    f.write("/moller/ana/rootfilename ./o_prexSim\n")
    f.write("/run/beamOn "+str(nrEv)+"\n")
    f.close()

    f=open(outDir+"/"+"/preRun.mac",'w')
    f.write("/moller/det/readGeometryFromFile true\n")
    f.write("/gun/particle e-\n")
    f.write("/moller/gun/gen 7\n")
    seedA=long(time.time()+jobNr)
    seedB=long(time.time()*100+jobNr)
    f.write("/random/setSeeds "+str(seedA)+" "+str(seedB)+"\n")

    if config=="crex5":
        f.write("/gun/energy 2. GeV\n")
        f.write("/moller/field/setConfiguration crex\n")
        f.write("/moller/det/setDetectorFileName geometry/crex5deg_" + identifier + ".gdml\n")
    elif config=="prexII":
    	f.write("/gun/energy 1.05 GeV\n")
        f.write("/moller/field/setConfiguration prex2\n")
        f.write("/moller/det/setDetectorFileName geometry/prex5deg_" + identifier + ".gdml\n")

    f.write("/moller/field/useQ1fringeField true\n")
    f.write("/moller/det/setShieldMaterial polyethylene\n")
    f.write("/testhadr/CutsAll 0.7 mm\n")
    f.write("/run/initialize\n")
    f.close()

    return 0

def createBashScript(outDir, email):
    f=open(outDir+"/"+"/myScript.sh",'w')
    f.write("#!/bin/bash\n")
    f.write("#SBATCH --ntasks=1\n")
    f.write("#SBATCH --ntasks-per-node=1\n")
    f.write("#SBATCH --time=02:00:00\n")
    f.write("#SBATCH --output=" + outDir + "/log.out\n")
    f.write("#SBATCH --error=" + outDir + "/log.err\n")
    #f.write("#SBATCH --mail-type=ALL\n")
    #f.write("#SBATCH --mail-user=" + email + "\n")
    f.write("#SBATCH --partition=serial\n")
    f.write("cd " + outDir + "\n")
    f.write("./prexsim preRun.mac myRun.mac\n")
    f.close()
    return 0

if __name__ == '__main__':
    main()

