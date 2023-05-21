import ctypes
import numpy as np

from astropy.io import fits
import io

def check_status(status):
    if status.value != 0:
        error_message = ctypes.create_string_buffer(30)
        cfitsio.fits_get_errstatus(status, error_message)
        print(f"CFITSIO error: {error_message.value.decode()}")

# Load the cfitsio shared library.
cfitsio = ctypes.CDLL('/Users/gustavo/Downloads/cfitsio-4.1.0/lib/libcfitsio.dylib')

# Define the necessary cfitsio functions and types.
class FitsFile(ctypes.Structure):
    pass

FitsFilePtr = ctypes.POINTER(FitsFile)
fits_open_file = cfitsio.ffopen
fits_open_file.argtypes = [ctypes.POINTER(FitsFilePtr), ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_int)]
fits_create_memfile = cfitsio.ffinit
fits_create_memfile.argtypes = [ctypes.POINTER(FitsFilePtr), ctypes.POINTER(ctypes.POINTER(ctypes.c_char_p)), ctypes.POINTER(ctypes.c_int)]
fits_close_file = cfitsio.ffclos
fits_close_file.argtypes = [FitsFilePtr, ctypes.POINTER(ctypes.c_int)]
fits_copy_image_section = cfitsio.fits_copy_image_section
fits_copy_image_section.argtypes = [FitsFilePtr, FitsFilePtr, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int)]
fits_create_file = cfitsio.ffinit
fits_create_file.argtypes = [ctypes.POINTER(FitsFilePtr), ctypes.c_char_p, ctypes.POINTER(ctypes.c_int)]


# Now you can use these in your Python code:
status = ctypes.c_int(0)
infptr = FitsFilePtr()
outfptr = FitsFilePtr()

# Open the input file.
fits_open_file(ctypes.byref(infptr), b'proc_STRIPE82-20180814-082204.fits', 0, ctypes.byref(status))

fits_get_img_size = cfitsio.ffgisz
fits_get_img_size.argtypes = [FitsFilePtr, ctypes.c_int, ctypes.POINTER(ctypes.c_long), ctypes.POINTER(ctypes.c_int)]

# Get image size
naxes = (ctypes.c_long * 2)()  # assuming a 2D image
status = ctypes.c_int(0)

fits_get_img_size(infptr, 2, naxes, ctypes.byref(status))
print(naxes[0], naxes[1])

# Step 1: Create a block of memory to serve as the "file"
#memfile = ctypes.create_string_buffer(1000000)  # allocate 1 MB; adjust as needed

# Step 1: Create a block of memory to serve as the "file"
memfile = ctypes.create_string_buffer(1000000)  # allocate 1 MB; adjust as needed

# Step 2: Create a new FITS file in that block of memory
memfilename = f"mem://{ctypes.addressof(memfile)}"
status = ctypes.c_int(0)
fits_create_file(ctypes.byref(outfptr), memfilename.encode('ascii'), ctypes.byref(status))

check_status(status)
hdu_list = fits.open(io.BytesIO(memfile.raw))

# Select a section of the image.
section = b'1200:1400,1200:1400'  # This is an example, modify as needed.
fits_copy_image_section(infptr, outfptr, section, ctypes.byref(status))


# Close the files.
fits_close_file(infptr, ctypes.byref(status))
#fits_close_file(outfptr, ctypes.byref(status))

print(status)
print(memfile.raw.find(b'END'))
# Step 4: Access the memory block as a Python string, convert it to a byte array, and open it with astropy
hdu_list = fits.open(io.BytesIO(memfile.raw))
data = hdu_list[0].data  # this is a NumPy array
