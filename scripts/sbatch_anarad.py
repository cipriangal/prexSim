#!/usr/bin/python
from subprocess import call
import subprocess
import sys,os,time

def create_bash_script(job_string, file_title):    
    f = open(file_title + '.sh','w')
    f.write("#!/bin/bash\n")
    f.write("#SBATCH --ntasks=1\n")
    f.write("#SBATCH --ntasks-per-node=1\n")
    f.write("#SBATCH --time=01:00:00\n")
    f.write("#SBATCH --output=" + file_title + ".out\n")
    f.write("#SBATCH --error=" + file_title + ".err\n")
    #f.write("#SBATCH --mail-type=ALL\n")
    #f.write("#SBATCH --mail-user=" + email + "\n")
    f.write("#SBATCH --partition=serial\n")
    f.write(job_string + "\n")
    f.close()

def anarad_string(run_num, identifier):
	event_count = 500000
	e_cut = str(30)
	short_events = str(event_count/1000) + 'k'
	config = 'crex5'
	#config = 'prexII'
	dir_prefix = config + 'crex5_' + short_events + 'Ev_'
	filename = 'o_prexSim.root'
	outfile_prefix = config + '_'
	sensative_dets = [10013]

	file_num = '%04d'%run_num
	infile = '../output/' + dir_prefix + file_num + '_' + identifier + '/' + filename
	outfile = outfile_prefix + file_num + '_' + identifier

	det_str = ''
	for det in sensative_dets:
		det_str += str(det) + ' '

	fn_str  = '../build/anaRad'
	arg_str = e_cut + ' ' + str(event_count) + ' ' + infile + ' ' + outfile + ' ' + det_str
	bash_command = fn_str + ' ' + arg_str

	return bash_command

#Initialize stuff pertaining to files and locations
identifier = 'shieldless'
outdir = 'analysis'
start_run = 0
end_run = 80

#Loop over events
for n in range(start_run, end_run):
	#Write the anarand command string
	job = anarad_string(n, identifier)

	#Create and write the sh file containing that string
	file_title = outdir + "/anarad" + "_" + '%04d'%n + '_' + identifier
	create_bash_script(job, file_title)

	#Submit as batch
	print("Submitting: " + job)
	call(["sbatch", file_title + '.sh'])

	
#Usage: anaRad [eCut for medium->HE in MeV][number of events] [input file name] [output file name] [list of sensative detectors #]
# for example: build/anaRad 10 1e6 o_HAPPEX2_1e6.root HAPPEX2_1e6 10008 10009 2001 2002 8002 8003
#   Output files will be of the type: output/anaRad_OutputFileName.root

