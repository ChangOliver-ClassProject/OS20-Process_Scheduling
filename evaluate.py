
def calc_time_unit(path = "output/TIME_MEASUREMENT_dmesg.txt"):
	with open(path, "r") as f:
		time_unit = 0;
		for line in f.readlines():
			_, pid, start, end = line.strip().split(' ')
			time_unit += float(end) - float(start)
		time_unit /= 5000
		return time_unit	

def cmp(unit, calc_path, stdout_path, dmesg_path, output):

	process = {}
	# process name: converted_theory_start_time, converted_theory_end_time
	#			converted_theory_elapse_time, real_start_time, real_end_time
	# 			real_elapse_time]

	nprocess = 0
	theory_start = 999999999999999
	theory_end = 0
	real_start = 999999999999999
	real_end = 0

	with open(calc_path, "r") as f:
		for line in f.readlines():
			name, start, end = line.strip().split(' ')
			converted = (float(end) - float(start)) * unit
			process[name] = [float(start), float(end), converted]
			nprocess += 1
			if float(start) < theory_start:
				theory_start = float(start)
			if float(end) > theory_end:
				theory_end = float(end)

	name_to_pid = {}
	with open(stdout_path, "r") as f:
		for line in f.readlines():
			name, pid = line.strip().split(' ')
			name_to_pid[pid] = name

	with open(dmesg_path, "r") as f:
		for line in f.readlines():
			_, pid, start, end = line.split(' ')
			process[name_to_pid[pid]].append(float(start))
			process[name_to_pid[pid]].append(float(end))
			process[name_to_pid[pid]].append(float(end)-float(start))
			if float(start) < real_start:
				real_start = float(start)
			if float(end) > real_end:
				real_end = float(end)			

	theory_total_time = (theory_end - theory_start) * unit
	real_total_time = (real_end - real_start)
	
	with open(outpath, 'w') as f:
		f.write('Theoretical Total Time,{}\n'.format(theory_total_time))
		f.write(' , ,Name,Start,End,Elapsed Time\n')
		for n in range(1, nprocess+1):
			idx = 'P'+ str(n)
			f.write(' , ,{},{},{},{}\n'.format(idx, (process[idx][0]-theory_start)*t_unit, (process[idx][1]-theory_start)*t_unit, process[idx][2]))
		
		f.write('Real Total Time,{}\n'.format(real_total_time))
		f.write(' , ,Name,Start,End,Elapsed Time\n')
		for n in range(1, nprocess+1):
			idx = 'P'+ str(n)
			f.write(' , ,{},{},{},{}\n'.format(idx, process[idx][3]-real_start, process[idx][4]-real_start, process[idx][5]))
		
		f.write('Error (Real-Theoretical) Total Time,{}\n'.format(real_total_time-theory_total_time))
		f.write(' , ,Name,Start,End,Elapsed Time\n')
		for n in range(1, nprocess+1):
			idx = 'P'+ str(n)
			f.write(' , ,{},{},{},{}\n'.format(idx, abs(process[idx][3]-real_start-process[idx][0]), abs(process[idx][4]-real_start-process[idx][1]), abs(process[idx][5]-process[idx][2])))
			


t_unit = calc_time_unit()
print(t_unit)

case = ['FIFO', 'SJF', 'PSJF', 'RR']

for i in case:
	for n in range(1, 6):
		calc_path = 'calc/theoretical/' + i + '_' + str(n) + '_calc.txt'
		stdout_path = 'output/' + i + '_' + str(n) + '_stdout.txt'
		dmesg_path = 'output/' + i + '_' + str(n) + '_dmesg.txt'
		outpath = 'compare/' + i + '_' + str(n) + '_cmp.csv'
		cmp(t_unit, calc_path, stdout_path, dmesg_path, outpath)

