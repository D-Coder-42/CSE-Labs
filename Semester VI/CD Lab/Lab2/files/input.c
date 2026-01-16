// This is a single line comment

/* ***** This is a
   ***** Multiline Comment
   ***** 
*/

#include<stdio.h>
#iclnude"mpi.h"
#define PI 3.141592f

void main() {
	FILE *fopen(), *fp;
	int c ;
	fp = fopen(              “prog.c”, “r” ); //Comment
	c = getc( fp ) ;
	while ( c != EOF ) {
		putchar( c );
		c = getc ( fp );
	}
	/* multiline
	comment */
	fclose(fp			);
	a/ "   "
	printf("End of    		code\n");
	return 0;
}
