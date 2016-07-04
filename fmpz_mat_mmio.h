/**
 * fmpz_mat_mmio.h - MatrixMarket IO for FLINT.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#ifndef __FMPZ_MAT_MMIO__H
#define __FMPZ_MAT_MMIO__H

#include <flint/fmpz_mat.h>


int fmpz_mat_mm_read(FILE *fp, fmpz_mat_t M);
int fmpz_mat_mm_readfile(const char *fname, fmpz_mat_t M);

int fmpz_mat_mm_write(FILE *fp, const fmpz_mat_t M);
int fmpz_mat_mm_writefile(const char *fname, const fmpz_mat_t M);


#endif

/* vim: set tw=78 ts=4 sw=4 et ai: */
