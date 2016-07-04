/**
 * hnf.c - Compute HNF of a multiple precision integer matrix.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>

#include "fmpz_mat_mmio.h"
#include "fmpz_mat_hnf_col_transform.h"


const char *argp_program_version = "hnf 2016-05-31";
const char *argp_program_bug_address = \
        "Malte Dehling <malte.dehling@uni-goettingen.de>";

static char doc[] = \
        "Compute HNF of a multiple precision integer matrix." \
        "\vAll files should be in MatrixMarket v2.0 'matrix coordinate " \
        "integer general' format.";

static char args_doc[] = "MATRIX-A";

static struct argp_option options[] = {
    {NULL,  'H',    "FILE", 0, \
        "Write HNF to FILE instead of stdout"},
    {NULL,  'U',    "FILE", 0, \
        "Write unimodular transformation to FILE"},
    NULL
};

struct args {
    char *fname_A;
    char *fname_H;
    char *fname_U;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct args *args = state->input;

    switch(key) {
    case 'H':
        args->fname_H = arg;
        break;
    case 'U':
        args->fname_U = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            argp_usage(state);  /* Too many arguments. */
        args->fname_A = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1)
            argp_usage(state);  /* Too few arguments. */
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    };

    return 0;
};

static struct argp argp = {options, parse_opt, args_doc, doc};


int main(int argc, char **argv) {
    int ret = EXIT_SUCCESS;
    struct args args = {NULL, NULL, NULL};
    fmpz_mat_t A;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    if (!fmpz_mat_mm_readfile(args.fname_A, A)) {
        ret = EXIT_FAILURE;
    } else {
        fmpz_mat_t H, U;

        /* Compute the column HNF for A such that H = AU. */
        printf("Computing HNF... "); fflush(stdout);
        fmpz_mat_init(H, fmpz_mat_nrows(A), fmpz_mat_ncols(A));
        fmpz_mat_init(U, fmpz_mat_ncols(A), fmpz_mat_ncols(A));
        fmpz_mat_hnf_col_transform(H, U, A);
        printf("done.\n");

        if (args.fname_H != NULL) {
            if (!fmpz_mat_mm_writefile(args.fname_H, H)) {
                ret = EXIT_FAILURE;
            };
        } else {
            if (!fmpz_mat_mm_write(stdout, H)) {
                ret = EXIT_FAILURE;
            };
        };

        if (args.fname_U != NULL) {
            if (!fmpz_mat_mm_writefile(args.fname_U, U)) {
                ret = EXIT_FAILURE;
            };
        };

        fmpz_mat_clear(H);
        fmpz_mat_clear(U);

        fmpz_mat_clear(A);
    };

    exit(ret);
};


/* vim: set tw=78 ts=4 sw=4 et ai: */
