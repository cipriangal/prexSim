#!/usr/bin/python
from subprocess import call
import sys,os,time

def main():

    ### always check that these are the options you want
    sourceDir="/home/cg8fd/prex/prexSim"
    outputDir="/home/cg8fd/prex/prexSim/output"
    email="cg8fd@virginia.edu"
    submit=1

    ##only crex5 defined for now
    configuration="crex5"
    nrEv=100
    nrStart=0
    nrStop =5

    jobName=configuration+'_%03dkEv'%(nrEv/1000)
    
    for nr in range(nrStart,nrStop): # repeat for nr jobs
        print "Starting job setup for jobID:",nr
        
        jobFullName=jobName+'_%04d'%nr
        createMacFiles(configuration,outputDir+"/"+jobFullName,sourceDir,nrEv,nr)

        ###copy executable
        call(["cp",sourceDir+"/build/prexsim",
              outputDir+"/"+jobFullName+"/prexsim"])              

        createBashScript(outputDir+"/"+jobFullName)

        ###submit job
        if submit==1:
            print "submitting ",jobFullName
            call(["sbatch",outputDir+"/"+jobFullName+"/myScript.sh"])
        else:
            print "Not submitting",jobFullName

    print "All done"

    
def createMacFiles(config,outDir,sourceDir,nrEv,jobNr):

    if not os.path.exists(outDir+"/geometry"):
        os.makedirs(outDir+"/geometry")

    call(["cp",sourceDir+"/geometry/materials.xml",outDir+"/geometry/"])
    call(["cp",sourceDir+"/geometry/kriptoniteDetectors.gdml",outDir+"/geometry/"])
    call(["cp","-r",sourceDir+"/geometry/schema",outDir+"/geometry"])    
    if config=="crex5":
        call(["cp",sourceDir+"/geometry/crex5deg.gdml",outDir+"/geometry"])
    
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
        f.write("/gun/energy 2.2 GeV\n")
        f.write("/moller/field/setFieldScaleFactor 2.35\n")
        f.write("/moller/field/setLowLim -74 cm\n")
        f.write("/moller/field/setHighLim 74 cm\n")
        f.write("/moller/det/setDetectorFileName geometry/crex5deg.gdml\n")
    
    f.write("/moller/det/setShieldMaterial polyethylene\n")
    f.write("/testhadr/CutsAll 0.7 mm\n")
    f.write("/run/initialize\n")
    f.close()

    return 0
    
def createBashScript(outDir):    
    f=open(outDir+"/"+"/myScript.sh",'w')
    f.write("#!/bin/bash\n")
    f.write("#SBATCH --ntasks=1\n")
    f.write("#SBATCH --ntasks-per-node=1\n")
    f.write("#SBATCH --time=12:00:00\n")
    f.write("#SBATCH --output="+outDir+"/log.out\n")
    f.write("#SBATCH --error="+outDir+"/log.err\n")
    f.write("#SBATCH --mail-type=ALL\n")
    f.write("#SBATCH --mail-user=cg8fd@virginia.edu\n")
    f.write("#SBATCH --partition=serial\n")
    f.write("cd "+outDir+"\n")
    f.write("./prexsim preRun.mac myRun.mac\n")
    f.close()
    return 0  

if __name__ == '__main__':
    main()
                  
