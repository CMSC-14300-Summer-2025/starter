#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_ARGS 128

size_t dec_length(int number);
void wc(FILE *file, int *lcount, int *wcount, int *bcount);
void print_results(char program_name[], int file_count, char *file_name[], int err_no[], int lcount[], int wcount[], int bcount[]);

int main(int argc, char *argv[]) {
    int lcount[MAX_ARGS];
    int wcount[MAX_ARGS];
    int bcount[MAX_ARGS];
    int err_no[MAX_ARGS] = { 0 };

    int return_code = EXIT_SUCCESS;

    FILE *file;

    /* You code here */


    return return_code;
}
