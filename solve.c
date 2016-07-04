/**
 * solve.c - Solve system of linear diophantine equations via HNF.
 *
 * Copyright (C) 2016 by Malte Dehling <malte.dehling@uni-goettingen.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>

#include "fmpz_mat_mmio.h"
#include "fmpz_mat_solve_hnf.h"


const char *argp_program_version = "solve 2016-05-31";
const char *argp_program_bug_address = \
        "Malte Dehling <malte.dehling@uni-goettingen.de>";

static char doc[] = \
        "Solve system of linear diophantine equations via HNF." \
        "\vAll files should be in MatrixMarket v2.0 'matrix coordinate " \
        "integer general' format.";

static char args_doc[] = "MATRIX-A VECTOR-b";

static struct argp_option options[] = {
    {NULL,  'x',    "FILE", 0, \
        "Write solution vector x to FILE instead of stdout"},
    NULL
};

struct args {
    char *fname[2];
    char *fname_x;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct args *args = state->input;

    switch(key) {
    case 'x':
        args->fname_x = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
            argp_usage(state);  /* Too many arguments. */
        args->fname[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 2)
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
    struct args args;
    fmpz_mat_t A, b;

    args.fname_x = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    if (!fmpz_mat_mm_readfile(args.fname[0], A)) {
        ret = EXIT_FAILURE;
    } else if (!fmpz_mat_mm_readfile(args.fname[1], b)) {
        fmpz_mat_clear(A);
        ret = EXIT_FAILURE;
    } else {
        fmpz_mat_t x;

        fmpz_mat_init(x, fmpz_mat_ncols(A), 1);

        if (!fmpz_mat_solve_hnf(x, A, b)) {
            printf("fmpz_mat_solve_hnf() failed!\n");
            ret = EXIT_FAILURE;
        } else if (args.fname_x != NULL) {
            if (!fmpz_mat_mm_writefile(args.fname_x, x)) {
                ret = EXIT_FAILURE;
            };
        } else {
            if (!fmpz_mat_mm_write(stdout, x)) {
                ret = EXIT_FAILURE;
            };
        };

        fmpz_mat_clear(x);

        fmpz_mat_clear(A);
        fmpz_mat_clear(b);
    };

    exit(ret);
};


/* vim: set tw=78 ts=4 sw=4 et ai: */
