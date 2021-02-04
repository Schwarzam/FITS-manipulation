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
    
    double CRVAL1;
    double CRVAL2;

    double CRPIX1;
    double CRPIX2;

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
    hdutype = (int*) malloc(16);

    int nmove = 1;
    fits_movrel_hdu(fptr, nmove, hdutype, &status);

    fits_get_hdu_num(fptr, &currhdu);
    cout << "Current HDU:";cout << currhdu <<endl;

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
    section = (char*) malloc(512);
    
    int bitpix, naxis, anynul;
    long naxes[2] = {1,1}, fpixel[2] = {1,1};
    double *pixels;
    char format[20], hdformat[20];

    int count = 0;
    double sum = 0;


    int loop;
    if (!fits_get_img_param(fptr, 2, &bitpix, &naxis, naxes, &status) )
        {
          if (naxis > 2 || naxis == 0)
             printf("Error: only 1D or 2D images are supported\n");
          else
          {
            /* get memory for 1 row */
            pixels = (double *) malloc(naxes[0] * sizeof(double));

            if (pixels == NULL) {
                printf("Memory allocation error\n");
                return(1);
            }
            
            
            for (fpixel[1] = Yend; fpixel[1] >= Ystart; fpixel[1]--){
                if (fits_read_pix(fptr, TDOUBLE, fpixel, naxes[0], NULL,
                    pixels, NULL, &status) )  /* read row of pixels */
                break;  /* jump out of loop on error */

                 /* print row number */
                
                for (loop = Xstart; loop < Xend + 1; loop++){
                    count ++;
                    sum = sum + pixels[loop];
                }
                    
            }

            float mean;
            mean = sum / count;
            cout << count << "     " << sum << "    " <<  mean << endl;
            free(pixels);
          }
        }

    printf("END\n\n");  /* terminate listing with END */
    fits_close_file(fptr, &status);

    if (status)          /* print any error messages */
      fits_report_error(stderr, status);

    return(status);
}
