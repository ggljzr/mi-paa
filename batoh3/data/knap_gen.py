import subprocess
import sys

gen_path = "/home/ggljzr/Documents/skola/paa/batoh3/knapgen"
knap_path = "/home/ggljzr/Documents/skola/paa/batoh3/src"

dest_file = gen_path + "/data/imp.dat"
inst_n = 10

def measure_for_params(n, m, max_weight, max_price, k, d):
	results = {'bb' : 0, 'dyn' : 0, 'greedy' : 0}
	gen_command_string = './knapgen -I 100 -n {} -N {} -m {} -W {} -C {} -k {} -d {} > {}'.format(n,inst_n,
												  m, max_weight, 
												  max_price, k, d,
												  dest_file)
	knap_command_string = './batoh ' + dest_file 

	gen_output = subprocess.check_output(gen_command_string, shell=True, cwd=gen_path)
	knap_output = subprocess.check_output(knap_command_string, shell=True, cwd=knap_path)
	raw_results = knap_output.split()

	results['bb'] = float(raw_results[0])
	results['dyn'] = float(raw_results[1])
	results['greedy'] = float(raw_results[2])
	results['error'] = float(raw_results[3])
	
	return results

def measure_max_weight(n=27, m=0.5, k=1, d=0, max_price=100, begin=100, end=1000, step=100):
	res_list = []
	for i in range(begin, end+step, step):
		res = measure_for_params(n,m,i,max_price,k,d)
		res_list.append((i, n, res))

	return res_list

def measure_max_price(n=27, m=0.5, k=1, d=0, max_weight=100, begin=100, end=1000, step=100):
	res_list = []
	for i in range(begin, end+step, step):
		res = measure_for_params(n,m,max_weight,i,k,d)
		res_list.append((i, n, res))

	return res_list

def measure_m(n=27, k=1, d=0, max_weight=100, max_price=100, begin=0.05, end=0.96, step=0.05):
	res_list = []
	i = begin
	while i <= end:
		res = measure_for_params(n,i,max_weight,max_price,k,d)
		res_list.append((i,n,res))
		i = i + step
	return res_list

def measure_k(n=27, m=0.5, d=1, max_weight=1000, max_price=100, begin=1.0, end=3.0, step=0.1):
	res_list = []
	i = begin
	while i <= end:
		res = measure_for_params(n, m, max_weight, max_price, i, d)
		res_list.append((i,n,res))
		i = i + step
	return res_list

def print_res(res):
	print('{:.2} {} {} {} {} {}'.format(res[0], res[1], res[2]['bb'], res[2]['dyn'], 
			                    res[2]['greedy'], res[2]['error']))

	
if __name__ == "__main__":

	res_list = []

	n = 30
	begin = 1000
	end = 20000
	step = 1000
	d = 0
	
	if 'weight' in sys.argv:
		res_list = measure_max_weight(n=n, d=d,begin=begin, end=end, step=step)
	elif 'price' in sys.argv:
		res_list = measure_max_price(n=n, d=d,begin=begin, end=end, step=step)
	elif '-m' in sys.argv:
		res_list = measure_m(n=n,d=1,k=2)
	elif '-k' in sys.argv:
		res_list = measure_k(n=n, d=-1)
	else:
		res_list = measure_max_price()

	for i in res_list:
		print_res(i)



