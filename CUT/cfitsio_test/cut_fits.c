// Import required libraries
#include "fitsio.h"
#include <stdio.h>

int main() {
    fitsfile *infptr;  // Input FITS file pointer
    fitsfile *outfptr; // Output FITS file pointer
    int status = 0;
    char *memblock = NULL;
    size_t memsize = 0;
    char section[] = "1200:1400,1200:1400";

    // Open the FITS file
    if (fits_open_file(&infptr, "proc_STRIPE82-20180814-082204.fits", READONLY, &status)) {
        fits_report_error(stderr, status);
        return(status);
    }

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
