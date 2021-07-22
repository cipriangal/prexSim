#!/usr/bin/python
from subprocess import call
import sys, os, time, tarfile

def main():

    email = "tao@jlab.org"
    ##only crex, prexII, tritium defined for now

    #configuration = "prex1"
    #configuration = "crex"
    #configuration = "prex2"
    # configuration = "apex"
    #configuration = "happex2"
    configuration = "pvdis"

    sourceDir = "/w/moller12gev-sciwork18/tao/prex-sim"
    outputDir = "/lustre19/expphy/volatile/halla/moller12gev/tao/prexSim_pvdis"

    nrEv   = 1000000
    nrStart= 0
    nrStop = 100

    ###format should be _Name or "" for base config

    print('Starting setup for ' + configuration + ' simulation with geometry ...')
    print('Running ' + str(nrEv*(nrStop - nrStart)) + ' events...')

    jobName=configuration + '_%03dkEv'%(nrEv/1000)

    ###tar exec+geometry
    make_tarfile(sourceDir,configuration)

    for nr in range(nrStart,nrStop): # repeat for nr jobs
        print("Starting job setup for jobID: " + str(nr))

        jobNumber   = '%05d'%nr
        jobFullName = jobName + '_%05d'%nr
        createMacFiles(configuration, outputDir+"/"+jobName+"/"+jobNumber, sourceDir, nrEv, nr)

        ###copy tarfile
        call(["cp",sourceDir+"/scripts/z_config.tar.gz",
              outputDir+"/"+jobName+"/"+jobNumber+"/z_config.tar.gz"])

    createXMLfile(sourceDir,outputDir+"/"+jobName,jobName,nrStart,nrStop,email)

    print "All done for configuration ", configuration," for #s between ",nrStart, " and ", nrStop


def createMacFiles(config,outDir,sourceDir,nrEv,jobNr):

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

    if config=="crex":
        f.write("/gun/energy 2.22 GeV\n")
        f.write("/moller/field/setConfiguration crex\n")
        f.write("/moller/det/setDetectorFileName geometry/crex.gdml\n")
    elif config=="prexII":
    	f.write("/gun/energy 0.95 GeV\n")
        f.write("/moller/field/setConfiguration prex2\n")
        f.write("/moller/det/setDetectorFileName geometry/prexII.gdml\n")
    elif config=="tritium":
        f.write("/gun/energy 11. GeV\n")
        f.write("/moller/field/setFieldScaleFactor 0.\n")
        f.write("/moller/field/setLowLim -74 cm\n/moller/field/setHighLim 74 cm\n")
        f.write("/moller/det/setDetectorFileName geometry/tritium.gdml\n")
    elif config=="prex1":
    	f.write("/gun/energy 1.06 GeV\n")
        f.write("/prex/gun/setRasterX 6 mm\n")
        f.write("/prex/gun/setRasterY 4 mm\n")
        f.write("/moller/field/setConfiguration prex1\n")
        f.write("/moller/det/setDetectorFileName geometry/prex1.gdml\n")
    elif config=="happex2":
    	f.write("/gun/energy 3. GeV\n")
        f.write("/prex/gun/setRasterX 5 mm\n")
        f.write("/prex/gun/setRasterY 5 mm\n")
        f.write("/moller/field/setConfiguration happex2\n")
        f.write("/moller/det/setDetectorFileName geometry/happex2.gdml\n")
    elif config=="apex":
    	f.write("/gun/energy 2.2 GeV\n")
        f.write("/prex/gun/setRasterX 4 mm\n")
        f.write("/prex/gun/setRasterY 4 mm\n")
        f.write("/moller/field/setConfiguration apex\n")
        f.write("/moller/det/setDetectorFileName geometry/apex.gdml\n")
    elif config=="pvdis":
    	f.write("/gun/energy 6 GeV\n")
        f.write("/prex/gun/setRasterX 5 mm\n")
        f.write("/prex/gun/setRasterY 5 mm\n")
        f.write("/moller/field/setConfiguration pvdis\n")
        f.write("/moller/det/setDetectorFileName geometry/pvdis.gdml\n")

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
    f.write("  <OS name=\"centos77\"/>\n")
    f.write("  <Memory space=\"4500\" unit=\"MB\"/>\n")

    f.write("  <Command><![CDATA[\n")
    f.write("    pwd\n")
    f.write("    tar -zxvf z_config.tar.gz\n")
    f.write("    ./prexsim preRun.mac myRun.mac\n")
    f.write("  ]]></Command>\n")

    for nr in range(nStart,nStop): # repeat for nr jobs
        idName= writeDir+"/"+'%05d'%(nr)
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
    tar.add(sourceDir + "/geometry/",arcname="geometry/")
    tar.close()

if __name__ == '__main__':
    main()

