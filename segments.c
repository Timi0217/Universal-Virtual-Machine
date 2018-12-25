/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 * segments.c
 *
 * Purpose: The purpose of this file is to contain the implementation for the 
 * 	    segent abstraction. This allows for words to be pushed to and 
 * 	    from specific segments. 
 */

#include "segments.h"

Seq_T mapped;
Seq_T unmapped;

void initialize_zero_segment(FILE* pfile, long int seg_zero_size); 


/* 
* name: initialize_segments
* parameters: pointer to file stream, int for the size of the zero segment 
* does: creates mapped and unmapped sequences and initializes the zero segment
*       to all zeros of the given length of segment size 
* returns: void     
*/
void initialize_segments(FILE* pfile, long int seg_zero_size) {
	/* create mapped and unmapped sequences */
	mapped = Seq_new(1);
	unmapped = Seq_new(1);

	initialize_zero_segment(pfile, seg_zero_size); 
}

/* 
* name: initialize_zero_segment 
* parameters: FILE* pfile, long int
* does: loads instructions from file stream into zero segment
* returns: void 
*/
void initialize_zero_segment(FILE* pfile, long int seg_zero_size) 
{	
	int word_size = sizeof(Word); 
	Seq_T segment = Seq_new(seg_zero_size);
	
	/* create word if given um file contains words */
	Word *word; 
	int byte = 0; 
	/* loop through each word */
	for (int i = 0; i < seg_zero_size; i++) {
		/* add each byte in file to word */
		word = malloc(word_size);
		*word = (Word) Bitpack_newu(0, 0, 0, 0);
		for (int j = 0; j < word_size; j++) {
			byte = fgetc(pfile);
			/* add to word byte by byte */
			*word = (Word) Bitpack_newu(*word, 8,
					 word_size * 8 - (8 * (j+1)), byte);
		}

		Seq_addhi(segment, word); 
	}

	Seq_addlo(mapped, segment);
	fclose(pfile);
}


/* 
* name: map_segment
* parameters: int for the number of words to be stored in a given segment 
* does: creates a segment in the mapped segments sequence 
* returns: int with index in mapped sequence where the segment was added    
*/
int allocate_segment(int word_count)
{
	Seq_T new_segment = Seq_new(word_count);

	Word *word; 
	/* malloc and initialize values in segment */
	for (int i = 0; i < word_count; i++) {
		word = malloc(sizeof(Word)); 
		*word = 0;
		Seq_addhi(new_segment, word); 
	}

	int unmapped_length = Seq_length(unmapped);
	int lowest_index = -1; 
	int *index;
	if (unmapped_length != 0) {
		index = Seq_remlo(unmapped); 
		lowest_index = *index; 
		free(index); 
	}


	/* if we are not reusing an index*/
	if (lowest_index == -1) { 
		Seq_addhi(mapped, new_segment);
		return Seq_length(mapped) - 1;
	}
	else {
		Seq_put(mapped, lowest_index, new_segment); 
	}
	
	return lowest_index;
}

/* 
* name: unmap_segment
* parameters: int with the identifier of the mapped segment 
* does: frees a the segment with the given index from the mapped sequence 
* returns: void     
*/
void free_segment(int identifier)
{
	assert(identifier < Seq_length(mapped) && identifier >= 0);

	/* return if segment is NULL */
	Seq_T segment = Seq_get(mapped, identifier);
	if (segment == NULL)
		return;  

	/* free each individual element in sequence */
	int segment_length = Seq_length(segment); 
	for (int i = 0; i < segment_length; i++) {
		Word* word = Seq_get(segment, i);
		if (word != NULL)
			free(word); 
	}

	/* add segment id to unmapped, then free segment if not seg 0 */
	if (identifier != 0) {
		int *unmapped_index = malloc(sizeof(int)); 
		*unmapped_index = identifier;
		Seq_addhi(unmapped, unmapped_index);  
	}

	Seq_free(&segment); 
	Seq_put(mapped, identifier, NULL);
}

/* 
* name: add_word_to_memory 
* parameters: int for segment id, int for offset in segment, word for value 
* does: adds a given word to the segment with the given segment id at the 
*       the offset specified
* returns: void     
*/
void add_word_to_memory(int seg_id, int offset, Word value)
{
	assert(seg_id < Seq_length(mapped) && seg_id >= 0);
	Seq_T segment = Seq_get(mapped, seg_id); 

	assert(offset < Seq_length(segment) && offset >= 0);
	Word* word = Seq_get(segment, offset); 
	
	*word = value; 
}

/* 
* name: retrieve_word_from_memory 
* parameters: int for the segment id, in for the offset in memory 
* does: creates mapped and unmapped sequences and initializes the zero segment
*       to all zeros of the given length of segment size 
* returns: word that was at given segment id with given offset     
*/
word retrieve_word_from_memory(int seg_id, int offset)
{
	assert(seg_id < Seq_length(mapped) && seg_id >= 0);
	Seq_T segment = Seq_get(mapped, seg_id); 
	assert(segment != NULL); 

	assert(offset < Seq_length(segment) && offset >= 0);
	Word* word = Seq_get(segment, offset); 
	assert(word != NULL);

	return *word;
}

/* 
* name: free_segments
* parameters: none 
* does: frees all memory associated with the mapped and unmapped segments 
* returns: void   
*/
void free_segments()
{
	/* free all segments in mapped sequence */
	int mapped_length = Seq_length(mapped); 
	for (int i = 0; i < mapped_length; i++) {
		free_segment(i); 
	}

	Seq_free(&mapped); 

	int unmapped_length = Seq_length(unmapped); 
	int *index; 
	/* free all indicies in unmapped */
	for (int i = 0; i < unmapped_length; i++) {
		index = Seq_get(unmapped, i);
		if (index != NULL)
			free(index); 
	} 
	Seq_free(&unmapped); 
}

/* 
* name: duplicate_to_segment_one
* parameters: int with index 
* does: dupicates a segment and loads it into segment one 
* returns: void   
*/
void duplicate_to_segment_zero(int index)
{
	assert(index < Seq_length(mapped)); 
	Seq_T segment = Seq_get(mapped, index);
	assert(segment != NULL); 

	free_segment(0); 

	int segment_length = Seq_length(segment); 
	Seq_T new_segment = Seq_new(segment_length);
	Word* word; 
	Word* current_value; 
	/* make copy of segment with given index */
	for (int i = 0; i < segment_length; i++) {
		word = malloc(sizeof(Word));
		current_value = Seq_get(segment, i);
		*word = *current_value;
		Seq_addhi(new_segment, word);
	}

	Seq_put(mapped, 0, new_segment);
}






