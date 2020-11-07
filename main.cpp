//
//  main.cpp
//  fist-test
//
//  Created by oliveira on 06/11/20.
//
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "fitsio.h"

#include <iostream>
using namespace std;


int main(int argc, char ** argv) {
    fitsfile *fptr;

    cout << argv[1] << endl;

    char card[FLEN_CARD];
    int status = 0,  nkeys, ii;  /* MUST initialize status */

    fits_open_file(&fptr, argv[1], READONLY, &status);

    // Number of HDUs
    int hdunums = 0;
    fits_get_num_hdus(fptr, &hdunums, &status);
    cout << "number of HDUs:";cout << hdunums <<endl;

    //current HDU
    int currhdu = 0;
    fits_get_hdu_num(fptr, &currhdu);
    cout << "Current HDU:";cout << currhdu <<endl;

    //Go to different HDU 
    int *hdutype = NULL;
    int nmove = 1;
    fits_movrel_hdu(fptr, nmove, hdutype, &status);

    fits_get_hdu_num(fptr, &currhdu);
    cout << "Current HDU:";cout << currhdu <<endl;

    // nkeys
    fits_get_hdrspace(fptr, &nkeys, NULL, &status);
    for (ii = 1; ii <= nkeys; ii++)  {
      fits_read_record(fptr, ii, card, &status); /* read keyword */
      printf("%s\n", card);
    }

    // create a new empty FITS.
    fitsfile *data;
    char *createfile = "!/Users/oliveira/Documents/fist-test/file.fits";
    fits_create_file(&data, createfile, &status);

    // Copy FITS
    // fits_copy_file(fptr, data, 1, 1, 1, &status);

    //Copy only section
    char *section = "1701:2000, 1701:2000";
    fits_copy_image_section(fptr, data, section, &status);


    // Its necessary to close the file to save changes.
    fits_close_file(data, &status);


    printf("END\n\n");  /* terminate listing with END */
    fits_close_file(fptr, &status);

    if (status)          /* print any error messages */
      fits_report_error(stderr, status);

    return(status);
}
