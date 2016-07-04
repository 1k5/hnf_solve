/**
 * solve.c - Verify solution to system of linear diophantine equations.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>

#include "fmpz_mat_mmio.h"


const char *argp_program_version = "verify 2016-06-15";
const char *argp_program_bug_address = \
        "Malte Dehling <malte.dehling@uni-goettingen.de>";

static char doc[] = \
        "Verify solution to system of linear diophantine equations." \
        "\vAll files should be in MatrixMarket v2.0 'matrix coordinate " \
        "integer general' format.";

static char args_doc[] = "MATRIX-A MATRIX-X MATRIX-B";

struct args {
    char *fname[3];
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct args *args = state->input;

    switch(key) {
    case ARGP_KEY_ARG:
        if (state->arg_num >= 3)
            argp_usage(state);  /* Too many arguments. */
        args->fname[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 3)
            argp_usage(state);  /* Too few arguments. */
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    };

    return 0;
};

static struct argp argp = {NULL, parse_opt, args_doc, doc};


int main(int argc, char **argv) {
    int ret = EXIT_SUCCESS;
    struct args args;
    fmpz_mat_t A, X, B;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    if (!fmpz_mat_mm_readfile(args.fname[0], A)) {
        ret = EXIT_FAILURE;
    } else if (!fmpz_mat_mm_readfile(args.fname[1], X)) {
        fmpz_mat_clear(A);
        ret = EXIT_FAILURE;
    } else if (!fmpz_mat_mm_readfile(args.fname[2], B)) {
        fmpz_mat_clear(A);
        fmpz_mat_clear(X);
        ret = EXIT_FAILURE;
    } else {
        fmpz_mat_t AX;

        fmpz_mat_init(AX, fmpz_mat_nrows(A), fmpz_mat_ncols(X));
        fmpz_mat_mul(AX, A, X);

        if (!fmpz_mat_equal(AX, B)) {
            printf("AX - B is *not* zero!\n");
            ret = EXIT_FAILURE;
        } else {
            printf("AX = B.\n");
        };

        fmpz_mat_clear(A);
        fmpz_mat_clear(X);
        fmpz_mat_clear(B);
        fmpz_mat_clear(AX);
    };

    exit(ret);
};


/* vim: set tw=78 ts=4 sw=4 et ai: */
