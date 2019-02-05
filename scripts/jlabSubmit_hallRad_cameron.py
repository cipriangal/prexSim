#!/usr/bin/python
from subprocess import call
import sys, os, time, tarfile

def main():

#FIXME Update these
    email = "cameronc@jlab.org"

    #configuration = "prexI"
    #configuration = "crex5"
    configuration = "prexII"
    #configuration = "moller"
    #configuration = "happex2"

    stage = "new"
    varied = raw_input("Please enter the indicative name: ")#"off_thickness"
    #geo = raw_input("Please enter the can geometry (sph or cyl): ")
    offset = raw_input("Please enter the offset number (integers): ")
    units = raw_input("Please enter the units (2 characters: mm, ml, etc.): ") 

    identifier = "cadSAMs"#raw_input("Please enter the identifier: ")

    #sourceDir = "/work/halla/parity/disk1/ciprian/prexSim"
    sourceDir = "/work/halla/parity/disk1/cameronc/masterPrexSim"
    sourceMasterDir = "/work/halla/parity/disk1/cameronc/masterPrexSim"
    outputDir = "/lustre/expphy/volatile/halla/parity/cameronc/prexSim/output/SAM_"+stage+"_hallRad"
    nrEv=900000

    jobName=configuration + '_' + identifier + '_' + varied + '_' + offset + units + '_%03dkEv'%(nrEv/1000)
    listName='list_' + identifier + '_' + varied + '_' + offset + units

    if not os.path.exists(outputDir+"/"+jobName+"/log"):
        os.makedirs(outputDir+"/"+jobName+"/log")
    createXMLfile(sourceDir,outputDir,jobName,varied,stage,identifier,offset,units,listName,email)

    call(["cp",sourceDir+"/output/ls_mode.sh",
              outputDir+"/"+jobName+"/ls_mode.sh"])
    call(["cp",sourceMasterDir+"/build/hallRad",
              outputDir+"/"+jobName+"/hallRad"])
    print "All done for configuration ",configuration,"_",identifier,"for",offset

def createXMLfile(sourceDir,outputDir,jobName,varied,stage,identifier,offset,units,listName,email):

    if not os.path.exists(sourceDir+"/output/jobs"):
        os.makedirs(sourceDir+"/output/jobs")

    f=open(sourceDir+"/output/jobs/"+jobName+"_hallRad.xml","w")
    f.write("<Request>\n")
    f.write("  <Email email=\""+email+"\" request=\"false\" job=\"true\"/>\n")
    f.write("  <Project name=\"prex\"/>\n")

#    f.write("  <Track name=\"debug\"/>\n")
    f.write("  <Track name=\"analysis\"/>\n")

    f.write("  <Name name=\""+identifier+"_hallRad\"/>\n")
    f.write("  <OS name=\"centos7\"/>\n")
    f.write("  <Memory space=\"3500\" unit=\"MB\"/>\n")

    f.write("  <Command><![CDATA[\n")
    f.write("    pwd\n")
    f.write("    source /site/12gev_phys/softenv.csh 2.0\n")
    f.write("    ./ls_mode.sh " + varied + " " + offset + " " + stage + " " + units + "\n")
    
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
