HNF_OBJS = hnf.o mmio.o fmpz_mat_mmio.o fmpz_mat_hnf_col_transform.o
SOLVE_OBJS = solve.o mmio.o fmpz_mat_mmio.o fmpz_mat_hnf_col_transform.o
SOLVE_OBJS += fmpz_mat_solve_hnf.o
VERIFY_OBJS = verify.o mmio.o fmpz_mat_mmio.o


CFLAGS = -march=native -mtune=native -O2
LDFLAGS = -lflint -lgmp


all: hnf solve verify

hnf: $(HNF_OBJS)

solve: $(SOLVE_OBJS)

verify: $(VERIFY_OBJS)

clean:
	rm -f hnf solve verify $(HNF_OBJS) $(SOLVE_OBJS) $(VERIFY_OBJS)

