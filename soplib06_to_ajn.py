def convert_sop06_2_ajn(filename):	
	f = open(filename, 'r')
	lines = f.readlines()
	matrix = [['0' if c != '-1' else '1' for c in l.strip().split('\t')] for l in lines]
	f.close()
	str_line = [' '.join(l) + '\n' for l in matrix]
	f = open(filename + '.ajn.txt', 'w')
	f.writelines(str(len(str_line))+',n\n')
	f.writelines((str_line))
	f.close()
	return str_line
