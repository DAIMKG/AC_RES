import sys
if (len(sys.argv) != 3):
	print("usage; ...")
	exit()
fin = open(sys.argv[1],'r')
fout = open(sys.argv[2],'w')
for line in open(sys.argv[1]):
	if "\\" not in line:
		fout.write(line)
		