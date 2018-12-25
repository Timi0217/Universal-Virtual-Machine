#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void emit_halt_test(Seq_T instructions);
extern void emit_verbose_halt_test(Seq_T instructions);
extern void emit_add_test(Seq_T stream);
extern void emit_print_six_test(Seq_T stream); 
extern void emit_bitwise_NAND(Seq_T stream);
extern void emit_load_program_test(Seq_T stream);
extern void emit_load_seg_test(Seq_T stream);
extern void emit_store_seg_test(Seq_T stream);
extern void emit_input_test(Seq_T stream);
extern void emit_mult_test(Seq_T stream);
extern void emit_div_test(Seq_T stream);
extern void emit_unmap_test(Seq_T stream);
extern void emit_map_seg_test(Seq_T stream);
extern void emit_store_seg_large_test(Seq_T stream);
//extern void emit_add_test_overload(Seq_T stream);

/* begin of unit tests for hw */
extern void CM(Seq_T stream); 


/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*emit_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "", emit_halt_test },
        { "halt-verbose", NULL, "", emit_verbose_halt_test },
        { "emit_add_test", NULL, "", emit_add_test },
        { "emit_print_six_test", NULL, "", emit_print_six_test}, 
        { "conditional_move", NULL, "", CM},
        { "emit_bitwise_NAND", NULL, "", emit_bitwise_NAND},
        { "emit_load_program_test", NULL, "", emit_load_program_test},
        { "emit_load_seg_test", NULL, "", emit_load_seg_test},
        { "emit_store_seg_test", NULL, "", emit_store_seg_test},
        { "emit_input_test", NULL, "", emit_input_test},
        { "emit_mult_test", NULL, "", emit_mult_test},
        { "emit_div_test", NULL, "", emit_div_test},
        { "emit_unmap_test", NULL, "", emit_unmap_test},
        { "emit_map_seg_test", NULL, "", emit_map_seg_test},
        { "emit_store_seg_large_test", NULL, "", emit_store_seg_large_test}
};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->emit_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}


