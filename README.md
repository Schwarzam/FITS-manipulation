# FITS-manipulation

FITS cuts 100x faster and with better memory performance than astropy Cutout2D

CODE INSIDE 'CUT' FOLDER.

Need to compile with lcfitsio that may be installed with:

    apt-get install libcfitsio-dev
    
To compile:

    g++ main.cpp -o main -lcfitsio -w

To run cut:

    ./main path/to/fits RA DEC Size Output/path
