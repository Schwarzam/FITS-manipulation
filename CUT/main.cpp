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
#include <cmath>

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
    for (ii = 1; ii <= 10; ii++)  {
      fits_read_record(fptr, ii, card, &status); /* read keyword */
      //printf("%s\n", card);
    }

    // create a new empty FITS.
    fitsfile *data;
    char *createfile = argv[5];
    //char *createfile = "!/home/gustavo/django/FITS-manipulation/file.fits";
    fits_create_file(&data, createfile, &status);

    // Copy FITS
    // fits_copy_file(fptr, data, 1, 1, 1, &status);

    //Copy only section

    // Get Values from header
    // char *string = "CRVAL1";
    // fits_read_str(fptr, string, card, &status);
    // cout << card << endl;
    //    //    //

    double CRVAL1;
    // fits_read_key(fptr, TDOUBLE, "CRVAL1", &CRVAL1, NULL, &status);
    // cout << CRVAL1 << endl;

    double CRVAL2;
    // fits_read_key(fptr, TDOUBLE, "CRVAL2", &CRVAL2, NULL, &status);
    // cout << CRVAL2 << endl;

    double CRPIX1;
    // fits_read_key(fptr, TDOUBLE, "CRPIX1", &CRPIX1, NULL, &status);
    // cout << CRPIX1 << endl;

    double CRPIX2;
    // fits_read_key(fptr, TDOUBLE, "CRPIX2", &CRPIX2, NULL, &status);
    // cout << CRPIX2 << endl;

    char* pEnd;
    double xpos = strtod(argv[2], &pEnd);
    double ypos = strtod(argv[3], &pEnd);

    double *yrefval;
    yrefval = (double*) malloc(120);
    double *xrefval;
    xrefval = (double*) malloc(120);

    double *xrefpix;
    xrefpix = (double*) malloc(120);
    double *yrefpix;
    yrefpix = (double*) malloc(120);

    double *xpix;
    xpix = (double*) malloc(120);
    double *ypix;
    ypix = (double*) malloc(120);

    double *rot;
    rot = (double*) malloc(120);
    char coordtype[16];

    double *xinc;
    xinc = (double*) malloc(120);
    double *yinc;
    yinc = (double*) malloc(120);

    fits_read_img_coord(fptr, xrefval, yrefval, xrefpix, yrefpix, xinc, yinc, rot, coordtype, &status);

    //fits_world_to_pix(xpos , ypos, CRVAL1, CRVAL2, CRPIX1, CRPIX2, xinc, yinc, rot, "--TAN", xpix, ypix, &status);
    cout << *xrefval << endl;
    cout << *yrefval << endl;
    cout << *xinc << endl;
    cout << *yinc << endl;
    cout << *rot << endl;
    cout << coordtype <<endl;

    double x = *xinc;
    double y = *yinc;
    double refvalx = *xrefval;
    double refvaly = *yrefval;
    double refpixx = *xrefpix;
    double refpixy = *yrefpix;
    double incx = *xinc;
    double incyy = *yinc;
    double rott = *rot;

    fits_world_to_pix(xpos , ypos, refvalx, refvaly, refpixx, refpixy, incx, incyy, rott, coordtype, xpix, ypix, &status);
    cout << *xpix << endl;
    cout << *ypix <<endl;


    std::ostringstream strs;
    strs << *xpix;
    std::string str = strs.str();

    int xpixx = (int) *xpix;
    int ypixx = (int) *ypix;

    int size = (int) strtod(argv[4], &pEnd);
    int Xstart = xpixx - (size/2);
    int Xend = xpixx + (size/2) - 1;

    if (Xstart < 0){
      Xstart = 1;
    }
    if (Xend > 11000){
      Xend = 10999;
    }

    int Ystart = ypixx - (size/2);
    int Yend = ypixx + (size/2) - 1;

    if (Ystart < 0){
      Ystart = 1;
    }
    if (Yend > 11000){
      Yend = 10999;
    }

    std::stringstream ss;
    ss << Xstart << ":" << Xend << "," << Ystart << ":" << Yend;
    string newstr = ss.str();

    cout << newstr <<endl;

    char *section = const_cast<char*>(newstr.c_str());

    // int bitpix, naxis, anynul, loop;
    // long naxes[2] = {1,1}, fpixel[2] = {1,1};
    // double *pixels;
    // char format[20], hdformat[20];

    // int count = 0;
    // double sum = 0;

    // if (!fits_get_img_param(fptr, 2, &bitpix, &naxis, naxes, &status) )
    //     {
    //       if (naxis > 2 || naxis == 0)
    //          printf("Error: only 1D or 2D images are supported\n");
    //       else
    //       {
    //         /* get memory for 1 row */
    //         pixels = (double *) malloc(naxes[0] * sizeof(double));

    //         if (pixels == NULL) {
    //             printf("Memory allocation error\n");
    //             return(1);
    //         }
            
            
    //         for (fpixel[1] = (ypixx + 32); fpixel[1] >= (ypixx - 31); fpixel[1]--){
    //             if (fits_read_pix(fptr, TDOUBLE, fpixel, naxes[0], NULL,
    //                 pixels, NULL, &status) )  /* read row of pixels */
    //             break;  /* jump out of loop on error */

    //              /* print row number */
                
    //             for (loop = (xpixx - 32); loop <= (xpixx + 31); loop++){
    //                 count ++;
    //                 sum = sum + pow(pixels[loop], 2);
    //             }
                    
    //         }

    //         float mean;
    //         mean = sum / count;
    //         cout << "MEAN_CENTER 64x64:" << mean << endl;

    //         float rms;
    //         rms = sqrt(mean);

    //         cout << "RMS: " << rms << endl;
    //         free(pixels);
    //       }
    //     }


    //char *section = "8397:8797,6155:6555";
    fits_copy_image_section(fptr, data, section, &status);

    // Its necessary to close the file to save changes.
    fits_close_file(data, &status);

    fits_close_file(fptr, &status);

    if (status)          /* print any error messages */
      fits_report_error(stderr, status);
    
    cout << "\n" << "status: " << status << endl;
    return(status);
}
