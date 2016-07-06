# Compute HNF and solve diophantine matrix equations

## Build instructions
Requires libflint (>= 2.5.2) and libgmp to build.  Make sure the libraries and
includes files are in your search path.  Download the source using `git` and
build with `make`, e.g. as follows:

    $ git clone https://github.com/1k5/hnf_solve.git
    $ cd hnf_solve
    $ make


## Usage

### Hermite normal form
    $ ./hnf --help
    Usage: hnf [OPTION...] MATRIX-A
    Compute HNF of a multiple precision integer matrix.

      -H FILE                    Write HNF to FILE instead of stdout
      -U FILE                    Write unimodular transformation to FILE
      -?, --help                 Give this help list
          --usage                Give a short usage message
      -V, --version              Print program version

    All files should be in MatrixMarket v2.0 'matrix coordinate integer
    general' format.

    Report bugs to Malte Dehling <malte.dehling@uni-goettingen.de>.

Try e.g. the following:

    $ ./hnf test/A.mtx
    Computing HNF... done.
    %%MatrixMarket matrix coordinate integer general
    24 15 38
    1 1 1
    2 2 1
    3 1 -1
    [...]
    23 12 1
    24 4 1
    24 6 -1
    24 13 1

### Diophantine equation solver
    $ ./solve --help
    Usage: solve [OPTION...] MATRIX-A VECTOR-b
    Solve system of linear diophantine equations via HNF.

      -x FILE                    Write solution vector x to FILE instead of
                                 stdout
      -?, --help                 Give this help list
          --usage                Give a short usage message
      -V, --version              Print program version

    All files should be in MatrixMarket v2.0 'matrix coordinate integer
    general' format.

    Report bugs to Malte Dehling <malte.dehling@uni-goettingen.de>.

Try e.g. the following:

    $ ./solve test/A.mtx test/b.mtx
    Computing HNF... done.
    %%MatrixMarket matrix coordinate integer general
    15 1 4
    2 1 -1
    8 1 -1
    10 1 -1
    15 1 -1


## Acknowledgements
The actual work is done using FLINT's HNF implementations by Alex Best.  The
FLINT library is mainly developed by William Hart, Fredrik Johansson, and
Sebastian Pancratz.
This repository includes a modified version of the MatrixMarket IO routines
available at http://math.nist.gov/MatrixMarket/mmio-c.html .


[//]: # ( vim: set tw=78 ts=4 sw=4 et ai: )
