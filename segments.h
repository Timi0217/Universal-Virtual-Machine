/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 * segments.h
 *
 * Purpose: The purpose of this file is to contain the interface for the 
 * 	    segent abstraction. This allows for words to be pushed to and 
 * 	    from specific segments. 
 */

#ifndef SEGMENTS_H_
#define SEGMENTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <seq.h>
#include <assert.h> 
#include "definitions.h"
#include "bitpack.h"

typedef uint32_t word;

extern void print_segments(); 
/* creates mapped and unmapped sequences and initializes the zero segment
   to all zeros of the given length of segment size */
extern void initialize_segments(FILE* pfile, long int seg_zero_size);

/* creates a segment in the mapped segments sequence */
extern int allocate_segment(int word_count);

/* frees a the segment with the given index from the mapped sequence */
extern void free_segment(int identifier);

/* adds a given word to the segment with the given segment id at the 
   the offset specified */
extern void add_word_to_memory(int seg_id, int offset, word value);

/* creates mapped and unmapped sequences and initializes the zero segment
   to all zeros of the given length of segment size */ 
extern word retrieve_word_from_memory(int seg_id, int offset);

/* frees all memory associated with the mapped and unmapped segments */
extern void free_segments();

/* duplicates a segment and loads it into segment one */
extern void duplicate_to_segment_zero(int index); 

#endif