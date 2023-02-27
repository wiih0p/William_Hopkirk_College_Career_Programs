import os
import sys
import csv
import threading
import pandas as pd
import numpy as np

N = [25, 50, 75, 100]
Pm = [0, 0.01, 0.03, 0.05]
Pc = [0, 0.1, 0.3, 0.5]
T = [2, 3, 4, 5]
solution = np.full((40, ), 1)

NUM_THREADS = 0
COUNTER = 0
dataframes = []
def run(n, pm, pc, t, i):
    global NUM_THREADS
    global COUNTER
    for j in range(20):
        csv = "csvs/" + str(i) + "stg" + str(j) + ".csv"
        cmd = ("python lab2.py --n %s --p_m %s --p_c %s --trn_size %s --csv_output %s" % (n, pm, pc, t, csv))
        #print(cmd)
        os.system(cmd)
        while(os.path.exists(csv) == False):
            pass
        df = pd.read_csv(csv)
        dataframes.append([df, n, pm, pc, t])
        COUNTER += 1
        print(str(int((COUNTER / 5021) * 100)) + "%")
    NUM_THREADS -= 1

i = 0
for n in N:
    for pm in Pm:
        for pc in Pc:
            for t in T:
                while(NUM_THREADS >= 4):
                    pass

                #print(str(n) + " " + str(pm) + " " + str(pc) + " " + str(t) + " " + str(NUM_THREADS))
                NUM_THREADS += 1
                threading.Thread(target=run, args=[n, pm, pc, t, i], daemon=True).start()
                i += 1

while(NUM_THREADS > 0):
    pass
