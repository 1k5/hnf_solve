/**
 * fmpz_mat_hnf_col_transform.c - Column HNF with tranformation.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>


/** Compute the column HNF with transformation matrix H = AU.
 *
 * TODO: Implement directly as in fmpz_mat_hnf_transform().
 */
void fmpz_mat_hnf_col_transform(fmpz_mat_t H, fmpz_mat_t U, \
        const fmpz_mat_t A)
{
    fmpz_mat_t tA, H_, U_;

    /* Compute row HNF for tA: H_ = U_*tA. */
    fmpz_mat_init(tA, fmpz_mat_ncols(A), fmpz_mat_nrows(A));
    fmpz_mat_transpose(tA, A);
    fmpz_mat_init(H_, fmpz_mat_nrows(tA), fmpz_mat_ncols(tA));
    fmpz_mat_init(U_, fmpz_mat_nrows(tA), fmpz_mat_nrows(tA));

    /* Argh!  Pernet-Stein algorithm can fail (with low probability.)  Why
     * doesn't fmpz_mat_hnf_transform() check this?!  Alex has this fixed, so
     * soon this should no longer be needed. */
    do {
        fmpz_mat_hnf_transform(H_, U_, tA);
    } while (!fmpz_mat_is_in_hnf(H_));

    fmpz_mat_clear(tA);

    /* Transpose H_, U_: H = A*U is column HNF. */
    fmpz_mat_transpose(H, H_);
    fmpz_mat_transpose(U, U_);
    fmpz_mat_clear(H_);
    fmpz_mat_clear(U_);
};


/* vim: set tw=78 ts=4 sw=4 et ai: */
