/**
 * fmpz_mat_mmio.c - MatrixMarket IO for FLINT.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 *
 * TODO: Fix error handling.
 */

#include <stdio.h>
#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>
#include "mmio.h"


/** Read MatrixMarket data from (open) FILE *fp as fmpz_mat_t M.
 */
int fmpz_mat_mm_read(FILE *fp, fmpz_mat_t M) {
    int ret = 1;
    MM_typecode matcode;

    if (mm_read_banner(fp, &matcode) != 0) {
        fprintf(stderr, "File format error.\n");
        ret = 0;
    } else if ( !(mm_is_matrix(matcode) && mm_is_coordinate(matcode) \
            && mm_is_integer(matcode) && mm_is_general(matcode)) ) {
        fprintf(stderr, "Unsupported MM format.\n");
        ret = 0;
    } else {
        long nrows, ncols, nnz;

        if (mm_read_mtx_crd_size(fp, &nrows, &ncols, &nnz) != 0) {
            fprintf(stderr, "Could not parse matrix size.\n");
            ret = 0;
        } else {
            long i;
            long row, col;
            fmpz_t val;

            fmpz_init(val);
            fmpz_mat_init(M, nrows, ncols);
            fmpz_mat_zero(M);

            for (i = 0; i < nnz; i++) {
                /* TODO: Should handle errors here. */
                if (fscanf(fp, "%ld %ld", &row, &col) != 2) {
                    fprintf(stderr, "Error reading (row, col) from file.\n");
                    ret = 0;
                    break;
                };
                /* MM format uses 1-based indexing. */
                if (fmpz_fread(fp, val) <= 0) {
                    fprintf(stderr, "Error reading value from file.\n");
                    ret = 0;
                    break;
                } else {
                    /* Sum up duplicates! */
                    fmpz_add(fmpz_mat_entry(M, row-1, col-1), \
                            fmpz_mat_entry(M, row-1, col-1), val);
                };
            };

            fmpz_clear(val);
        };
    };

    return ret;
};


/** Open file fname and read MatrixMarket data as fmpz_mat_t M.
 */
int fmpz_mat_mm_readfile(const char *fname, fmpz_mat_t M) {
    int ret = 1;
    FILE *fp;

    if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "Failed to open file: '%s'\n", fname);
        ret = 0;
    } else {
        if (!fmpz_mat_mm_read(fp, M))
            ret = 0;
        fclose(fp);
    };

    return ret;
};


/** Write fmpz_mat_t M as MatrixMarket data to (open) FILE *fp.
 */
int fmpz_mat_mm_write(FILE *fp, const fmpz_mat_t M) {
    int ret = 1;
    MM_typecode matcode;

    mm_initialize_typecode(&matcode);
    mm_set_matrix(&matcode);
    mm_set_coordinate(&matcode);
    mm_set_integer(&matcode);
    mm_set_general(&matcode);

    if (mm_write_banner(fp, matcode) != 0) {
        fprintf(stderr, "Cannot write to file.\n");
        ret = 0;
    } else {
        long i, j;
        long nnz = 0;

        for (i = 0; i < fmpz_mat_nrows(M); i++)
            for (j = 0; j < fmpz_mat_ncols(M); j++)
                if (!fmpz_is_zero(fmpz_mat_entry(M, i, j)))
                    nnz++;

        mm_write_mtx_crd_size(fp, fmpz_mat_nrows(M), fmpz_mat_ncols(M), nnz);

        for (i = 0; i < fmpz_mat_nrows(M); i++) {
            for (j = 0; j < fmpz_mat_ncols(M); j++) {
                if (!fmpz_is_zero(fmpz_mat_entry(M, i, j))) {
                    /* MM format uses 1-based indexing. */
                    fprintf(fp, "%ld %ld ", i+1, j+1);
                    fmpz_fprint(fp, fmpz_mat_entry(M, i, j));
                    fprintf(fp, "\n");
                };
            };
        };
    };

    return ret;
};


/** Open file fname and write fmpz_mat_t M as MatrixMarket data.
 */
int fmpz_mat_mm_writefile(const char *fname, const fmpz_mat_t M) {
    int ret = 1;
    FILE *fp;

    if ((fp = fopen(fname, "w")) == NULL) {
        fprintf(stderr, "Failed to open file: '%s'.\n", fname);
        ret = 0;
    } else {
        if (!fmpz_mat_mm_write(fp, M))
            ret = 0;
        fclose(fp);
    };

    return ret;
};


/* vim: set tw=78 ts=4 sw=4 et ai: */
