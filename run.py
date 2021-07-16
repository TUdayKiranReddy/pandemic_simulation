import numpy as np
import matplotlib.pyplot as plt
import os 

os.system("cc -g fast_sir_algo.c")
os.system("./a.out")

def read_file_to_array(filepath):
	a = []
	infile = open(filepath,'r')
	for line in infile:
		a.append(int(line[0:-1]))
	infile.close()
	return np.array(a)

times = read_file_to_array('times.dat')
no_of_susceptibles = read_file_to_array('susceptibles.dat')
no_of_infections = read_file_to_array('infections.dat')
no_of_recoveries = read_file_to_array('recoveries.dat')

os.remove('susceptibles.dat')
os.remove('infections.dat')
os.remove('recoveries.dat')

plt.title("FAST SIR Curves",loc='center')
plt.plot(times,no_of_susceptibles,label = 'Susceptibles')
plt.plot(times,no_of_infections,label = 'Infections')
plt.plot(times,no_of_recoveries,label = 'Recoveries')


plt.xlabel("Time in days")
plt.ylabel("#Infections")
plt.grid()
plt.legend()
plt.show()

