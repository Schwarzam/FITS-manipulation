//
//  main.cpp
//  fist-test
//
//  Created by oliveira on 06/11/20.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fitsio.h"
#include <complex.h>

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
    char *createfile = "!/home/schwarzam/FITS-manipulation/file.fits";
    fits_create_file(&data, createfile, &status);

    // Copy FITS
    // fits_copy_file(fptr, data, 1, 1, 1, &status);

    //Copy only section
    char *section = "1701:2000, 1701:2000";
    fits_copy_image_section(fptr, data, section, &status);


    // Get Values from header
    // char *string = "CRVAL1";
    // fits_read_str(fptr, string, card, &status);
    // cout << card << endl;
    //    //    //

    double CRVAL1;
    fits_read_key(fptr, TDOUBLE, "CRVAL1", &CRVAL1, NULL, &status);
    cout << CRVAL1 << endl;

    double CRVAL2;
    fits_read_key(fptr, TDOUBLE, "CRVAL2", &CRVAL2, NULL, &status);
    cout << CRVAL2 << endl;

        double CRPIX1;
    fits_read_key(fptr, TDOUBLE, "CRPIX1", &CRPIX1, NULL, &status);
    cout << CRPIX1 << endl;

    double CRPIX2;
    fits_read_key(fptr, TDOUBLE, "CRPIX2", &CRPIX2, NULL, &status);
    cout << CRPIX2 << endl;

    double CD1_1;
    fits_read_key(fptr, TDOUBLE, "CD1_1", &CD1_1, NULL, &status);
    cout << CD1_1 << endl;

    double CD2_2;
    fits_read_key(fptr, TDOUBLE, "CD2_2", &CD2_2, NULL, &status);
    cout << CD2_2 << endl;

    double xpos = 48.7;
    double ypos = -(30.900001);
    double *xpix;
    double *ypix;

    double divisao = xpos/ypos;

    *ypix = xpos/ypos;
    *xpix = xpos/ypos;

    double xinc = CD1_1;
    double yinc = CD2_2;

    double tangente = tan(divisao);
    // tangente devia dar -0.27514232

    double rot = 1 / tangente;
    cout << rot << endl;
    fits_world_to_pix(xpos , ypos, CRVAL1, CRVAL2, CRPIX1, CRPIX2, xinc, yinc, rot, "--TAN", xpix, ypix, &status);
    cout << *xpix << endl;
    cout << *ypix << endl;

    // Its necessary to close the file to save changes.
    fits_close_file(data, &status);


    printf("END\n\n");  /* terminate listing with END */
    fits_close_file(fptr, &status);

    if (status)          /* print any error messages */
      fits_report_error(stderr, status);

    return(status);
}
