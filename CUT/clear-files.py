from os import walk
import numpy as np
import time
import os

loop = True
while loop:
    mypath = '/storage/splus/images_psql/jpgsGRI/files'

    f = []
    for (dirpath, dirnames, filenames) in walk(mypath):
        f.extend(filenames)
        break
    
    olds = np.array(filenames)
    
    time.sleep(600)
    
    f = []
    for (dirpath, dirnames, filenames) in walk(mypath):
        f.extend(filenames)
        break
    
    news = np.array(filenames)
    
    for item in olds:
        if item in news:
            os.system(f'rm {mypath}/"{item}"')