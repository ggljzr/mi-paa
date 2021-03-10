import subprocess
import sys

import time

cooling_start = 0.998
cooling = cooling_start
cooling_inc = 0.00001
delta = 0.99999 - cooling_start
steps = int(delta / cooling_inc)



inst_num = 50

def measure_error(n,k):
	try:
		subprocess.check_call("rm ../data/small/inst_{}_{}_cooling_e.log".format(n,k), shell=True)
	except subprocess.CalledProcessError:
		pass

	max_val = 0
	avg_errors = [0] * steps
	avg_times = [0] * steps
	error_index = 0

	for i in range(0,inst_num):
		sys.stderr.write("instance {} \n".format(i))
		cooling = cooling_start
		subprocess.check_call("./gen.out {} {} 500 > inst.dat".format(n,k), shell=True)

		error_index = 0
		max_val = 0
		for j in range(0,steps):
			try:
				subprocess.check_call("rm ./temp.log", shell=True)
			except subprocess.CalledProcessError:
				pass

			sat_string = "./sat.out inst.dat temp.log 80 {} 0.01 e".format(cooling)
			out = subprocess.check_output(sat_string, shell=True)
		
			current_val=int(out.split()[3])
			current_err=float(out.split()[7])
			current_time=float(out.split()[8])
	
			avg_errors[error_index] += current_err
			avg_times[error_index] += current_time

			#kdyz byl pro nastaveni vysledek lepsi nez nas dosud nalezenej
			if current_val > max_val:
				max_val = current_val
				subprocess.check_call("cp ./temp.log ./temp_max.log",shell=True)

			cooling += cooling_inc
			error_index += 1

		subprocess.check_call("cat ./temp_max.log >> ../data/small/inst_{}_{}_cooling_e.log".format(n,k), shell=True)	

	cooling = cooling_start
	for entry in zip(avg_errors, avg_times):
		err = entry[0] / inst_num
		time = entry[1] / inst_num
		print(str(cooling) + " " + str(err) + " " + str(time))
		cooling += cooling_inc

def measure_large(n,k):
	try:
		subprocess.check_call("rm ../data/large/inst_{}_{}_cooling_e.log".format(n,k), shell=True)
	except subprocess.CalledProcessError:
		pass

	max_val = 0
	avg_val = 0
	avg_time = 0
	cooling = cooling_start

	abs_max = 0
	
	loop_num = 10

	#subprocess.check_call("./gen.out {} {} 500 > inst.dat".format(n,k), shell=True)

	for i in range (0, steps):

		max_val = 0
		avg_val = 0
		avg_time = 0

		for j in range(0, loop_num):
			try:
				subprocess.check_call("rm ./temp.log", shell=True)
			except subprocess.CalledProcessError:
				pass

			sat_string = "./sat.out inst.dat temp.log 80 {} 0.01 t".format(cooling)
			out = subprocess.check_output(sat_string, shell=True)

			current_val = int(out.split()[2])
			current_time = float(out.split()[6])
		
			time.sleep(1)

			if current_val > max_val:
				max_val = current_val
				subprocess.check_call("cp ./temp.log ./temp_max.log",shell=True)
			avg_val += current_val
			avg_time += current_time
		
		print("{} {} {} {}".format(cooling, max_val, avg_val/loop_num, avg_time/loop_num))
		
		sys.stderr.write("done {} / {}\n".format(i, steps))

		if max_val > abs_max:
			abs_max = max_val
			subprocess.check_call("cp ./temp_max.log ../data/large/inst_{}_{}_cooling_e.log".format(n,k), shell=True)

		cooling += cooling_inc	

def success_rate():
	
	success = 0

	loop_num = 30
	max_clause = 150
	variables = 50

	for clause in range(variables,max_clause + 1,2):
		success = 0

		for j in range(0, loop_num):
			try:
				subprocess.check_call("rm ./temp.log", shell=True)
			except subprocess.CalledProcessError:
				pass
			try:
				subprocess.check_call("./gen.out {} {}  500 > inst.dat".format(variables, clause), shell=True)
			except subprocess.CalledProcessError:
				pass

			time.sleep(1)

			out = subprocess.check_output("./sat.out inst.dat temp.log 80 0.9999 0.01 t",shell = True)
			current_val = int(out.split()[2])
			if current_val > 0:
				success += 1
		
		sys.stderr.write("done {} / {}\n".format(clause,max_clause))
		print("{} {}".format(clause/40, success/loop_num))




if __name__ == "__main__":
	success_rate()


