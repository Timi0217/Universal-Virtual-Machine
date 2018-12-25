/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 * UniveralMachine.h
 *
 * Purpose: The purpose of this file is to act as the glue between the segments
 *          and instructions abstractions. It will contain functions in order 
 *          to create and initalize the unviversal machine. It will also have 
 *          functions to beginning running the machine and processing command 
 *          line instructions. 
 */

#ifndef UNIVERSALMACHINE_H_
#define UNIVERSALMACHINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <seq.h>
#include <assert.h> 
#include "segments.h" 
#include "instructions.h"
#include "definitions.h"
#include "bitpack.h"

/* initializes registers, creates segments, and populates zero segment with 
   instructions */  
void create_machine(FILE* pfile, long int words_in_file);

/* begins running the program and processing instructions */
void run(FILE* pfile, long int file_size);

/* loops through each instruction in segment zero and executes them */  
void process_instructions(long int words_in_file); 

/* executes a single instruction */
int process_instruction(int offset); 

#endif