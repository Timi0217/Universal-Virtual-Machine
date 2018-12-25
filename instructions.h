/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 * instructions.h
 *
 * Purpose: The purpose of this file is to contain the interface for the 
 * 	    um instructions abstraction. This contains the 14 opcodes and the 
 *	    registers for the program 
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <seq.h>
#include "segments.h" 
#include "definitions.h"
#include "bitpack.h" 


/* this function initializes a sequence with the given number of registers 
   and a program counter */
void initialize_registers(); 

/* this function frees all memory associated with the sequence of registers */
void free_registers(); 

/* if $r[C] does not eqaul zero, then $r[A] := $r[B] */
int conditional_move(Register ra, Register rb, Register rc);

/* $r[A] := $m[$r[B]][$r[C]] */
int segmented_load(Register ra, Register rb, Register rc);

/* $m[$r[A]][$r[C]] := $r[C] */
int segmented_store(Register ra, Register rb, Register rc);

/* $r[A] := ($r[B] + $[C]) mod 2^32  */
int addition(Register ra, Register rb, Register rc);

/* $r[A] := ($r[B] * $[C]) mod 2^32  */
int multiplication(Register ra, Register rb, Register rc);

/* $r[A] := ($r[B] / $[C]) */
int division(Register ra, Register rb, Register rc);

/* $r[A] := !($r[B] and $r[C]) */
int bitwise_NAND(Register ra, Register rb, Register rc);

/* stop computation */
int halt();

/* a new segment is created with a number of words equal to value in $r[C]. 
   each word in the segment is initialized to zero. the new segment 
   is placed in $r[B]. the new segment is mapped as $m[$r[B]] */
int map_segment(Register rb, Register rc);

/* the segment $m[$r[C]] is unmapped. future map segment instructions can
   reuse the identifier in $r[C] */
int unmap_segment(Register rc);

/* the value in $r[C] is written to the I/O device immediately. only values 
   from 0 to 255 are allowed */
int output(Register rc);

/* the universal machine waits for input on the I/O device. when the input 
   arrives, $r[C] is loaded with the input, which must be a value from 0 to 
   to 255. if the end of input has been signaled, then $r[C] is laoded with 
   a full 32 bit-word in which every bit is 1 */
int input(Register rc);

/* segment $m[$r[B]] is duplicated,a nd the duplicate replicates $m[0], which 
   is abandonded. the program counter is set to point to $m[0][$r[C]. 
   if $r[B] = 0, the load program operation is expected to be very quick */
int load_program(Register rb, Register rc);

/* the three bits immediately less significant than the opcode describe a 
   single register A. The remaining 25 bits are an unsigned binary value. 
   the instruction sets $r[A] to that value. */
int load_value(Word word);

#endif





