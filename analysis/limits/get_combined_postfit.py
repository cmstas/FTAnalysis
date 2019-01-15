# see note in analysis/yields/py_doAll.py
import sys
sys.modules["numba"] = None
import uproot

from matplottery.utils import Hist1D, Hist2D
import numpy as np

# np.set_printoptions(linewidth=220,precision=3)

def get_total_postfit_vals_errs(fname, nchannels=3, which="shapes_prefit"):
    f = uproot.open(fname)

    # which = "shapes_fit_s"
    # which = "shapes_prefit"
    yields = f[which]["total_overall"] # s+b total for all channels stacked next to each other in one hist
    covariance = f[which]["overall_total_covar"] # s+b covariance

    tc = Hist2D(covariance).counts
    nbins = tc.shape[0]//nchannels
    cov4d = []
    for i in range(nchannels):
        row = []
        for j in range(nchannels):
            row.append(tc[nbins*i:nbins*(i+1),nbins*j:nbins*(j+1)])
        cov4d.append(row)
    cov4d = np.array(cov4d)

    """
    indices are
        - first year/channel (0,1,2 = 2016,2017,2018; x-axis in covar plot)
        - second year/channel (0,1,2 = 2016,2017,2018; y-axis in covar plot)
        - first channel bin number (5 (0-index) = 6th bin (1-index))
        - second channel bin number
    so cov4d[1,2,7,3] = cov(2017-8thbin,2018-4thbin)
    and NOTE that cov4d[1,2] = cov4d[2,1].T (since now the last two dimensions need to be swapped)
    cov(2016+2017+2018) = 2*(cov(2016,2017)+cov(2016,2018)+cov(2017,2018)) + cov(2016,2016) + cov(2017,2017) + cov(2018,2018)
    so we could've done: 2*(cov4d[0,2]+cov4d[0,1]+cov4d[1,2])+cov4d[0,0]+cov4d[1,1]+cov4d[2,2]
    but with the above note, it simplifies to just adding all submatrices, taking transpose for half of the off-diagonals
    """
    cov2d = np.zeros((nbins,nbins))
    for i in range(cov4d.shape[0]):
        for j in range(cov4d.shape[1]):
            if i > j: cov2d += cov4d[i,j].T
            else: cov2d += cov4d[i,j]
    # std dev is sqrt of diagonal, so this is a 1D array of bin errors
    errs = np.diag(cov2d)**0.5
    # reshape to have nchannels arrays of nbins each, then sum out nchannels to get total yield
    vals = Hist1D(yields).counts
    vals = vals.reshape(nchannels,-1).sum(0)

    return vals,errs

if __name__ == "__main__":

    fname = "fitDiagnosticsname.root"
    print get_total_postfit_vals_errs(fname, which="shapes_prefit")
