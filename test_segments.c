/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 */

#include "segments.h"
#include "assert.h"

int main()
{
	Seq_T mapped;
	Seq_T unmapped;
	//Seq_T segment;
	Seq_T temp;
	int length_mapped; //length of the sequence containing mapped segments.
	int length_unmapped; //length of sequence containing unmapped segment id
	int word_count = 0; //num words in a segment
	int seg_id; //a segment id for testing purposes
	int offset; //an offset for testing purposes
	word value; //an instance of the word datatype for testing purposes
	word value2; //an instance of the word datatype for testing purposes
	mapped = Seq_new(0);
	unmapped = Seq_new(0);

	/* first test: Test that when map_segment is called the value it returns
	 * is equal to 1 + the previous length of the sequence containing the 
	 * segments
	*/

	//testing adding to our sequence
	word_count = map_segment(5);
	length_mapped = Seq_length(mapped);
	assert(length_mapped == 1 && word_count == 5);

	word_count = map_segment(2); //this adds another segment to our mapped
	assert(length_mapped == 2 && word_count == 2);

	//testing removing from our sequence
	unmap_segment(3); //this should be an error cause it goes out of bounds

	unmap_segment(0);
	length_mapped = Seq_length(mapped);
	length_unmapped = Seq_length(unmapped);

	assert(length_mapped == 1 && length_unmapped == 1);


	seg_id = 0; //this segment has been unmapped
	offset = 2;
	value = 32;
	add_word_to_memory(seg_id, offset, value); //this shld be an error
	//because the segment has been unmapped and the seg_id now exists in
	//the unmapped sequence
	seg_id = 1;
	add_word_to_memory(seg_id, offset, value);

	temp = (Seq_T)Seq_get(mapped, 1);
	assert(Seq_length(temp) == 1);//makes sure that there is just one value
	//in the segment

	value2 = retrieve_word_from_memory(1, 2);
	assert(value == value2); //makes sure that the value added and 
	//retrieved are the same.
	
	value2 = retrieve_word_from_memory(0, 2); //exception thrown from
	//function because segment 0 exists in unmapped.

	add_word_to_memory(seg_id, offset, value);
	add_word_to_memory(seg_id, offset, value); //this call should throw an 
	//exception from the add_word_to_memory function because the 
	//segment is already full

	return 0;
}

/*
void initialize_segments(int seg_zero_size){
	mapped = Seq_new(0);
	unmapped = Seq_new(0);
	Seq_new *segment = Seq_new(seg_zero_size);
	seq_put(mapped, 0, segment);
}

int map_segment(int word_count){
	return word_count;
}

void unmap_segment(int identifier){
	(void) identifier;
}

void add_word_to_memory(int seg_id, int offset, word value){
	(void) seg_id;
	(void) offset;
	(void) value;
}

word retrieve_word_from_memory(int seg_id, int offset){
	word my_word = 10;
	(void) seg_id;
	(void) offset;
	return my_word;
}
*/
