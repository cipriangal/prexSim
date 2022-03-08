#!/apps/bin/python3
from subprocess import call
import sys, os, time, tarfile

def main():

    email = "ciprian@jlab.org"

    #configuration = "prex1"
    #configuration = "crex"
    #configuration = "prex2"
    #configuration = "apex"
    #configuration = "happex2"
    configuration = "pvdis"

    identifier = "" ##this needs to be the same as the gdml file

    sourceDir = "/work/halla/moller12gev/ciprian/prex/prexSim"
    outputDir = "/volatile/halla/parity/ciprian/farmOut/test"

    nrEv   = 1000
    nrStart= 0
    nrStop = 3
    submit  = 1 ## submit is 1 to submit job, submit is 0 to create folder without submitting the jobs

    if not os.path.exists(outputDir):
       os.makedirs(outputDir)

    print('Starting setup for ' + configuration + ' simulation with geometry ' + identifier + '...')
    print('Running ' + str(nrEv*(nrStop - nrStart)) + ' events...')
    jobName=configuration + '_' + identifier + '_%03dkEv'%(nrEv/1000)

    ###tar exec+geometry
    make_tarfile(sourceDir)
    call(["cp",sourceDir+"/scripts/default.tar.gz",
          outputDir+"/default.tar.gz"])

    for jobNr in range(nrStart,nrStop): # repeat for jobNr jobs
        print("Starting job setup for jobID: " + str(jobNr))

        jobFullName = jobName + '_%04d'%jobNr
        outDirFull=outputDir+"/"+jobFullName
        createMacFiles(configuration, outDirFull, sourceDir, nrEv, jobNr, identifier)

        createSBATCHfile(sourceDir,outDirFull,jobName,jobNr)

        if submit==1:
            print("submitting", jobName)
            call(["sbatch",sourceDir+"/jobs/"+jobName+".sh"])

    print("All done for config ",configuration," for #s between ",nrStart, " and ", nrStop)

def createMacFiles(config,outDir,sourceDir,nrEv,jobNr,identifier):

    if not os.path.exists(outDir+"/log"):
        os.makedirs(outDir+"/log")

    f=open(outDir+"/"+"/myRun.mac",'w')
    f.write("/moller/ana/rootfilename ./o_prexSim\n")
    f.write("/run/beamOn "+str(nrEv)+"\n")
    f.close()

    f=open(outDir+"/"+"/preRun.mac",'w')
    f.write("/moller/det/readGeometryFromFile true\n")
    f.write("/gun/particle e-\n")
    f.write("/moller/gun/gen 7\n")
    seedA=int(time.time()+jobNr)
    seedB=int(time.time()*100+jobNr)
    f.write("/random/setSeeds "+str(seedA)+" "+str(seedB)+"\n")

    if config=="crex":
        f.write("/gun/energy 2.22 GeV\n")
        f.write("/moller/field/setConfiguration crex\n")
        f.write("/moller/det/setDetectorFileName geometry/crex_"+identifier+".gdml\n")
    elif config=="prexII":
        f.write("/gun/energy 0.95 GeV\n")
        f.write("/moller/field/setConfiguration prex2\n")
        f.write("/moller/det/setDetectorFileName geometry/prexII_"+identifier+".gdml\n")
    elif config=="tritium":
        f.write("/gun/energy 11. GeV\n")
        f.write("/moller/field/setFieldScaleFactor 0.\n")
        f.write("/moller/field/setLowLim -74 cm\n/moller/field/setHighLim 74 cm\n")
        f.write("/moller/det/setDetectorFileName geometry/tritium_"+identifier+".gdml\n")
    elif config=="prex1":
        f.write("/gun/energy 1.06 GeV\n")
        f.write("/prex/gun/setRasterX 6 mm\n")
        f.write("/prex/gun/setRasterY 4 mm\n")
        f.write("/moller/field/setConfiguration prex1\n")
        f.write("/moller/det/setDetectorFileName geometry/prex1"+identifier+".gdml\n")
    elif config=="happex2":
        f.write("/gun/energy 3. GeV\n")
        f.write("/prex/gun/setRasterX 5 mm\n")
        f.write("/prex/gun/setRasterY 5 mm\n")
        f.write("/moller/field/setConfiguration happex2\n")
        f.write("/moller/det/setDetectorFileName geometry/happex2"+identifier+".gdml\n")
    elif config=="apex":
        f.write("/gun/energy 2.2 GeV\n")
        f.write("/prex/gun/setRasterX 4 mm\n")
        f.write("/prex/gun/setRasterY 4 mm\n")
        f.write("/moller/field/setConfiguration apex\n")
        f.write("/moller/det/setDetectorFileName geometry/apex"+identifier+".gdml\n")
    elif config=="pvdis":
        f.write("/gun/energy 6 GeV\n")
        f.write("/prex/gun/setRasterX 5 mm\n")
        f.write("/prex/gun/setRasterY 5 mm\n")
        f.write("/moller/field/setConfiguration pvdis\n")
        f.write("/moller/det/setDetectorFileName geometry/pvdis"+identifier+".gdml\n")

    f.write("/moller/field/useQ1fringeField false\n")

    f.write("/moller/det/setShieldMaterial polyethylene\n")
    f.write("/testhadr/CutsAll 0.7 mm\n")
    f.write("/run/initialize\n")
    f.close()

    return 0

def createSBATCHfile(sourceDir,outDirFull,jobName,jobNr):

    if not os.path.exists(sourceDir+"/jobs"):
        os.makedirs(sourceDir+"/jobs")

    f=open(sourceDir+"/jobs/"+jobName+".sh","w")
    f.write("#!/bin/bash\n")
    f.write("#SBATCH --ntasks=1\n")
    f.write("#SBATCH --job-name="+jobName+'_%03d'%jobNr+"\n")
    f.write("#SBATCH --output="+outDirFull+"/log.out\n")
    f.write("#SBATCH  --error="+outDirFull+"/log.err\n")
    f.write("#SBATCH --partition=priority\n")
    #f.write("#SBATCH --partition=production\n")
    f.write("#SBATCH --account=halla\n")
    f.write("#SBATCH --mem-per-cpu=5000\n")
    f.write("#SBATCH --exclude=farm19104,farm19105,farm19106,farm19107,farm1996,farm19101\n")
    f.write("cd "+outDirFull+"\n")
    f.write("cp ../default.tar.gz ./\n")
    f.write("tar -zxvf default.tar.gz\n")
    f.write("./prexsim preRun.mac myRun.mac\n")
    f.write("rm -rf default.tar.gz geometry macro.mac prexsim\n")
    f.close()
    return 0



def make_tarfile(sourceDir):
    print("making geometry tarball")
    if os.path.isfile(sourceDir+"/scripts/default.tar.gz"):
        os.remove(sourceDir+"/scripts/default.tar.gz")
    tar = tarfile.open(sourceDir+"/scripts/default.tar.gz","w:gz")
    tar.add(sourceDir+"/build/prexsim",arcname="prexsim")
 #   tar.add(sourceDir+"/geometry/schema",arcname="geometry/schema")
    tar.add(sourceDir + "/geometry/",arcname="geometry/")
    tar.close()


if __name__ == '__main__':
    main()

