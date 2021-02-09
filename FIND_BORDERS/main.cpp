#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "fitsio.h"
#include <vector>
using std::vector; 

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

    vector<int> arrX;
    vector<int> arrY;

    if (!fits_open_file(&fptr, "/Users/oliveira/Documents/SPLUS-n06s01_F395_swpweight.fz", READONLY, &status))
    
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
            lastpixel = 0;

            double sum = 0;
            int count_to_mean = 0;
            for (fpixel[1] = naxes[1]; fpixel[1] >= 1; fpixel[1]--){
                if (fits_read_pix(fptr, TDOUBLE, fpixel, naxes[0], NULL, pixels, NULL, &status) ){  /* read row of pixels */
                  break;  /* jump out of loop on error */
                }
                for (ii = 0; ii < naxes[0]; ii++){
                    count_to_mean ++;
                    sum = sum + pixels[ii];
                }     
            }
            float mean = 0;
            mean = sum / count_to_mean;
            cout << mean << " " << sum << " " << count_to_mean << " " << endl;

            for (fpixel[1] = naxes[1]; fpixel[1] >= 1; fpixel[1]--){
                if (fits_read_pix(fptr, TDOUBLE, fpixel, naxes[0], NULL, pixels, NULL, &status) ){  /* read row of pixels */
                  break;  /* jump out of loop on error */
                }

                rowsum = 0;
                
                Ycount ++;

                Xcount = 0;
                for (ii = 0; ii < naxes[0]; ii++){
                    // cout << lastpixel << endl;
                    Xcount ++;

                    if (lastpixel < mean-400 && pixels[ii] > mean){
                        Xcord = Xcount;
                        Ycord = Ycount;

                        arrX.push_back(Xcount);
                        arrY.push_back(Ycount);
                        counted = 1;            
                    } 

                    if (lastpixel > mean && pixels[ii] < mean-400){
                        Xcord = Xcount;
                        Ycord = Ycount;

                        arrX.push_back(Xcount);
                        arrY.push_back(Ycount);
                        counted = 1;            
                    } 


                    lastpixel = pixels[ii];
                    rowsum = rowsum + pixels[ii];
                }    
                
            }
            free(pixels);
            cout << Xcord << " " << Ycord << " " << Ycount << endl;

            cout << "X_array: ";
            for (vector<int>::const_iterator i = arrX.begin(); i != arrX.end(); ++i){
                cout << *i << " ";
            }
            cout << "\n";

            cout << "Y_array: ";
            for (vector<int>::const_iterator i = arrY.begin(); i != arrY.end(); ++i){
                cout << *i << " ";
            }
            cout << "\n";

            
          }
        }
        fits_close_file(fptr, &status);
    } 

    if (status) fits_report_error(stderr, status); /* print any error message */
    return(status);
}