## FITS image 2d Cutout

FITS cuts much much faster and with better memory performance than astropy Cutout2D

Need to compile with lcfitsio that may be installed with:

    apt-get install libcfitsio-dev
    
To compile:

    g++ main.cpp -o main -lcfitsio -w

Run cut_fits.py. 