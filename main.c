/*
Code to cut a fits image around a given position.
The code takes as input a fits file and a position (RA, DEC) and a size.
The code returns a fits file with the cut image in the stdout so it doesnt write on disk.

Written by Gustavo Schwarz 
2023-09-27

gcc -o cut_fits cut_fits.c -lcfitsio

./cut_fits <fits_file> <RA> <DEC> <size>

*/


#include "fitsio.h"
#include <stdio.h>

void move_image_hdu(fitsfile *infptr, int status){
    // Move HDU to the image if hdu exists
    if (fits_movabs_hdu(infptr, 2, NULL, &status)) {
        //fits_report_error(stderr, status);
        //return(status);
    }

    if (status == 0) {
        return;
    }else{
        status = 0;
        fits_movabs_hdu(infptr, 1, NULL, &status);
    }
}

int main(int argc, char ** argv) {
    fitsfile *infptr;  // Input FITS file pointer
    fitsfile *outfptr; // Output FITS file pointer
    int status = 0;
    void *memblock = NULL;
    size_t memsize = 0;

    // Open the FITS file
    char *filename = argv[1];
    if (fits_open_file(&infptr, filename, READONLY, &status)) {
        fits_report_error(stderr, status);
        return(status);
    }

    move_image_hdu(infptr, status);

    
    double ra = atof(argv[2]);
    double dec = atof(argv[3]);
    int size = atoi(argv[4]);

    double yrefval;
    
    double xrefval;
    double xrefpix;
    double yrefpix;
    double xpix;
    double ypix;
    double rot;
    char coordtype[16];
    double xinc;
    double yinc;

    fits_read_img_coord(infptr, &xrefval, &yrefval, &xrefpix, &yrefpix, &xinc, &yinc, &rot, coordtype, &status);
    fits_world_to_pix(ra , dec, xrefval, yrefval, xrefpix, yrefpix, xinc, yinc, rot, coordtype, &xpix, &ypix, &status);
    
    int Xstart = xpix - (size/2);
    int Xend = xpix + (size/2) - 1;

    int Ystart = ypix - (size/2);
    int Yend = ypix + (size/2) - 1;

    char section[100];
    sprintf(section, "%d:%d,%d:%d", Xstart, Xend, Ystart, Yend);

    // Create the memory file
    if (fits_create_memfile(&outfptr, &memblock, &memsize, 0, realloc, &status)) {
        fits_report_error(stderr, status);
        return(status);
    }

    // Copy the section
    if (fits_copy_image_section(infptr, outfptr, section, &status)) {
        fits_report_error(stderr, status);
        return(status);
    }

    float xcenter_original = 0.5*(Xend - Xstart);
    float ycenter_original = 0.5*(Yend - Ystart);

    char *comments;
    comments = (char*) malloc(32);
    comments = "Center position";

    char *keyname;
    keyname = (char*) malloc(32);
    keyname = "X01TILE";
    fits_write_key(outfptr, TFLOAT, keyname, &xcenter_original, comments, &status);
    keyname = "Y01TILE";
    fits_write_key(outfptr, TFLOAT, keyname, &ycenter_original, comments, &status);
    keyname = "X0TILE";
    float xcenter = xpix * 1;
    fits_write_key(outfptr, TFLOAT, keyname, &xcenter, comments, &status);
    keyname = "Y0TILE";
    float ycenter = ypix * 1;
    fits_write_key(outfptr, TFLOAT, keyname, &ycenter, comments, &status);
    

    // Close the files
    if (fits_close_file(infptr, &status)) {
        fits_report_error(stderr, status);
        return(status);
    }
    if (fits_close_file(outfptr, &status)) {
        fits_report_error(stderr, status);
        return(status);
    }

    // Write the memory block to stdout
    fwrite(memblock, 1, memsize, stdout);

    return 0;
}

