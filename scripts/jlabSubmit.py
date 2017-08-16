#!/usr/bin/python
from subprocess import call
import sys,os,time

def main():

    email = "ciprian@jlab.org"
    ##only crex5, prexII defined for now
    configuration = "crex5"
    sourceDir = "/home/ciprian/prex/ajzprexSim/prexSim"
    outputDir = "/lustre/expphy/volatile/halla/parity/ciprian/farmOut"
    nrEv   = 1000
    nrStart= 0
    nrStop = 3
    identifier = "test"

    print('Running ' + str(nrEv*(nrStop - nrStart)) + ' events...')

    jobName=configuration + '_' + identifier + '_%03dkEv'%(nrEv/1000)

    for nr in range(nrStart,nrStop): # repeat for nr jobs
        print("Starting job setup for jobID: " + str(nr))

        jobFullName = jobName + '_%05d'%nr
        createMacFiles(configuration, outputDir+"/"+jobFullName, sourceDir, nrEv, nr, identifier)

        ###copy executable
        call(["cp",sourceDir+"/build/prexsim",
              outputDir+"/"+jobFullName+"/prexsim"])

        ###copy geoemtries
        call(["cp","-r",sourceDir+"/geometry",
              outputDir+"/"+jobFullName])


    createXMLfile(sourceDir,outputDir,jobName,nrStart,nrStop,email)

    print "All done"


def createMacFiles(config,outDir,sourceDir,nrEv,jobNr,identifier):

    if not os.path.exists(outDir+"/geometry"):
        os.makedirs(outDir+"/geometry")

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
        f.write("/moller/det/setDetectorFileName geometry/crex5_AJZShields.gdml\n")
    elif config=="prexII":
    	f.write("/gun/energy 1. GeV\n")
        f.write("/moller/field/setConfiguration prex2\n")
        f.write("/moller/det/setDetectorFileName geometry/prexII_AJZShields.gdml\n")

    ##f.write("/moller/det/setDetectorFileName geometry/prexII_AJZShields.gdml\n")
    ##f.write("/moller/det/setDetectorFileName geometry/crex5_AJZShields.gdml\n")

    f.write("/moller/field/useQ1fringeField false\n")

    f.write("/moller/det/setShieldMaterial polyethylene\n")
    f.write("/testhadr/CutsAll 0.7 mm\n")
    f.write("/run/initialize\n")
    f.close()

    return 0

def createXMLfile(source,writeDir,idRoot,nStart,nStop,email):

    if not os.path.exists(source+"/scripts/jobs"):
        os.makedirs(source+"/scripts/jobs")

    f=open(source+"/scripts/jobs/"+idRoot+".xml","w")
    f.write("<Request>\n")
    f.write("  <Email email=\""+email+"\" request=\"false\" job=\"true\"/>\n")
    f.write("  <Project name=\"prex\"/>\n")
#    f.write("  <Track name=\"debug\"/>\n")
    f.write("  <Track name=\"simulation\"/>\n")
    f.write("  <Name name=\""+idRoot+"\"/>\n")
    f.write("  <OS name=\"centos7\"/>\n")
    f.write("  <Command><![CDATA[\n")
    f.write("     prexSim preRun.mac myRun.mac\n")
    f.write("  ]]></Command>\n")
    f.write("  <Memory space=\"2000\" unit=\"MB\"/>\n")

    for nr in range(nStart,nStop): # repeat for nr jobs
        idName= writeDir+"/"+idRoot+'_%05d'%(nr)
        f.write("  <Job>\n")
        f.write("   <Command><![CDATA[\n")
        f.write("     cd "+idName+"\n")
        f.write("     ./prexSim preRun.mac myRun.mac\n")
        f.write("   ]]></Command>\n")
        f.write("  </Job>\n\n")

    f.write("</Request>\n")
    f.close()
    return 0

if __name__ == '__main__':
    main()

