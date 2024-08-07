/* arith.c */
void add(ADDR n,int lflag);
void subtract(ADDR n,int lflag);
void mult_add(ADDR n,int lflag);
void mult_sub(ADDR n,int lflag);
void collate(ADDR n,int lflag);
void rshift(ADDR n,int lflag);
void lshift(ADDR n,int lflag);
void roundoff(ADDR n,int lflag);
/* control.c */
void halt(ADDR n,int lflag);
void no_operation(ADDR n,int lflag);
void pos_branch(ADDR n,int lflag);
void neg_branch(ADDR n,int lflag);
/* edsac.c */
/* error.c */
void error(char* msg);
int check_addr(ADDR n,int lflag);
/* exec_ord.c */
void exec_order(void);
void undefined(ADDR n, int lflag);
/* execute.c */
void execute(void);
/* fetch.c */
void fetch_order(void);
/* init.c */
WORD read_18_bits(void);
void read_binary_to_store(void);
void initialize(void);
void process_options(char *options);
/* inout.c */
void input(ADDR n,int lflag);
void output(ADDR n,int lflag);
void print_check(ADDR n,int lflag);
/* main.c */
int main(int argc,char *argv[]);
/* memory.c */
void u_transfer(ADDR n,int lflag);
void transfer(ADDR n,int lflag);
void load_multiplier(ADDR n,int lflag);
/* mult.c */
void do_mult(ADDR n,int lflag);
void add_into_acc(void);
void sub_from_acc(void);
/* printbits.c */
void printbits_n(WORD x, int n);
void printbits_17(WORD x);
void printbits_18(WORD x);

/* shift.c */
void shift_right(CONTROL_WORD control);
void shift_left(CONTROL_WORD control);
