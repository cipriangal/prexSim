import config_reader as cr

def get_particle(key):
	if key == lg or key == mg or key == hg:   return 'g'
	elif key == le or key == me or key == he: return 'e'
	else: return 'n'
	
def get_energy(key):
	if key == lg or key == le or key == ln:   return '  0<E<0.1  '
	elif key == mg or key == me or key == mn: return ' 0.1<E<eCut'
	elif key == hg or key == he or key == hn: return ' eCut<E<1000'
	elif key == hnt: return 'target eCut<E<1000'
	elif key == hnc: return 'collimator eCut<E<1000'
	
##########################################################################
#|\      /|     /\     ======= |\    |      |====== |=======| |\   |==== #
#| \    / |    /  \       |    | \   |      |       |       | | \  |     #
#|  \  /  |   /----\      |    |  \  |      |       |       | |  | |==== #
#|   \/   |  /      \     |    |   \ |      |       |       | | /  |     #
#|        | /        \ ======= |    \|      |====== |=======| |/   |==== #
##########################################################################

start_file = cr.start_run()
end_file = cr.end_run()
sensative_det = 10013
#config = 'crex5'
config = cr.config()
identifier = cr.identifier()
dir_path = 'output/'
f_prefix = 'o_' + config + '_'
output_filename = config + '_summed_out_' + identifier

e = 'energy'; f = 'flux'

lg = 'low_energy_gammas';    mg = 'med_energy_gammas';    hg = 'high_energy_gammas'
le = 'low_energy_electrons'; me = 'med_energy_electrons'; he = 'high_energy_electrons'
ln = 'low_energy_neutrons';  mn = 'med_energy_neutrons';  hn = 'high_energy_neutrons'
hnt = 'high_energy_neutrons_target'
hnc = 'high_energy_neutron_coll'

keys = [lg, mg, hg, le, me, he, ln, mn, hn, hnt, hnc]

summed_data = {e: {}, f: {}}

for key in keys:
	summed_data[e][key] = 0; summed_data[f][key] = 0

for n in range(start_file, end_file):
	f_num    = '%04d'%n
	
	fname = dir_path + f_prefix + f_num + '_' + identifier + '_SV' + str(sensative_det) + '_powerFlux.dat'
	
	infile = open(fname)
	lines = infile.readlines()
	tokens = lines[35].split()
	summed_data[e][keys[9]] += float(tokens[3])
	summed_data[f][keys[9]] += int(tokens[4])
	tokens = lines[36].split()
	summed_data[e][keys[10]] += float(tokens[3])
	summed_data[f][keys[10]] += int(tokens[4])
	for i in range(0, len(keys)-2):
		tokens = lines[i].split()
		summed_data[e][keys[i]] += float(tokens[2])
		summed_data[f][keys[i]] += int(tokens[3])

	infile.close()
	
outfile = open('new_' + output_filename + '.dat', 'w+')
for i in range(0, len(keys)):
	config_str = get_particle(keys[i]) + '\t' + get_energy(keys[i])
	number_str = '%.3f'%(summed_data[e][keys[i]]) + '\t' + str(summed_data[f][keys[i]])
	outfile.write(config_str + '\t' + number_str + '\n')
	
outfile.close()
