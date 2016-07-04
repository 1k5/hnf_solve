/**
 * fmpz_mat_solve_hnf.c - Solve system of diophantine equations via HNF.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#include <stdio.h>

#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>

#include "fmpz_mat_hnf_col_transform.h"


/** Solve diophantine matrix equation Ax = b for x.
 *
 * TODO: Implement solving for multiple columns in b simultaneously.  Also
 * make behavior more similar to FLINT's own fmpz_mat_solve_*() functions.
 */
int fmpz_mat_solve_hnf(fmpz_mat_t x, const fmpz_mat_t A, const fmpz_mat_t b) {
    int ret = 1;
    fmpz_mat_t H, U, y, b_;
    fmpz_t r;
    long row = 0, col, i;

    /* Compute the column HNF for A such that H = AU. */
    printf("Computing HNF... "); fflush(stdout);
    fmpz_mat_init(H, fmpz_mat_nrows(A), fmpz_mat_ncols(A));
    fmpz_mat_init(U, fmpz_mat_ncols(A), fmpz_mat_ncols(A));
    fmpz_mat_hnf_col_transform(H, U, A);
    printf("done.\n");

    /* Solve Hy = b_ by backward substitution.  Then x = Uy solves Ax = b_. */
    fmpz_mat_init_set(b_, b);
    fmpz_mat_init(y, fmpz_mat_ncols(H), 1);
    fmpz_init(r);
    for (col = 0; col < fmpz_mat_ncols(H); col++) {
        for (; row < fmpz_mat_nrows(H) \
                && fmpz_is_zero(fmpz_mat_entry(H, row, col)); row++) {
            /* Row extends by zero to the right; check that b_[row] = 0. */
            if (!fmpz_is_zero(fmpz_mat_entry(b_, row, 0))) {
                printf("b_[%d] = 0 expected, found: ", row);
                fmpz_print(fmpz_mat_entry(b_, row, 0));
                printf(".\n");
                ret = 0;
                goto cleanup;
            };
        };

        if (row < fmpz_mat_nrows(H)) {
            /* Now have a nonzero entry. */
            fmpz_tdiv_qr(fmpz_mat_entry(y, col, 0), r, \
                    fmpz_mat_entry(b_, row, 0), fmpz_mat_entry(H, row, col));
            if (!fmpz_is_zero(r)) {
                printf("Found non-zero remainder ");
                fmpz_print(r);
                printf(" in row %d.\n", row);
                ret = 0;
                goto cleanup;
            };
            for (i = row; i < fmpz_mat_nrows(H); i++) {
                fmpz_submul(fmpz_mat_entry(b_, i, 0), \
                        fmpz_mat_entry(H, i, col), fmpz_mat_entry(y, col, 0));
            };
        };
    };

    fmpz_mat_mul(x, U, y);

cleanup:
    fmpz_clear(r);
    fmpz_mat_clear(y);
    fmpz_mat_clear(b_);
    fmpz_mat_clear(H);
    fmpz_mat_clear(U);

    return ret;
};


/* vim: set tw=78 ts=4 sw=4 et ai: */
