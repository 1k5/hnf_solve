/**
 * fmpz_mat_hnf_col_transform.h - MatrixMarket IO for FLINT.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#ifndef __FMPZ_MAT_HNF_COL_TRANSFORM__H
#define __FMPZ_MAT_HNF_COL_TRANSFORM__H

#include <flint/fmpz_mat.h>


void fmpz_mat_hnf_col_transform(fmpz_mat_t H, fmpz_mat_t U, \
        const fmpz_mat_t A);


#endif

/* vim: set tw=78 ts=4 sw=4 et ai: */
