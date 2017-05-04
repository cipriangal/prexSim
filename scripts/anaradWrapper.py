#!/usr/bin/python
#from subprocess import call
import subprocess
import sys,os,time

event_count = 500000
e_cut = str(30)
short_events = str(event_count/1000) + 'k'
identifier = '_concreteLHalfWidth45cut'
dir_prefix = 'crex5_' + short_events + 'Ev_'
filename = 'o_prexSim.root'
outfile_prefix = 'crex5_'
sensative_dets = [10013]

for n in range(0, 20):
	file_num = '%04d'%n
	infile = '../output/' + dir_prefix + file_num + identifier + '/' + filename
	outfile = outfile_prefix + file_num + identifier
	
	det_str = ''
	for det in sensative_dets:
		det_str += str(det) + ' '
	
	fn_str  = '../build/anaRad'
	arg_str = e_cut + ' ' + str(event_count) + ' ' + infile + ' ' + outfile + ' ' + det_str
	bash_command = fn_str + ' ' + arg_str
	
	print(bash_command)
	
	process = subprocess.Popen(bash_command.split(), stdout=subprocess.PIPE)
	output, error = process.communicate()
	
	print(output)
	print(str(error) + '\n\n')
	
#Usage: anaRad [eCut for medium->HE in MeV][number of events] [input file name] [output file name] [list of sensative detectors #]
# for example: build/anaRad 10 1e6 o_HAPPEX2_1e6.root HAPPEX2_1e6 10008 10009 2001 2002 8002 8003
#   Output files will be of the type: output/anaRad_OutputFileName.root

