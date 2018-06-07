# This is the supporting material of the paper entitled 
# "MILP-aided bit-based division property for primitives with non-bit-permutation linear layers". 
# Please find the paper at https://eprint.iacr.org/2016/811.pdf.

# For more information, feedback or questions, please contact lingsun@mail.sdu.edu.cn.
# Implemented by Sun Ling, Key Laboratory of Cryptologic Technology and Information Security, 
# Ministry of Education, Shandong University, Jinan 250100, China.

import sys
from gurobipy import *
import time

try:
	B = list([])
        TotalB = 0
	for i in range(100):
		file = "result"+str(i)+".lp"
		m=read(file)
		m.Params.Threads=4
		m.optimize()
		if (m.Status == 3):
			B = B + [i]
			TotalB += 1
	print(str(B))
	f=open('summatrize.dat','a')
	f.write(str(B))
	f.write('\n')
	f.write(str(TotalB))
	f.close();
	
except GurobiError:
    print ('Error reported')
