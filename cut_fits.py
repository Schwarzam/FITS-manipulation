import subprocess
from astropy.io import fits
import io

import argparse

parser = argparse.ArgumentParser(description='Cut a FITS file')
parser.add_argument('filepath', type=str, help='Path to the FITS file')
parser.add_argument('ra', type=float, help='RA of the target in degrees')
parser.add_argument('dec', type=float, help='Dec of the target in degrees')
parser.add_argument('radius', type=float, help='Radius of the cutout in pixels')

args = parser.parse_args()

ra = args.ra
dec = args.dec
radius = args.radius
filepath = args.filepath

# Run the C program and capture the output
process = subprocess.run(["./main", filepath, ra, dec, radius], capture_output=True, check=True)

# Convert the output into a FITS file with Astropy
hdu_list = fits.open(io.BytesIO(process.stdout))
data = hdu_list[0].data  # this is a NumPy array
print(data.shape)
