/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 * instructions.c
 *
 * Purpose: The purpose of this file is to contain the implementation for the 
 * 	    um instructions abstraction. This contains the 14 opcodes and the 
 *	    registers for the program 
 */

#include "instructions.h"

Register *registers;
const int NUM_REGISTERS = 8; 
int program_count = 0; 

/* 
*  name: initialize_registers 
*  parameters: none
*  does: this function initializes a sequence with the given number of 
*        registers and a program counter 
*  returns: void 
*/
void initialize_registers() {
	/* add one to include program counter */
	registers = (Register *)malloc(sizeof(Register)* (NUM_REGISTERS));
	for (int i = 0; i < NUM_REGISTERS; i++) {
		registers[i] = 0;  
	}  
}

/* 
*  name: free_registers 
*  parameters: none 
*  does: this function frees all memory associated with the registers 
*        registers 
*  returns: void 
*/
void free_registers() {
	free(registers); 
}

/* 
* name: conditional_move 
* parameters: Register ra, Register rb, Register rc
* does: if $r[C] does not eqaul zero, then $r[A] := $r[B]
* returns: int 
*/
int conditional_move(Register ra, Register rb, Register rc)
{
	if (registers[rc] != 0)
		registers[ra] = registers[rb];

	program_count++; 
	return program_count; 
}

/* 
* name: segmented_load 
* parameters: Register ra, Register rb, Register rc
* does: $r[A] := $m[$r[B]][$r[C]] 
* returns: int 
*/
int segmented_load(Register ra, Register rb, Register rc)
{
	registers[ra] = retrieve_word_from_memory(registers[rb], registers[rc]);
	program_count++; 
	return program_count; 
}

/* 
* name: segmented_store 
* parameters: Register ra, Register rb, Register rc 
* does: $m[$r[A]][$r[B]] := $r[C]
* returns: int 
*/
int segmented_store(Register ra, Register rb, Register rc)
{
	add_word_to_memory(registers[ra], registers[rb], registers[rc]); 
	program_count++; 
	return program_count; 
}

/* 
* name: addition 
* parameters: Register ra, Register rb, Register rc 
* does: $r[A] := ($r[B] + $[C]) mod 2^32 
* returns: int 
*/
int addition(Register ra, Register rb, Register rc)
{
	registers[ra] = (registers[rb] + registers[rc]) % ((uint64_t)1 << 32); 
	program_count++; 
	return program_count; 
}

/* 
* name: multiplication
* parameters: Register ra, Register rb, Register rc
* does: $r[A] := ($r[B] * $[C]) mod 2^32 
* returns: int 
*/
int multiplication(Register ra, Register rb, Register rc)
{
	registers[ra] = (registers[rb] * registers[rc]) % ((uint64_t)1 << 32); 
	program_count++; 
	return program_count; 
}

/* 
* name: division 
* parameters: Register ra, Register rb, Register rc
* does: $r[A] := ($r[B] / $[C]) 
* returns: int 
*/
int division(Register ra, Register rb, Register rc)
{
	assert(registers[rc] != 0);
	registers[ra] = registers[rb] / registers[rc]; 
	program_count++; 
	return program_count; 
}

/* 
* name: bitwise_NAND 
* parameters: Register ra, Register rb, Register rc
* does: $r[A] := !($r[B] and $r[C])
* returns: int 
*/
int bitwise_NAND(Register ra, Register rb, Register rc)
{
	registers[ra] = ~(registers[rb] & registers[rc]);
	program_count++; 
	return program_count;   
}

/* 
* name: halt
* parameters: none
* does: stops computation of a program 
* returns: int 
*/
int halt()
{
	free_segments();
	free_registers(); 
	exit(EXIT_SUCCESS);
}

/* 
* name: map_segment 
* parameters: Register rb, Register rc
* does: new segment is created with a number of words equal to value in $r[C]. 
*       each word in the segment is initialized to zero. the new segment index 
*       is placed in $r[B]. the new segment is mapped as $m[$r[B]]
* returns: int 
*/
int map_segment(Register rb, Register rc)
{
	registers[rb] = allocate_segment(registers[rc]); 
	program_count++; 
	return program_count;  
}

/* 
* name: unmap_segment 
* parameters: Register rc
* does: the segment $m[$r[C]] is unmapped. future map segment instructions can
*       reuse the identifier in $r[C] 
* returns: int 
*/
int unmap_segment(Register rc)
{
	free_segment(registers[rc]); 
	program_count++; 
	return program_count;  
}

/* 
* name: output
* parameters: Register rc 
* does: the value in $r[C] is written to the I/O device immediately. only 
*       values from 0 to 255 are allowed. 
* returns: int 
*/
int output(Register rc)
{
	if (registers[rc] > 255)
		exit(EXIT_FAILURE);

	fprintf(stdout, "%c", registers[rc]); 
	program_count++; 
	return program_count; 
}

/* 
* name: input
* parameters: Register rc
* does: the universal machine waits for input on the I/O device. when the input 
*       arrives, $r[C] is loaded with the input, which must be a value from 0  
*       to 255. if the end of input has been signaled, then $r[C] is laoded  
*       with a full 32 bit-word in which every bit is 1
* returns: int 
*/
int input(Register rc)
{
	registers[rc] = (Word) fgetc(stdin);

	if ((char)registers[rc] == EOF) {
		program_count++; 
		return program_count;
	}

	if (registers[rc] > 255) 
		exit(EXIT_FAILURE); 

	program_count++; 
	return program_count;  
}

/* 
* name: load_program 
* parameters: Register rb, Register rc
* does: segment $m[$r[B]] is duplicated, and the duplicate replaces $m[0], 
*       which is abandonded. the program counter is set to point to  
*       $m[0][$r[C]]. if $r[B] = 0, the load program operation is expected to
*       be very quick
* returns: int 
*/
int load_program(Register rb, Register rc) 
{
	if (registers[rb] != 0)
		duplicate_to_segment_zero(registers[rb]); 

	program_count = registers[rc]; 
	return program_count; 
}

/* 
* name: load_value 
* parameters: Word word
* does: the three bits immediately less significant than the opcode describe a 
*       single register A. The remaining 25 bits are an unsigned binary value. 
*       the instruction sets $r[A] to that value.
* returns: int 
*/
int load_value(Word word) 
{
	Register ra = (Register) Bitpack_getu(word, 3, 25);
	Word value = (Word) Bitpack_getu(word, 25, 0);

	registers[ra] = value; 

	program_count++; 
	return program_count;  
}

