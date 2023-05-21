import subprocess
from astropy.io import fits
import io

# Run the C program and capture the output
process = subprocess.run(["./cut_fits"], capture_output=True, check=True)

# Convert the output into a FITS file with Astropy
hdu_list = fits.open(io.BytesIO(process.stdout))
data = hdu_list[0].data  # this is a NumPy array