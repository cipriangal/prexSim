#!/usr/bin/python
from subprocess import call
import sys, os, time, tarfile

def main():

#FIXME Update these
    email = "cameronc@jlab.org"

    #config = "prex1"
    #config = "crex5"
    config = "prex2"
    #config = "moller"
    #config = "happex2"

    #stage = "final_shorterIn"
    stage = "HEAD"
    #varied = raw_input("Please enter an indicative name: ")#"thin"
    #geo = raw_input("Please enter the can geometry (sph or cyl): ")
    #offset = raw_input("Please enter the offset in mm (integers up to 360): ")
    #thickness = "6.0"#raw_input("Please enter the thickness in mm (integers up to 15): ")
    #thin_thickness = raw_input("Please enter the thin thickness in mils (integers from 10 to 65): ")
    #sph_thickness = raw_input("Please enter the sph thickness in mils (integers from 10 to 65): ")
    #thick_thickness = raw_input("Please enter the thick thickness in mils (integers from 10 to 65): ")
    #can_thinner_length = raw_input("Please enter the can thin length in mm (integers from 0 to 60, or up to 340): ")
    identifier = "NOcadSAMs"#raw_input("Please enter the identifier: ")

    #f = open('../geometry/subBeamPipe_'+identifier+'.xml', 'w')
    #fileout = '    <constant name="sam_can_thicker_wall_thickness" value="' + thick_thickness + '*25.4/10000.0"/>\n    <constant name="sam_can_wall_thickness" value="' + thin_thickness + '*25.4/10000.0"/>\n    <constant name="sam_can_thinner_wall_length" value="' + can_thinner_length + '/10"/>\n    <constant name="full_sam_r_outward_offset" value="' + offset + '.0/10 + 0*0.75"/>\n    <constant name="sam_quartz_height" value="' + thickness + '/10"/>\n    <constant name="quartz_z_face_offset" value="1.2455"/>\n    <constant name="sam_can_face_thickness" value="' + sph_thickness + '*25.4/10000.0"/>\n    <constant name="sam_window_thickness" value="sam_can_face_thickness"/>\n    <constant name="sam_window_inner_r" value="sam_window_outer_r - sam_window_thickness"/>\n    <constant name="sam_bot_face_sep" value="sam_quartz_bot_face + 0*(sam_window_outer_r - sam_window_thickness) + 1.*sqrt((sam_window_inner_r - 0.1)**2 - quartz_z_face_offset**2 - (0.5*sam_quartz_width)**2)"/>\n    <constant name="sam_mid_dist" value="full_sam_r_outward_offset + sam_bot_face_sep + sam_can_length/2."/>\n    <constant name="sam_quartz_length" value="2.0 + sam_quartz_height - 1*0.75"/>\n    <constant name="sam_quartz_mid_dist" value="full_sam_r_outward_offset + sam_quartz_bot_face + sam_quartz_length/2."/>\n'

    #f.write(fileout)
    #f.close()

    #sourceDir = "/work/halla/parity/disk1/ciprian/prexSim"
    sourceDir = "/work/halla/parity/disk1/cameronc/HEADprexSim"
    outDir = "/lustre/expphy/volatile/halla/parity/cameronc/prexSim/output/SAM_"+stage+"_tests"
    if not os.path.exists(outDir):
        os.makedirs(outDir)
    nrEv   = 900000 #900000
    nrStart= 0
    nrStop = 500 #60
    ###format should be Name (removed _)
    #"SAMs_noAl" #6inDonut_SAMs"  (spherical, cylindrical, noFace, noAl, noQ, noQnoAl)
#</FIXME>

    print('Running ' + str(nrEv*(nrStop - nrStart)) + ' events...')

    jobName=config + '_' + identifier + '_%03dkEv'%(nrEv/1000)

    ###tar exec+geometry
    make_tarfile(sourceDir,config,identifier)

    for jobNr in range(nrStart,nrStop): # repeat for jobNr jobs
        #print("Starting job setup for jobID: " + str(jobNr))

        jobFullName = jobName + '_%05d'%jobNr
        outDirFull=outDir+"/"+jobFullName
        createMacFiles(config, outDirFull, sourceDir, nrEv, jobNr, identifier)

        ###copy tarfile
        call(["cp",sourceDir+"/scripts/z_config.tar.gz",
              outDir+"/"+jobFullName+"/z_config.tar.gz"])

    createXMLfile(sourceDir,outDir,jobName,nrStart,nrStop,email)

    print "All done for config ",config,"_",identifier," for #s between ",nrStart, " and ", nrStop


def createMacFiles(config,outDirFull,sourceDir,nrEv,jobNr,identifier):

    if not os.path.exists(outDirFull+"/log"):
        os.makedirs(outDirFull+"/log")

    f=open(outDirFull+"/"+"myRun.mac",'w')
    f.write("/moller/ana/rootfilename ./o_prexSim\n")
    f.write("/run/beamOn "+str(nrEv)+"\n")
    f.close()

    f=open(outDirFull+"/"+"/preRun.mac",'w')
    f.write("/moller/det/readGeometryFromFile true\n")
    f.write("/gun/particle e-\n")
    f.write("/moller/gun/gen 7\n")
    seedA=long(time.time()+jobNr)
    seedB=long(time.time()*100+jobNr)
    f.write("/random/setSeeds "+str(seedA)+" "+str(seedB)+"\n")

    if config=="crex5":
        f.write("/gun/energy 2.22 GeV\n")
        f.write("/moller/field/setConfiguration crex\n")
        f.write("/moller/det/setDetectorFileName geometry/crex5_"+identifier+".gdml\n")
    elif config=="prex2":
    	f.write("/gun/energy 0.95 GeV\n")
        f.write("/moller/field/setConfiguration prex2\n")
        f.write("/moller/det/setDetectorFileName geometry/prex2_"+identifier+".gdml\n")
    elif config=="prex1":
    	f.write("/gun/energy 1. GeV\n")
        f.write("/moller/field/setConfiguration prex1\n")
        f.write("/moller/det/setDetectorFileName geometry/prex1_"+identifier+".gdml\n")
    elif config=="moller":
    	f.write("/gun/energy 11. GeV\n")
        f.write("/moller/field/setConfiguration moller\n")
        f.write("/moller/det/setDetectorFileName geometry/moller_"+identifier+".gdml\n")
    elif config=="happex2":
    	f.write("/gun/energy 3. GeV\n")
        f.write("/moller/field/setConfiguration happex2\n")
        f.write("/moller/det/setDetectorFileName geometry/happex2_"+identifier+".gdml\n")

    f.write("/moller/field/useQ1fringeField false\n")

    f.write("/moller/det/setShieldMaterial polyethylene\n")
    f.write("/testhadr/CutsAll 0.7 mm\n")
    f.write("/run/initialize\n")
    f.close()

    return 0

def createXMLfile(sourceDir,outDir,jobName,nrStart,nrStop,email):

    if not os.path.exists(sourceDir+"/scripts/jobs"):
        os.makedirs(sourceDir+"/scripts/jobs")

    f=open(sourceDir+"/scripts/jobs/"+jobName+".xml","w")
    f.write("<Request>\n")
    f.write("  <Email email=\""+email+"\" request=\"false\" job=\"true\"/>\n")
    f.write("  <Project name=\"prex\"/>\n")

#    f.write("  <Track name=\"debug\"/>\n")
    f.write("  <Track name=\"simulation\"/>\n")

    f.write("  <Name name=\""+jobName+"\"/>\n")
    f.write("  <OS name=\"centos7\"/>\n")
    f.write("  <Memory space=\"3500\" unit=\"MB\"/>\n")

    f.write("  <Command><![CDATA[\n")
    f.write("    pwd\n")
    f.write("    tar -zxvf z_config.tar.gz\n")
    f.write("    ./prexsim preRun.mac myRun.mac\n")
    f.write("  ]]></Command>\n")

    for number in range(nrStart,nrStop): # repeat for number jobs
        idName= outDir+"/"+jobName+'_%05d'%(number)
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

def make_tarfile(sourceDir,config,ident):
    print "making geometry tarball"
    if os.path.isfile(sourceDir+"/scripts/z_config.tar.gz"):
        os.remove(sourceDir+"/scripts/z_config.tar.gz")
    tar = tarfile.open(sourceDir+"/scripts/z_config.tar.gz","w:gz")
    tar.add(sourceDir+"/build/prexsim",arcname="prexsim")
    tar.add(sourceDir+"/geometry",arcname="geometry")


    tar.close()

if __name__ == '__main__':
    main()

