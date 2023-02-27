import os
import csv
import pandas as pd
N = [25, 50, 75, 100]
Pm = [0, .01, .03, .05]
Pc = [0, .1, .3, .5]
tourn = [2, 3, 4 ,5]
count = 0
for i in N:
    for j in Pm:
        for k in Pm:
            for l in tourn:
                for o in range(20):
                    join = "output" + str(count) + ".csv"
                    df = pd.read_csv(join)
                    # Write data to the rightmost blank columns
                    df.loc[1, 'N'] =  i
                    df.loc[1, 'Pm'] =  j
                    df.loc[1, 'Pc'] =  k
                    df.loc[1, 'tourn'] =  l
                    df.loc[1, 'iter'] =  o

                    # Save the updated DataFrame to a new CSV file
                    join2 = "z" + str(count) + ".csv"
                    df.to_csv(join2, index=False)
                    count+=1