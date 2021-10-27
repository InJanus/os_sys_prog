# this is used to just generate the numbers for the c function
# generate numbers then output to file
# Brian Culberson 10/26/21

import sys
import random
from time import time

def main(length):
    # generate a list of numbers
    random.seed(time())
    lower_limit = 1
    upper_limit = int(length)*4
    f = open("output.txt", "w")

    for i in range(int(length)):
        f.write(str(random.randint(lower_limit, upper_limit)))
        f.write("\n")
    f.close()

if __name__ == "__main__":
    for i, v in enumerate(sys.argv):
        if v == '-v' or v == '--value':
            #next value is number of variables
            main(sys.argv[i+1])
            exit(0)