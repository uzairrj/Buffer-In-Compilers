#ifndef BUFFER_H_
#define BUFFER_H_

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* constant definitions */
#define RT_FAIL_1 (-1)         /* operation failure return value 1 */
#define RT_FAIL_2 (-2)         /* operation failure return value 2 */
#define LOAD_FAIL (-2)         /* load fail return value */

#define DEFAULT_INIT_CAPACITY 200   /* default initial buffer capacity */
#define DEFAULT_INC_FACTOR 15       /* default increment factor */

/* Add your bit-masks constant definitions here */
#define DEFAULT_FLAGS  0xFFFC
#define SET_EOB 0x0002/*0000 0000 0000 0010*/
#define RESET_EOB 
#define CHECK_EOB 0x0002 /*0000 0000 0000 0010*/
#define SET_R_FLAG 0x0001 /* 0000 0000 0000 0001 */
#define RESET_R_FLAG 0xFFFE   /* 1111 1111 1111 1110 */
#define CHECK_R_FLAG 0x0001   /* 0000 0000 0000 0001 */


/* user data type declarations */
typedef struct BufferDescriptor {
    char *cb_head;   /* pointer to the beginning of character array (character buffer) */
    short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
    short addc_offset;  /* the offset (in chars) to the add-character location */
    short getc_offset;  /* the offset (in chars) to the get-character location */
    short markc_offset; /* the offset (in chars) to the mark location */
    char  inc_factor; /* character array increment factor */
    char  mode;       /* operational mode indicator*/
    unsigned short flags;     /* contains character array reallocation flag and end-of-buffer flag */
} Buffer, *pBuffer;


/* function declarations */
Buffer *b_alloc(short init_capacity,char inc_factor, char o_mode);
int b_isfull(Buffer * const pBD);
pBuffer b_addc(pBuffer const pBD,char symbol);
Buffer * b_compact(Buffer * const pBD, char symbol);
int b_load (FILE * const fi, Buffer * const pBD);
int b_eob (Buffer * const pBD);
int b_clear (Buffer * const pBD);
void b_free (Buffer * const pBD);
short b_limit (Buffer * const pBD);
short b_capacity(Buffer * const pBD);
short b_mark (pBuffer const pBD, short mark);
int b_mode (Buffer * const pBD);
size_t b_incfactor (Buffer * const pBD);
int b_isempty (Buffer * const pBD);
char b_getc (Buffer * const pBD);
int b_print (Buffer * const pBD, char nl);
char b_rflag (Buffer * const pBD);
short b_retract (Buffer * const pBD);
short b_reset (Buffer * const pBD);
short b_getcoffset (Buffer * const pBD);
int b_rewind(Buffer * const pBD);
char * b_location(Buffer * const pBD);

#endif

