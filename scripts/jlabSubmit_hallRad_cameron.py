#!/usr/bin/python
from subprocess import call
import sys, os, time, tarfile

def main():

#FIXME Update these
    email = "cameronc@jlab.org"

    stage = "2019"
    name = raw_input("Please enter the job name to analyze: ")

    sourceDir = "/work/halla/parity/disk1/cameronc/crexSim"
    outputDir = "/lustre/expphy/volatile/halla/parity/cameronc/prexSim/output/SAM_"+stage+"_hallRad"
    nrEv=900000

    jobName=name + '_%03dkEv'%(nrEv/1000)
    listName='list_' + name

    if not os.path.exists(outputDir+"/"+jobName+"/log"):
        os.makedirs(outputDir+"/"+jobName+"/log")
    createXMLfile(sourceDir,outputDir,name,jobName,listName,email)

    call(["cp",sourceDir+"/output/ls_mode.sh",
              outputDir+"/"+jobName+"/ls_mode.sh"])
    call(["cp",sourceDir+"/build/hallRad",
              outputDir+"/"+jobName+"/hallRad"])
    print "All done for configuration ",name

def createXMLfile(sourceDir,outputDir,name,jobName,listName,email):

    if not os.path.exists(sourceDir+"/output/jobs"):
        os.makedirs(sourceDir+"/output/jobs")

    f=open(sourceDir+"/output/jobs/"+jobName+"_hallRad.xml","w")
    f.write("<Request>\n")
    f.write("  <Email email=\""+email+"\" request=\"false\" job=\"true\"/>\n")
    f.write("  <Project name=\"prex\"/>\n")

#    f.write("  <Track name=\"debug\"/>\n")
    f.write("  <Track name=\"analysis\"/>\n")

    f.write("  <Name name=\""+listName+"_hallRad\"/>\n")
    f.write("  <OS name=\"centos7\"/>\n")
    f.write("  <Memory space=\"3500\" unit=\"MB\"/>\n")

    f.write("  <Command><![CDATA[\n")
    f.write("    pwd\n")
    f.write("    source /site/12gev_phys/softenv.csh 2.3\n")
    f.write("    ./ls_mode.sh " + name + "\n")
    
    f.write("  ]]></Command>\n")

    idName= outputDir+"/"+jobName
    f.write("  <Job>\n")
    f.write("    <Input src=\""+idName+"/hallRad\" dest=\"hallRad\"/>\n")
    f.write("    <Input src=\""+idName+"/ls_mode.sh\" dest=\"ls_mode.sh\"/>\n")
    #f.write("    <Input src=\""+idName+"/"+listName+".txt\" dest=\""+listName+".txt\"/>\n")
    f.write("    <Output src=\""+listName+".txt\" dest=\""+idName+"/"+listName+".txt\"/>\n")
    f.write("    <Output src=\""+listName+"_hallRad.root\" dest=\""+idName+"/"+listName+"_hallRad.root\"/>\n")

    f.write("    <Stdout dest=\""+idName+"/log/log.out\"/>\n")
    f.write("    <Stderr dest=\""+idName+"/log/log.err\"/>\n")
    f.write("  </Job>\n\n")

    f.write("</Request>\n")
    f.close()
    return 0

if __name__ == '__main__':
    main()
