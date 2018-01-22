#!/usr/bin/python
from subprocess import call
import sys, os, time, tarfile
import config_reader as cr

def main():

    email = cr.jlab_id() + "@jlab.org"
    ##only crex5, prexII defined for now
    #configuration = "crex5"

    configuration = cr.config()
    sourceDir = "/lustre/expphy/work/halla/parity/" + cr.jlab_id() + "/prexSim"
    outputDir = "/lustre/expphy/volatile/halla/parity/" + cr.jlab_id() + "/farmOut"
    nrEv   = cr.nr_events()
    nrStart= cr.start_run()
    nrStop = cr.end_run()
    identifier = cr.identifier()

    print('Starting setup for ' + configuration + ' simulation with geometry ' + identifier + '...')
    print('Running ' + str(nrEv*(nrStop - nrStart)) + ' events...')

    jobName=configuration + '_' + identifier + '_%03dkEv'%(nrEv/1000)

    ###tar exec+geometry
    make_tarfile(sourceDir,configuration+"_"+identifier)

    for nr in range(nrStart,nrStop): # repeat for nr jobs
        print("Starting job setup for jobID: " + str(nr))

        jobNumber   = '%05d'%nr
        jobFullName = jobName + '_%05d'%nr
        createMacFiles(configuration, outputDir+"/"+jobName+"/"+jobNumber, sourceDir, nrEv, nr, identifier)

        ###copy tarfile
        call(["cp",sourceDir+"/scripts/z_config.tar.gz",
              outputDir+"/"+jobName+"/"+jobNumber+"/z_config.tar.gz"])

    createXMLfile(sourceDir,outputDir+"/"+jobName,jobName,nrStart,nrStop,email)

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
        f.write("/moller/det/setDetectorFileName geometry/crex5_"+identifier+".gdml\n")
    elif config=="prexII":
    	f.write("/gun/energy 1. GeV\n")
        f.write("/moller/field/setConfiguration prex2\n")
        f.write("/moller/det/setDetectorFileName geometry/prexII_"+identifier+".gdml\n")

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
    tar.add(sourceDir+"/geometry/schema",arcname="geometry/schema")
    tar.add(sourceDir+"/geometry/"+config+".gdml" ,arcname="geometry/"+config+".gdml")
    tar.add(sourceDir+"/geometry/kriptoniteDetectors.gdml",arcname="geometry/kriptoniteDetectors.gdml")
    tar.add(sourceDir+"/geometry/kriptoniteDetectors_withHRS.gdml",arcname="geometry/kriptoniteDetectors_withHRS.gdml")
    for subassem in cr.subassems():
        tar.add(sourceDir + "/geometry/sub" + subassem + ".gdml",arcname="geometry/sub" + subassem + ".gdml")
	print('Adding file: ' + subassem + '...')
    tar.add(sourceDir+"/geometry/materials.xml",arcname="geometry/materials.xml")
    tar.close()

if __name__ == '__main__':
    main()

