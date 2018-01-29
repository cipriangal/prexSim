#!/bin/python

def main():
    print "generate some temp files with gdml code to be copy pasted somewhere else"
    beamPREX1()
    beamPREX1v2()
    beamPREX2()

def beamPREX1():
    f1=open("beamPipePREX1.temp","w")

    f1.write("\t<solids>\n")
    nSlices=13
    ## determined by JM see haplog XXXX
    ### everything is in cm
    rMin=[ 4.128,  4.445,  4.763,  5.080,  6.350,   7.620,   10.160,  10.478,  12.700,  15.240,  30.480,   45.720,   45.720]
    rMax=[ 4.432,  4.750,  5.067,  5.385,  6.655,   7.925,   10.478,  10.795,  13.018,  15.558,  30.798,   46.038,   46.038]
    bLen=[28.018, 19.010, 15.240, 37.028, 23.416,  27.622,   22.368,  94.456,  30.480, 106.204, 551.180, 1564.642,  554.776]
    bCen=[14.009, 37.523, 54.648, 80.782, 111.004, 136.523, 161.518, 219.930, 282.398, 350.740, 679.432, 1737.343, 2797.052]
    septumRmax=4.102
    f1.write("\t\t<polycone aunit=\"deg\" startphi=\"0\" deltaphi=\"360\" lunit=\"cm\" name=\"beamPipe_solid\">\n")
    f1.write("\t\t\t<zplane rmin=\""+str(septumRmax)+"\" rmax=\""+str(rMax[0])+"\" z=\""+str(-1)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\""+str(septumRmax)+"\" rmax=\""+str(rMax[0])+"\" z=\""+str(0)+"\"/>\n")
    for i in range(0,nSlices):
        print i
        f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i])+"\" z=\""+str(bCen[i]-bLen[i]/2)+"\"/>\n")
        if i==12:
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i])+"\" z=\""+str(bCen[i]+bLen[i]/2)+"\"/>\n")
        else:
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i])+"\" z=\""+str(bCen[i]+bLen[i]/2-1)+"\"/>\n")

        if i<12:
            print i,"step 2"
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i+1])+"\" z=\""+str(bCen[i]+bLen[i]/2 -1)+"\"/>\n")
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i+1])+"\" z=\""+str(bCen[i]+bLen[i]/2)+"\"/>\n")
    f1.write("</polycone>\n")
    f1.write("<position name=\"beamPipe_pos_rel\" unit=\"cm\" x=\"0\" y=\"0\" z=\"0\"/>\n")
    f1.close()

def beamPREX1v2():
    f1=open("beamPipePREX1v2.temp","w")

    f1.write("\t<solids>\n")
    nSlices=12
    ## determined by JM see haplog XXXX
    ### everything is in cm
    rMin=[ 4.128,  4.445,  4.763,  5.080,  6.350,   7.620,   10.160,  10.478,  12.700,  15.240,  30.480,   45.720 ]
    rMax=[ 4.432,  4.750,  5.067,  5.385,  6.655,   7.925,   10.478,  10.795,  13.018,  15.558,  30.798,   46.038 ]
    bLen=[28.018, 19.010, 15.240, 37.028, 23.416,  27.622,   22.368,  94.456,  30.480, 106.204, 551.180, 1459.843 ]
    bCen=[14.009, 37.523, 54.648, 80.782, 111.004, 136.523, 161.518, 219.930, 282.398, 350.740, 679.432, 1789.4435]
    septumRmax=4.102
    f1.write("\t\t<polycone aunit=\"deg\" startphi=\"0\" deltaphi=\"360\" lunit=\"cm\" name=\"beamPipe_solid\">\n")
    f1.write("\t\t\t<zplane rmin=\""+str(septumRmax)+"\" rmax=\""+str(rMax[0])+"\" z=\""+str(-1)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\""+str(septumRmax)+"\" rmax=\""+str(rMax[0])+"\" z=\""+str(0)+"\"/>\n")
    for i in range(0,nSlices):
        print i
        f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i])+"\" z=\""+str(bCen[i]-bLen[i]/2)+"\"/>\n")
        if i==11:
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i])+"\" z=\""+str(bCen[i]+bLen[i]/2)+"\"/>\n")
        else:
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i])+"\" z=\""+str(bCen[i]+bLen[i]/2-1)+"\"/>\n")

        if i<11:
            print i,"step 2"
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i+1])+"\" z=\""+str(bCen[i]+bLen[i]/2 -1)+"\"/>\n")
            f1.write("\t\t\t<zplane rmin=\""+str(rMin[i])+"\" rmax=\""+str(rMax[i+1])+"\" z=\""+str(bCen[i]+bLen[i]/2)+"\"/>\n")

    zPos=bCen[-1]+bLen[-1]/2 + 330.2 ### (10 ft 7'' to front face of diffuser)
    f1.write("\t\t\t<zplane rmin=\""+str(rMin[-1])+"\" rmax=\""+str(rMax[-1])+"\" z=\""+str(zPos)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\"5.08\" rmax=\""+str(rMax[-1])+"\" z=\""+str(zPos)+"\"/>\n")
    zPos += 2.54 ## 1'' Al flange
    f1.write("\t\t\t<zplane rmin=\"5.08\" rmax=\""+str(rMax[-1])+"\" z=\""+str(zPos)+"\"/>\n")

    f1.write("</polycone>\n")
    f1.write("<position name=\"beamPipe_pos_rel\" unit=\"cm\" x=\"0\" y=\"0\" z=\"0\"/>\n")

    f1.write("\n\n")
    f1.write("<position name=\"berylliumWindow_pos\" unit=\"cm\" x=\"0\" y=\"0\" z=\"2850.835\"/>\n")
    f1.write("<position name=\"orifice_pos\" unit=\"cm\" x=\"0\" y=\"0\" z=\"2845.265\"/>\n")
    f1.write("<position name=\"orificeSupport_pos\" unit=\"cm\" x=\"0\" y=\"-37.5\" z=\"2846.54\"/>\n")
    f1.write("<box lunit=\"cm\" name=\"orifice_solid1\" x=\"40.64\" y=\"40.64\" z=\"2.54\"/>\n")
    f1.write("<box lunit=\"cm\" name=\"orificeSupport_solid1\" x=\"40.64\" y=\"15.64\" z=\"2.54\"/>\n")
    f1.write("\n")
    f1.close()

def beamPREX2():
    f1=open("beamPipePREX2.temp","w")

    f1.write("\t<solids>\n")
    nSlices=8
    #### 0-6 are for telescoping beampipe from target to exit beampipe. JLAB DWG. NO. 65620-D-38500-11
    #### 7 is a connection to 8inch gate valve. JLAB DWG. NO. 65620-C-38500-04
    ## pipe outer diameter in cm
    dMax     =[ 8.255,  8.890,   9.525, 10.160,  12.7,   15.240, 20.320,  34.290]
    ## pipe thickens in cm
    thickness=[ 0.308,  0.308,   0.308,  0.308,   0.308,  0.308,  0.211,   6.960]
    ## length in cm
    bLen=[     28.018, 19.010,  15.240, 37.028,  23.416, 27.623,  9.449,   1.905]
    ## center position in cm
    ##bCen=[     14.009, 37.523,  54.648, 80.782, 111.004,136.523,155.06 , 160.740]

    septumRmax=4.102
    f1.write("\t\t<polycone aunit=\"deg\" startphi=\"0\" deltaphi=\"360\" lunit=\"cm\" name=\"beamPipe_solid1\">\n")
    f1.write("\t\t\t<zplane rmin=\""+str(septumRmax)+"\" rmax=\""+str(dMax[0]/2)+"\" z=\""+str(-1)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\""+str(septumRmax)+"\" rmax=\""+str(dMax[0]/2)+"\" z=\""+str(0)+"\"/>\n")
    zPos = 0
    for i in range(0,nSlices-2):
        print i, "center",zPos
        f1.write("\t\t\t<zplane rmin=\""+str(dMax[i]/2 - thickness[i])+"\" rmax=\""+str(dMax[i]/2)+"\" z=\""+str(zPos)+"\"/>\n")
        zPos += bLen[i]
        f1.write("\t\t\t<zplane rmin=\""+str(dMax[i]/2 - thickness[i])+"\" rmax=\""+str(dMax[i]/2)+"\" z=\""+str(zPos-1)+"\"/>\n")

        print i,"step 2"
        f1.write("\t\t\t<zplane rmin=\""+str(dMax[i]/2 - thickness[i])+"\" rmax=\""+str(dMax[i+1]/2)+"\" z=\""+str(zPos -1)+"\"/>\n")
        f1.write("\t\t\t<zplane rmin=\""+str(dMax[i]/2 - thickness[i])+"\" rmax=\""+str(dMax[i+1]/2)+"\" z=\""+str(zPos)+"\"/>\n")

    f1.write("\t\t\t<zplane rmin=\""+str(dMax[6]/2 - thickness[6])+"\" rmax=\""+str(dMax[6]/2)+"\" z=\""+str(zPos)+"\"/>\n")
    zPos += bLen[6]
    f1.write("\t\t\t<zplane rmin=\""+str(dMax[6]/2 - thickness[6])+"\" rmax=\""+str(dMax[6]/2)+"\" z=\""+str(zPos-1.905)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\""+str(dMax[6]/2 - thickness[6])+"\" rmax=\""+str(dMax[7]/2)+"\" z=\""+str(zPos-1.905)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\""+str(dMax[6]/2 - thickness[6])+"\" rmax=\""+str(dMax[7]/2)+"\" z=\""+str(zPos-1.27)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\""+str(dMax[7]/2 - thickness[7])+"\" rmax=\""+str(dMax[7]/2)+"\" z=\""+str(zPos-1.27)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\""+str(dMax[7]/2 - thickness[7])+"\" rmax=\""+str(dMax[7]/2)+"\" z=\""+str(zPos)+"\"/>\n")
    f1.write("\t\t</polycone>\n")

    lengthGateValve=8##cm
    print "Gate valve center at: ",zPos+4
    p2nSlices=6
    #### 0 is a connection to 8inch gate valve. JLAB DWG. NO. 65620-C-38500-04 (placed after the gate valve)
    #### 1-5 connection to the wall of the hall. JLAB DWG NO 65620-E-38500-07
    #### 5 extended based on JLAB DWG NO 65620-E-38500-01
    ## pipe outer diameter in cm
    p2dMax     =[ 34.29 ,  20.955,  25.400,  30.480,  60.960,   91.440]
    ## pipe thickens in cm
    p2Thickness=[  6.96 ,   0.318,   0.318,   0.318,   0.318,    0.318]
    ## length in cm
    p2bLen=[       1.905,  94.456,  30.48 , 106.200, 551.180, 1578.780]
    ## center position in cm
    ##p2bCen=[     172.69 , 220.871, 283.34 , 351.680, 680.370, 1745.350]
    f1.write("\t\t<polycone aunit=\"deg\" startphi=\"0\" deltaphi=\"360\" lunit=\"cm\" name=\"beamPipe_solid2\">\n")
    zPos += lengthGateValve
    f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[0]/2 - p2Thickness[0])+"\" rmax=\""+str(p2dMax[0]/2)+"\" z=\""+str(zPos)+"\"/>\n")
    zPos += p2bLen[0]
    f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[0]/2 - p2Thickness[0])+"\" rmax=\""+str(p2dMax[0]/2)+"\" z=\""+str(zPos)+"\"/>\n")
    #f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[1]/2 - p2Thickness[1])+"\" rmax=\""+str(p2dMax[1]/2)+"\" z=\""+str(zPos)+"\"/>\n")
    for i in range(1,p2nSlices):
        zPos += p2bLen[i]/2
        print i
        f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[i]/2-p2Thickness[i])+"\" rmax=\""+str(p2dMax[i]/2)+"\" z=\""+str(zPos-p2bLen[i]/2)+"\"/>\n")
        if i==5:
            f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[i]/2-p2Thickness[i])+"\" rmax=\""+str(p2dMax[i]/2)+"\" z=\""+str(zPos+p2bLen[i]/2)+"\"/>\n")
        else:
            f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[i]/2-p2Thickness[i])+"\" rmax=\""+str(p2dMax[i]/2)+"\" z=\""+str(zPos+p2bLen[i]/2-1)+"\"/>\n")

        if i<5:
            print i,"step 2"
            f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[i]/2-p2Thickness[i])+"\" rmax=\""+str(p2dMax[i+1]/2)+"\" z=\""+str(zPos+p2bLen[i]/2 -1)+"\"/>\n")
            f1.write("\t\t\t<zplane rmin=\""+str(p2dMax[i]/2-p2Thickness[i])+"\" rmax=\""+str(p2dMax[i+1]/2)+"\" z=\""+str(zPos+p2bLen[i]/2)+"\"/>\n")
        zPos += p2bLen[i]/2

    #### the neck down before the difuser: JLAB DWG NO JL0009934
    ### conical neck down
    f1.write("\t\t\t<zplane rmin=\"45.402\" rmax=\"48.15\" z=\""+str(zPos)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\"47.67\" rmax=\"48.15\" z=\""+str(zPos+0.01)+"\"/>\n")
    zPos += 25.3
    f1.write("\t\t\t<zplane rmin=\"29.89\" rmax=\"30.37\" z=\""+str(zPos)+"\"/>\n")
    ### straight piece
    zPos += 233.6
    f1.write("\t\t\t<zplane rmin=\"29.89\" rmax=\"30.37\" z=\""+str(zPos)+"\"/>\n")
    #### center plate (donut) JLAB DWG NO JL0014254
    f1.write("\t\t\t<zplane rmin=\"4.13\" rmax=\"30.37\" z=\""+str(zPos)+"\"/>\n")
    zPos += 2
    f1.write("\t\t\t<zplane rmin=\"4.13\" rmax=\"30.37\" z=\""+str(zPos)+"\"/>\n")
    f1.write("\t\t\t<zplane rmin=\"29.89\" rmax=\"30.37\" z=\""+str(zPos)+"\"/>\n")
    donutZpos=zPos
    ### back to straigh piece
    zPos += 216.8
    f1.write("\t\t\t<zplane rmin=\"29.89\" rmax=\"30.37\" z=\""+str(zPos)+"\"/>\n")
    ### further neck down
    zPos += 15.6
    f1.write("\t\t\t<zplane rmin=\"15.23\" rmax=\"15.70\" z=\""+str(zPos)+"\"/>\n")
    zPos += 4.8
    f1.write("\t\t\t<zplane rmin=\"15.23\" rmax=\"15.70\" z=\""+str(zPos)+"\"/>\n")
    f1.write("\t\t</polycone>\n")

    f1.write("<position name=\"dumpICS1_beamL_pos\" unit=\"cm\" x=\"68\" y=\""+str(-27-83)+"\" z=\""+str(donutZpos-37+30.5)+"\"/>\n")
    f1.write("<position name=\"dumpICS2_beamL_pos\" unit=\"cm\" x=\"68\" y=\""+str(-27-166+41)+"\" z=\""+str(donutZpos+90-30.5)+"\"/>\n")
    f1.write("<position name=\"dumpICS3_beamL_pos\" unit=\"cm\" x=\"68\" y=\""+str(-27-83)+"\" z=\""+str(donutZpos+90+30.5)+"\"/>\n")

    f1.write("<position name=\"dumpICS1_beamR_pos\" unit=\"cm\" x=\"-68\" y=\""+str(-27-83)+"\" z=\""+str(donutZpos-37+30.5)+"\"/>\n")
    f1.write("<position name=\"dumpICS2_beamR_pos\" unit=\"cm\" x=\"-68\" y=\""+str(-27-166+41)+"\" z=\""+str(donutZpos+90-30.5)+"\"/>\n")
    f1.write("<position name=\"dumpICS3_beamR_pos\" unit=\"cm\" x=\"-68\" y=\""+str(-27-83)+"\" z=\""+str(donutZpos+90+30.5)+"\"/>\n")

    f1.write("<position name=\"dumpDoor_pos\" unit=\"cm\" x=\"0\" y=\"-42\" z=\""+str(donutZpos+318)+"\"/>\n")
    f1.write("<position name=\"dumpIC_beamR_pos\" unit=\"cm\" x=\"-68\" y=\""+str(-27-53)+"\" z=\""+str(donutZpos+90-30.5)+"\"/>\n")
    f1.write("<position name=\"dumpIC_beamL_pos\" unit=\"cm\" x=\"68\" y=\""+str(-27-53)+"\" z=\""+str(donutZpos+90-30.5)+"\"/>\n")

    f1.close()


if __name__ == '__main__':
    main()
