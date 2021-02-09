#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "fitsio.h"

using namespace std;

int main(int argc, char *argv[]){   
    fitsfile *fptr;   /* FITS file pointer, defined in fitsio.h */
    fptr = (fitsfile*)malloc(100000);
    int hdunums = 0;
    int status = 0;   /* CFITSIO status value MUST be initialized to zero! */

    int bitpix, naxis, ii, anynul;
    long naxes[2] = {1,1}, fpixel[2] = {1,1};
    double *pixels;
    char format[20], hdformat[20];


    if (!fits_open_file(&fptr, "/Users/oliveira/Downloads/SPLUS-s37s37_R_swp.fz", READONLY, &status))
    
        fits_get_num_hdus(fptr, &hdunums, &status);
        cout << "number of HDUs:";cout << hdunums << endl;

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

    {   
        
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
            

            /* loop over all the rows in the image, top to bottom */
            float rowmean = 0;
            float rowsum = 0;
            int Xcount = 0;
            int Ycount = 0;

            int lastpixel;

            int Xcord = 0;
            int Ycord = 0;

            int counted = 0;
            for (fpixel[1] = naxes[1]; fpixel[1] >= 1; fpixel[1]--){
                if (fits_read_pix(fptr, TDOUBLE, fpixel, naxes[0], NULL, pixels, NULL, &status) ){  /* read row of pixels */
                  break;  /* jump out of loop on error */
                }

                rowsum = 0;
                lastpixel = 0;
                Xcount ++;

                Ycount = 0;
                for (ii = 0; ii < naxes[0]; ii++){
                    lastpixel = pixels[ii];
                    Ycount ++;
                    if (counted != 1 && lastpixel == 0 && pixels[ii] != 0){
                        Xcord = Xcount;
                        Ycord = Ycount;

                        counted = 1;
                    }


                    rowsum = rowsum + pixels[ii];
                }    
                
            }
            free(pixels);
            cout << Xcord << " " << Ycord << " " << Ycount << endl;
          }
        }
        fits_close_file(fptr, &status);
    } 

    if (status) fits_report_error(stderr, status); /* print any error message */
    return(status);
}