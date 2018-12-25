/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 * UniveralMachine.c
 *
 * Purpose: The purpose of this file is to act as the glue between the segments
 *          and instructions abstractions. It will contain functions in order 
 *          to create and initalize the unviversal machine. It will also have 
 *          functions to beginning running the machine and processing command 
 *          line instructions. 
 */

#include "universal_machine.h"


/* 
* name: create_machine
* parameters: file stream, long int 
* does: initializes registers, creates segments, and populates zero segment 
*       with instructions  
* returns: void 
*/
void create_machine(FILE* pfile, long int words_in_file)
{	
	initialize_segments(pfile, words_in_file);
	initialize_registers();
}

/* 
* name: run
* parameters: file stream, long int 
* does: begins running the program and processing instructions 
* returns: void 
*/
void run(FILE* pfile, long int file_size)
{
	long int words_in_file = file_size / sizeof(word);
	create_machine(pfile, words_in_file); 
	process_instructions(words_in_file); 
	free_segments(); 
	free_registers(); 
}


/* 
* name: process_instructions 
* parameters: int with number of words in file 
* does: loops through each instruction in segment zero and executes them 
* returns: void 
*/
void process_instructions(long int words_in_file)
{
	/* loop through all instructions and execute them */ 
	for (int prog_count = 0; prog_count < words_in_file;) {
		prog_count = process_instruction(prog_count); 
	}
}

/* 
* name: process_instruction
* parameters: int with the offset in segment zero 
* does: executes a given instruction 
* returns: int 
*/
int process_instruction(int offset)
{	
	Word word = retrieve_word_from_memory(0, offset); 
	UM_instruction opcode = (UM_instruction) Bitpack_getu(word, 4, 
							sizeof(word) * 8 - 4);
	Register rc = Bitpack_getu(word, 3, 0);
	Register rb = Bitpack_getu(word, 3, 3);
	Register ra = Bitpack_getu(word, 3, 6);

	switch(opcode){
		case 0 :
			return conditional_move(ra, rb, rc); 
			break; 
		case 1 : 
			return segmented_load(ra, rb, rc); 
			break; 
		case 2 : 
			return segmented_store(ra, rb, rc); 
			break; 
		case 3 : 
			return addition(ra, rb, rc); 
			break; 
		case 4 : 
			return multiplication(ra, rb, rc); 
			break; 
		case 5 : 
			return division(ra, rb, rc); 
			break; 
		case 6 : 
			return bitwise_NAND(ra, rb, rc); 
			break; 
		case 7 : 
			return halt(); 
			break; 
		case 8 : 
			return map_segment(rb, rc); 
			break; 
		case 9 : 
			return unmap_segment(rc); 
			break; 
		case 10 : 
			return output(rc); 
			break; 
		case 11 : 
			return input(rc); 
			break; 
		case 12 : 
			return load_program(rb, rc); 
			break; 
		case 13 : 
			return load_value(word); 
			break; 
		default : 
			exit(EXIT_FAILURE); 
			break;  
	}
}






