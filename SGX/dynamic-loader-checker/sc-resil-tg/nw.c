//#include <stdlib.h>
//#include <string.h>

#include "enclave.h"

//#include "CFICheck.h"
//#include "CFICheck.c"

int  max( int f1, int f2, int f3, char * ptr )
{
	//CFICheck(0);
	int  max = 0 ;
	if( f1 >= f2 && f1 >= f3 )
	{
		max = f1 ;
		*ptr = '|' ;
	}
	else if( f2 > f3 )
	{
		max = f2 ;
		*ptr = '\\' ;
	}
	else
	{
		max = f3 ;
		*ptr = '-' ;
	}
	return  max ;
}

void dpm_init( int ** F, char ** traceback, int L1, int L2, int d )
{
	//Weijie:
	//unsigned int magic_in_dpm = 0;
	F[ 0 ][ 0 ] =  0 ;
	traceback[ 0 ][ 0 ] = 'n' ;

	int i=0, j=0;
	for( j = 1; j <= L1; j++ )
	{
		F[ 0 ][ j ] =  -j * d ;
		traceback[ 0 ][ j ] =  '-' ;
	}
	for( i = 1; i <= L2; i++ )
	{
		F[ i ][ 0 ] =  -i * d ;
		traceback[ i ][ 0 ] =  '|' ;
	}
}

size_t my_strlen(const char *str)
{
	/*
	const char *s;
	for (s = str; *s; ++s);
	return (s - str);
	*/
	if ( (str == NULL) || (*str == '\0') ) {
		return 0;
	}
	else {
		return my_strlen(str+1)+1;
	}
}


void print_matrix( int ** F, char *seq_1, char *seq_2 )
{
	int  L1 = my_strlen(seq_1);
	int  L2 = my_strlen(seq_2);
	//PrintDebugInfo("        ");
	for( int j = 0; j < L1; j++ )
	{
		//PrintDebugInfo("%c   ", seq_1[ j ]);
	}
	//PrintDebugInfo("\n");

	for( int i = 0; i <= L2; i++ )
	{
		if( i > 0 )
		{
			//PrintDebugInfo("%c ", seq_2[ i-1 ]);
		}
		for( int j = 0; j <= L1; j++ )
		{
			//PrintDebugInfo("%d ", F[ i ][ j ]);
		}
		//PrintDebugInfo("\n");
	}
}

void  print_traceback( char ** traceback, char *seq_1, char *seq_2 )
{
	int  L1 = my_strlen(seq_1);
	int  L2 = my_strlen(seq_2);

	for( int j = 0; j < L1; j++ )
	{
		//PrintDebugInfo("%c ", seq_1[ j ]);
	}
	puts("\n");

	for( int i = 0; i <= L2; i++ )
	{
		if( i > 0 )
		{
			//PrintDebugInfo("%c ", seq_2[ i-1 ]);
		}
		for( int j = 0; j <= L1; j++ )
		{
			//PrintDebugInfo("%c ", traceback[ i ][ j ]);
		}
		//PrintDebugInfo("\n");
	}
}

/* implemented by Weijie */
char *my_strrev(char *str)
{
	char *p1, *p2;
	if (! str || ! *str)
		return str;
	for (p1 = str, p2 = str + my_strlen(str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}
	
char *strncat(char *dst, const char *src, size_t n)
{
	if (n != 0) {
		char *d = dst;
		const char *s = src;

		while (*d != 0)
			d++;
		do {
			if ((*d = *s++) == 0)
				break;
			d++;
		} while (--n != 0);
		*d = 0;
	}
	return (dst);
}

int nw_align(                  // Needleman-Wunsch algorithm
		int **     F,
		char **    traceback,
		char *seq_1,
		char *seq_2,
		char *seq_1_al,
		char *seq_2_al,
		int        d         // Gap penalty
		)
{
	int        k = 0, x = 0, y = 0;

	int        fU, fD, fL ;
	char       ptr = NULL, nuc ;
	int        i = 0, j = 0;

	const int  a =  2;   // Match
	const int  b = -1;   // Mismatch

	const int  s[ 4 ][ 4 ] = { { a, b, b, b },    /* substitution matrix */
		{ b, a, b, b },
		{ b, b, a, b },
		{ b, b, b, a } } ;

	int  L1 = my_strlen(seq_1);
	int  L2 = my_strlen(seq_2);

	/* define by Weijie */
	//char attach[2] = "";
	//Weijie: no memset
	char attach[2];

	int temp = 0;
	//Weijie:
	//L1 = 8;
	//L2 = 7;

	for( i = 1; i <= L2; i++ )
	{
		for( j = 1; j <= L1; j++ )
		{
			nuc = seq_1[ j-1 ] ;
			switch( nuc )
			{
				case 'A':  x = 0 ;  break ;
				case 'C':  x = 1 ;  break ;
				case 'G':  x = 2 ;  break ;
				case 'T':  x = 3 ;
			}
			nuc = seq_2[ i-1 ] ;
			switch( nuc )
			{
				case 'A':  y = 0 ;  break ;
				case 'C':  y = 1 ;  break ;
				case 'G':  y = 2 ;  break ;
				case 'T':  y = 3 ;
			}
			fU = F[ i-1 ][ j ] - d ;
			fD = F[ i-1 ][ j-1 ] + s[ x ][ y ] ;
			fL = F[ i ][ j-1 ] - d ;
			temp = max( fU, fD, fL, &ptr ) ;
			F[ i ][ j ] = temp;
			traceback[ i ][ j ] =  ptr ;
		}
	}


	i-- ; j-- ;
	while( i > 0 || j > 0 )
	{
		switch( traceback[ i ][ j ] )
		{

			case '|' :      //seq_1_al += '-' ;
				strncat(seq_1_al, "-", 1);
				//seq_2_al += seq_2[ i-1 ] ;
				attach[0] = seq_2[i-1];
				attach[1] = '\0';
				strncat(seq_2_al, attach, 1);
				i-- ;
				break ;

			case '\\':      //seq_1_al += seq_1[ j-1 ] ;
				attach[0] = seq_1[j-1];
				attach[1] = '\0';
				strncat(seq_1_al, attach, 1);
				//seq_2_al += seq_2[ i-1 ] ;
				attach[0] = seq_2[i-1];
				attach[1] = '\0';
				strncat(seq_2_al, attach, 1);
				i-- ;  j-- ;
				break ;

			case '-' :      //seq_1_al += seq_1[ j-1 ] ;
				attach[0] = seq_1[j-1];



				attach[1] = '\0';
				strncat(seq_1_al, attach, 1);
				//seq_2_al += '-' ;
				strncat(seq_2_al, "-", 1);
				j-- ;
		}
		k++ ;
	}

	/* reverse seq_als, modified by Weijie. */
	char *tmp1, *tmp2;
	tmp1 = my_strrev(seq_1_al);
	tmp2 = my_strrev(seq_2_al);
	return  0 ;
}

/* entry point function nw */
void Ecall_nw(
		char *seq_1,          /*  Needleman-Wunsch   */
		char *seq_2,          /*  algorithm for      */
		char *seq_1_al,       /*  global alignment   */
		char *seq_2_al        /*  of nt sequence.    */
		)
{
	int  d = 2 ;                 /* gap penalty */
	unsigned long long L1 = my_strlen(seq_1);
	unsigned long long L2 = my_strlen(seq_2);
	//Weijie:
	//puts("L2:");
	//char rvl2[9];
	//my_itoa(L2, rvl2, 10);
	//puts(rvl2);
	// Dynamic programming matrix
	int ** F = (int **)malloc( (L2 + 1) * sizeof(int *) );
	
	for( int i = 0; i <= L2; i++ ){
		F[ i ] = (int *)malloc( L1 * sizeof(int));
		//Weijie"
		if (F[i] == NULL)	puts("malloc failed!");
		//char rvmalloc[8];
		//char *rva = rvmalloc;
		//rva = my_itoa(F[i], rva, 10);
		//puts(rva);
	}
	// Traceback matrix
	
	char ** traceback = (char **)malloc( (L2 + 1) * sizeof(char *));
	for( int i = 0; i <= L2; i++ )  
		traceback[ i ] = (char *)malloc( L1 * sizeof(char));

	// Initialize traceback and F matrix (fill in first row and column)
	dpm_init( F, traceback, L1, L2, d );
	/* Initialize seq_als */
	
	seq_1_al[0] = '\0';
	seq_2_al[0] = '\0';
	
	// Create alignment
	int rv;
	rv = nw_align( F, traceback, seq_1, seq_2, seq_1_al, seq_2_al, d );
	

	if (rv == 0){ 
		print_matrix( F, seq_1, seq_2 );
		print_traceback( traceback, seq_1, seq_2 );
	}
}

#define MAXSEQLEN 10000

#include "nw_data.h"

void enclave_main(){
	char seq_1[] = "AGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATG";
	char seq_2[] = "ACGTCGTGCATGCATGATCGTACTACTGTTATAGCGGCGGCTATATATCG";
	//char seq_1[] = "AGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATGGGATCGTACTGAGCTAGCTACGATGTGACGTCGATGCATGCATTGTGATC";
	//char seq_2[] = "ACGTCGTGCATGCATGATCGTACTACTGTTATAGCGGCGGCTATATATCGTATATTATTTATTATATGGGGGGGGCAGCATGCTATGATGCATGCTAGCT";

	//char seq_1[] = "AGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATGACGTCGTGCATGCATGATCGTACTACTGTTATAGCGGCGGCTATATATCGAGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATGGGATCGTACTGAGCTAGCTACGATGTGACGTCGATGCATGCATTGTGATCAGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATGAGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATG";
	//char seq_2[] = "AGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATGACGTCGTGCATGCATGATCGTACTACTGTTATAGCGGCGGCTATATATCGAGTACGTCGAAGCGATGCATGCGAGAGTACTGGATCACGACTGATGCATGGGATCGTACTGAGCTAGCTACGATGTGACGTCGATGCATGCATTGTGATCACGTCGTGCATGCATGATCGTACTACTGTTATAGCGGCGGCTATATATCGACGTCGTGCATGCATGATCGTACTACTGTTATAGCGGCGGCTATATATCG";

	char seq_1_al[MAXSEQLEN];
	char seq_2_al[MAXSEQLEN];
	puts("running NW algorithm...");

	Ecall_nw(seq_1, seq_2, seq_1_al, seq_2_al);
	//Ecall_nw(data501, data502, seq_1_al, seq_2_al);
	//Ecall_nw(data1001, data1002, seq_1_al, seq_2_al);
	//Ecall_nw("ACTACGTC", "ACGTCGT", seq_1_al, seq_2_al);

	puts("exiting...");
	enclave_exit();
}
