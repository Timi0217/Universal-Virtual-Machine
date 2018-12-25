/*
 * Justin Dorosh; JDORS01
 * Timi Dayo-Kayode; ODAYOK01
 * COMP 40
 * HW 6
 * 04/05/2018
 * main.c
 *
 * Purpose: The purpose of this file is to handle command line arguments and 
 *	    to create the virtual machine and call functions in 
 *          UniversalMachine.h in order to begin processing instructions. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h> 
#include "universal_machine.h"
#include <except.h>


int main(int argc, char* argv[])
{

	struct stat file_stats; 

	/* check for proper number of command line arguments */
	if (argc != 2) {
		fprintf(stderr, "Wrong number of command line arguments.\n");
		exit(EXIT_FAILURE); 
	}

	/* get size of file in bits */
	if (stat(argv[1], &file_stats) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	} 

	FILE* pfile = fopen(argv[1], "r"); 
	if (pfile == NULL) {
		fprintf(stderr, "Erroor opening file.\n");
		exit(EXIT_FAILURE); 
	}

	run(pfile, file_stats.st_size); 
	return EXIT_SUCCESS; 
}
