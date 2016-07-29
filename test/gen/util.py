import sys
import numpy as np

def dims(values):
    return (len(values), len(values[0])

def print_values(values):
    for row in values:
        print(*row);

def get_columns(values):
    m, n = dims(values)
    cols = ()
    for j in range(0, n):
        cols += (np.zeros(m),)
    for i in range(0, m):
        for j in range(0, n):
            cols[j][i] = values[i][j]
    return cols
