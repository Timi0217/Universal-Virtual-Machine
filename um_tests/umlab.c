/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>

#include <assert.h>
#include <seq.h>
#include "bitpack.h"

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;


/* Wrapper functions for each of the instructions */

static inline Um_instruction conditional_move(Um_register a, Um_register b, Um_register c)
{
        return three_register(CMOV, a , b, c);
}

static inline Um_instruction segmented_load(Um_register a, Um_register b, Um_register c)
{
        return three_register(SLOAD, a , b, c);
}

static inline Um_instruction segmented_store(Um_register a, Um_register b, Um_register c)
{
        return three_register(SSTORE, a , b, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b, Um_register c)
{
        return three_register(MUL, a , b, c);
}

static inline Um_instruction divide(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction map_segment(Um_register b, Um_register c) 
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_segment(Um_register c) 
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0 , 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0 , 0, c);
}

static inline Um_instruction load_program(Um_register b, Um_register c)
{
        return three_register(LOADP, 0 , b, c);
}

/* Functions for working with streams */

static inline Um_instruction get_inst(Seq_T stream, int i)
{
    assert(sizeof(Um_instruction) <= sizeof(uintptr_t));
    return (Um_instruction)(uintptr_t)(Seq_get(stream, i));
}

static inline void put_inst(Seq_T stream, int i, Um_instruction inst)
{
    assert(sizeof(inst) <= sizeof(uintptr_t));
    Seq_put(stream, i, (void *)(uintptr_t) inst);
}

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

extern void Um_write_sequence(FILE *output, Seq_T stream)
{ 
        int length = Seq_length(stream);
        assert(length > 0);
        for (int i = 0; i < length; i++) { 
                uintptr_t word = (uintptr_t)Seq_remlo(stream);
                for(int lsb = 24; lsb >= 0; lsb -= 8) {
                        putc( (int)Bitpack_getu(word, 8, lsb), output );
                }
        }
}

/* functions that return the two different instruction types  */
Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction instruction = Bitpack_newu(0, 3, 0, rc);
        instruction = Bitpack_newu(instruction, 3, 3, rb);
        instruction = Bitpack_newu(instruction, 3, 6, ra);
        instruction = Bitpack_newu(instruction, 4, 28, op);

        return instruction;   
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        /* word, width, lsb, value */ 
        Um_instruction instruction = Bitpack_newu(0, 25, 0, val);
        instruction = Bitpack_newu(instruction, 3, 25, ra);
        instruction = Bitpack_newu(instruction, 4, 28, 13);

        return instruction; 
}


/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        /* immediately halts */
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        /* immediately halts and does not print Bad! */
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}

  /* should print 'd' */
extern void emit_add_test(Seq_T stream) 
{
        /* add does $r[A] := ($r[B] + $[C]) */
        emit(stream, loadval(r1, 2));
        emit(stream, loadval(r2, 'a'));
        emit(stream, loadval(r3, 3));

        emit(stream, add(r1, r2, r3));
        emit(stream, output(r1));

        emit(stream, halt());
}


/* prints the number 54 */
extern void emit_print_six_test(Seq_T stream)
{
        /* should print 54 */
        emit(stream, loadval(r1, 48));
        emit(stream, loadval(r2, 6));
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}   

/* 1. conditional move test: rc = 0 and rc != 0 tested */ 
/* should output B, D, E */
extern void CM(Seq_T stream)
{
        /* if $r[C] does not eqaul zero, then $r[A] := $r[B] */
        emit(stream, loadval(r0, 0));
        emit(stream, loadval(r1, 1));
        emit(stream, loadval(r2, 2));
        emit(stream, loadval(r3, 3));
        emit(stream, loadval(r4, 4));
        emit(stream, loadval(r5, 5));
        emit(stream, loadval(r6, 6));
        emit(stream, loadval(r7, 7));

        /* since r3 does not equal 0 r1 gets r2 */
        emit(stream, conditional_move(r1, r2, r3)); 

        /* since r3 equals 0, r4 should remain unchanged */
        emit(stream, conditional_move(r4, r5, r0)); 

        /* should output 2, 4, 5 */
        emit(stream, output(r1));
        emit(stream, output(r4));
        emit(stream, output(r5));
        emit(stream, halt());
}


extern void emit_bitwise_NAND(Seq_T stream){
        /*
        emit(stream, loadval(r0, 0));
        Um_instruction val = ~0U;
        emit(stream, loadval(r1, val));

        emit(stream, loadval(r2, val));
        emit(stream, nand(r0, r1, r2));
        emit(stream, output(r0));
        emit(stream, halt());
        */

        emit(stream, loadval(r0, 0x1));
        emit(stream, loadval(r1, 0x1));
        emit(stream, loadval(r2, 0));
        emit(stream, nand(r2, r1, r0));
        fprintf(stderr, "%d\n", r2);
        emit(stream, output(r2));
        emit(stream, halt());

}

void emit_load_program_test(Seq_T stream){
    //mult test
    emit(stream, loadval(r0, 5));
    emit(stream, loadval(r1, 10));
    emit(stream, multiply(r2, r1, r0));
    emit(stream, output(r2));
    emit(stream, output(r1));
    emit(stream, output(r0));

    //begin loading
    emit(stream, loadval(r0, 60));
    emit(stream, map_segment(r2, r0));
    for (int i = 0; i < 6; i++){
        emit(stream, loadval(r7, i));
        emit(stream, loadval(r1, 0));
        emit(stream, segmented_load(r4, r1, r7));
        emit(stream, segmented_store(r2, r7, r4));
    }
    
    emit(stream, loadval(r6, 0));
    emit(stream, load_program(r2, r6));
    emit(stream, halt());
}

void emit_load_seg_test(Seq_T stream){
    emit(stream, loadval(r0, 10));
    emit(stream, loadval(r1, 1));
    emit(stream, map_segment(r2, r0)); //Creates segment with 10 words
    emit(stream, segmented_store(r2, r1, r0)); //Stores 10 in word 1 at the segment
    emit(stream, segmented_load(r3, r2, r1));
    emit(stream, output(r3));
    emit(stream, halt());
}

void emit_store_seg_test(Seq_T stream){
    emit(stream, loadval(r0, 10));
    emit(stream, loadval(r1, 1));
    emit(stream, map_segment(r2, r0)); //Creates segment with 10 words
    emit(stream, segmented_store(r2, r1, r0)); //Stores 10 in word 1 at the segment
    emit(stream, halt());
}


void emit_input_test(Seq_T stream){
    emit(stream, input(r0));
    emit(stream, output(r0));
    emit(stream, halt());
}

//desired output == '2'
void emit_mult_test(Seq_T stream){
    emit(stream, loadval(r0, 5));
    emit(stream, loadval(r1, 10));
    emit(stream, multiply(r2, r1, r0));
    emit(stream, output(r2));
    emit(stream, halt());
}

//desired output == 'E'
void emit_div_test(Seq_T stream){
    emit(stream, loadval(r0, 10));
    emit(stream, loadval(r1, 50));
    emit(stream, divide(r2, r1, r0));
    emit(stream, output(r2));
    emit(stream, halt());
}

void emit_unmap_test(Seq_T stream){
  emit(stream, loadval(r0, 1));
  emit(stream, map_segment(r1, r0));
  emit(stream, map_segment(r2, r0));
  emit(stream, map_segment(r3, r0));
  emit(stream, unmap_segment(r1));
  emit(stream, unmap_segment(r2));
  emit(stream, unmap_segment(r3));
  emit(stream, halt());
}

void emit_map_seg_test(Seq_T stream){
    emit(stream, loadval(r0, 10));
    emit(stream, map_segment(r3, r0));
    emit(stream, map_segment(r2, r0));
    emit(stream, map_segment(r1, r0));
    emit(stream, output(r1));
    emit(stream, output(r2));
    emit(stream, output(r3));
    emit(stream, halt());
}

//gives a bunch of ascii in alphabetical order
void emit_store_seg_large_test(Seq_T stream){
    emit(stream, loadval(r0, 254));
    emit(stream, loadval(r1, 1));
    emit(stream, map_segment(r2, r0)); //Creates segment with 10002 words
    emit(stream, loadval(r0, 1));
    emit(stream, segmented_store(r2, r1, r0)); //Stores 10 in word 1 at the segment
    for (int i = 0; i < 254; i++){
        emit(stream, loadval(r0, i)); 
        emit(stream, segmented_store(r2, r0, r0));
        emit(stream, segmented_load(r3, r2, r0));
        emit(stream, output(r3));
    }
    emit(stream, halt());
}





















