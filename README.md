# dbotsw
C++ implementation of "Dense Bag-of-Temporal-SIFT-Words"

# Requirements
dbotsw needs the following external librairies (alphabetic order):
- Boost
- LibSVM
- OpenCV

# Compilation
> make clean && make dbotsw

# Example
To launch the example:
- go to dbotsw/
- compile according to the instruction
- execute with: *./bin/dbotsw --p example/param/1 --f example/data/ --o example/output/*
- or execute with: */bin/dbotsw --p example/param/1 --t example/data/data_TRAIN --x example/data/data_TEST --o example/output/*

# Options
Execution options (alphabetic order):
- *--f* : Folder 'xx' containing the data under the format xx/xx_TRAIN + xx/xx_TEST
- *--o* : Output folder
- *--p* : Parameter file
- *--t* / *--x* : Training & Testing files

# Parameter file
An example of good parameter file is given in example/param/1.

Possible parameters that can be included in the parameter file :
sz_block, n_blocks, k (k-means), c_svm, n_sc, k_fold, dense, norm_bow, norm_bow_ssr...

# Extraction of feature vectors

Main program to extract feature vectors (and work directly from them) is available.

> make clean && make get_feature_vectors

Require options *--f* and *--o*
