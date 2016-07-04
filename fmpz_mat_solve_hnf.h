/**
 * fmpz_mat_solve_hnf.h - Solve system of diophantine equations via HNF.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#ifndef __FMPZ_MAT_SOLVE_HNF__H
#define __FMPZ_MAT_SOLVE_HNF__H

#include <flint/fmpz_mat.h>


int fmpz_mat_solve_hnf(fmpz_mat_t x, const fmpz_mat_t A, const fmpz_mat_t b);


#endif

/* vim: set tw=78 ts=4 sw=4 et ai: */
