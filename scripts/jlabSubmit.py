#!/usr/bin/python
from subprocess import call
import sys, os, time, tarfile

def main():

    email = "ciprian@jlab.org"
    ##only crex5, prexII, prexI, moller defined for now
    #configuration = "prexI"
    #configuration = "crex5"
    #configuration = "prexII"
    configuration = "moller"
    sourceDir = "/lustre/expphy/work/halla/parity/ciprian/prexSim"
    outputDir = "/lustre/expphy/volatile/halla/parity/ciprian/farmOut/dump/nakedMoller"
    nrEv   = 500000
    nrStart= 20
    nrStop = 40

    ###format should be _Name
    identifier = "_fatPipe"

    print('Running ' + str(nrEv*(nrStop - nrStart)) + ' events...')

    jobName=configuration + identifier + '_%03dkEv'%(nrEv/1000)

    ###tar exec+geometry
    make_tarfile(sourceDir,configuration+identifier)

    for nr in range(nrStart,nrStop): # repeat for nr jobs
        print("Starting job setup for jobID: " + str(nr))

        jobFullName = jobName + '_%05d'%nr
        createMacFiles(configuration, outputDir+"/"+jobFullName, sourceDir, nrEv, nr, identifier)

        ###copy tarfile
        call(["cp",sourceDir+"/scripts/z_config.tar.gz",
              outputDir+"/"+jobFullName+"/z_config.tar.gz"])

    createXMLfile(sourceDir,outputDir,jobName,nrStart,nrStop,email)

    print "All done"


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
    seedA=long(time.time()+jobNr)
    seedB=long(time.time()*100+jobNr)
    f.write("/random/setSeeds "+str(seedA)+" "+str(seedB)+"\n")

    if config=="crex5":
        f.write("/gun/energy 2. GeV\n")
        f.write("/moller/field/setConfiguration crex\n")
        f.write("/moller/det/setDetectorFileName geometry/crex5"+identifier+".gdml\n")
    elif config=="prexII":
    	f.write("/gun/energy 1. GeV\n")
        f.write("/moller/field/setConfiguration prex2\n")
        f.write("/moller/det/setDetectorFileName geometry/prexII"+identifier+".gdml\n")
    elif config=="prexI":
    	f.write("/gun/energy 1. GeV\n")
        f.write("/moller/field/setConfiguration prex1\n")
        f.write("/moller/det/setDetectorFileName geometry/prexI"+identifier+".gdml\n")
    elif config=="moller":
    	f.write("/gun/energy 11. GeV\n")
        f.write("/moller/field/setConfiguration moller\n")
        f.write("/moller/det/setDetectorFileName geometry/moller"+identifier+".gdml\n")

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
    f.write("  <Memory space=\"3500\" unit=\"MB\"/>\n")

    f.write("  <Command><![CDATA[\n")
    f.write("    pwd\n")
    f.write("    tar -zxvf z_config.tar.gz\n")
    f.write("    ./prexsim preRun.mac myRun.mac\n")
    f.write("  ]]></Command>\n")

    for nr in range(nStart,nStop): # repeat for nr jobs
        idName= writeDir+"/"+idRoot+'_%05d'%(nr)
        f.write("  <Job>\n")
        f.write("    <Input src=\""+idName+"/preRun.mac\" dest=\"preRun.mac\"/>\n")
        f.write("    <Input src=\""+idName+"/myRun.mac\" dest=\"myRun.mac\"/>\n")
        f.write("    <Input src=\""+idName+"/z_config.tar.gz\" dest=\"z_config.tar.gz\"/>\n")

        f.write("    <Output src=\"o_prexSim.root\" dest=\""+idName+"/o_prexSim.root\"/>\n")
        f.write("    <Stdout dest=\""+idName+"/log/log.out\"/>\n")
        f.write("    <Stderr dest=\""+idName+"/log/log.err\"/>\n")
        f.write("  </Job>\n\n")

    f.write("</Request>\n")
    f.close()
    return 0

def make_tarfile(sourceDir,config):
    print "making geometry tarball"
    if os.path.isfile(sourceDir+"/scripts/z_config.tar.gz"):
        os.remove(sourceDir+"/scripts/z_config.tar.gz")
    tar = tarfile.open(sourceDir+"/scripts/z_config.tar.gz","w:gz")
    tar.add(sourceDir+"/build/prexsim",arcname="prexsim")
    tar.add(sourceDir+"/geometry/schema",arcname="geometry/schema")
    tar.add(sourceDir+"/geometry/"+config+".gdml" ,arcname="geometry/"+config+".gdml")
    tar.add(sourceDir+"/geometry/kriptoniteDetectors.gdml",arcname="geometry/kriptoniteDetectors.gdml")
    tar.add(sourceDir+"/geometry/kriptoniteDetectors_withHRS.gdml",arcname="geometry/kriptoniteDetectors_withHRS.gdml")
    tar.add(sourceDir+"/geometry/subQ1HosesCylRedesign.gdml",arcname="geometry/subQ1HosesCylRedesign.gdml")
    tar.add(sourceDir+"/geometry/subTargetChamber.gdml",arcname="geometry/subTargetChamber.gdml")
    tar.add(sourceDir+"/geometry/subCollShields.gdml",arcname="geometry/subCollShields.gdml")
    tar.add(sourceDir+"/geometry/prex1BeampipeV2.gdml",arcname="geometry/prex1BeampipeV2.gdml")
    tar.add(sourceDir+"/geometry/subBeamPipe.gdml",arcname="geometry/subBeamPipe.gdml")
    tar.add(sourceDir+"/geometry/subBeamPipe_steelTelePipe.gdml",arcname="geometry/subBeamPipe_steelTelePipe.gdml")
    tar.add(sourceDir+"/geometry/subBeamPipe_fat.gdml",arcname="geometry/subBeamPipe_fat.gdml")
    tar.add(sourceDir+"/geometry/subBeamPipe_4inDonut.gdml",arcname="geometry/subBeamPipe_4inDonut.gdml")
    tar.add(sourceDir+"/geometry/subBeamPipe_noDonut.gdml",arcname="geometry/subBeamPipe_noDonut.gdml")
    tar.add(sourceDir+"/geometry/subBeamPipe_MidVacuum.gdml",arcname="geometry/subBeamPipe_MidVacuum.gdml")
    tar.add(sourceDir+"/geometry/subDumpShield.gdml",arcname="geometry/subDumpShield.gdml")
    tar.add(sourceDir+"/geometry/subDumpShield_cover.gdml",arcname="geometry/subDumpShield_cover.gdml")
    tar.add(sourceDir+"/geometry/subDumpShield_2layer.gdml",arcname="geometry/subDumpShield_2layer.gdml")
    tar.add(sourceDir+"/geometry/materials.xml",arcname="geometry/materials.xml")
    tar.add(sourceDir+"/geometry/subHRSplatform.gdml",arcname="geometry/subHRSplatform.gdml")
    tar.add(sourceDir+"/geometry/subHRSplatform_withShield.gdml",arcname="geometry/subHRSplatform_withShield.gdml")
    tar.close()

if __name__ == '__main__':
    main()

