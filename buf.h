/*
   	I have improved the buffer design , this time the buffer is an system 
	level component , not just the part of the lexical analyzer . The new
	buffer is used to handle the file input , and this can reduce a lot of 
	character copying times , the details will go to the developer guide .

	The buffer interface has been envolved into second edition , I hava added a lot
	of features , and to be simple I just let the discussion go to the developer 
	guide .
										*/
  
#ifndef _BUFFER_H

#define _BUFFER_H

#define DATA_POND_SIZE		4096
#define BUFFER_SIZE		MAX_EXPR_LENGTH		
#define TOTAL_SIZE              BUFFER_SIZE<<2+2

/* The following caculation exlude the buffer null terminator			*/

#define BUFFER_STAPOS(n)	(BUFFER_SIZE+1)*(n)	
#define BUFFER_ENDPOS(n)	BUFFER_SIZE-1+(n)*(BUFFER_SIZE+1)

/*	The following are lock relative macros					*/
/*	Explanation:
		RB-----------read only with boundary limitation,type=1
		RO-----------read only,type=2
		SW-----------self swapping out,type=3
										*/

#define RB_BUFFER_1		0x00000001
#define RB_BUFFER_2		0x00000002
#define RO_BUFFER_1		0x00000004
#define RO_BUFFER_2		0x00000008
#define SW_BUFFER_1		0x00000010
#define SW_BUFFER_2		0x00000020

#define RB_LOCK			1
#define RO_LOCK			2
#define SW_LOCK			3
#define RB			RB_LOCK
#define RO			RO_LOCK
#define SW			SW_LOCK
#define MAKE_LOCK(type,which)	type##_BUFFER_##which

#define END_OF_FILE		'\0'
#define END_OF_LINE		'\0'
#define EOS			29

#define END_LOCKED_BUFFER	28		/* pi	*/
#define NEXT_LOCKED		6628537

typedef int (*read_cmp)(char); /* give the condition if it would continue */

/*	I aimed to protect the whole raw data , so I will not export the storage variables	*/

void 	init_buffer();			/* set the default value			*/
int 	load_data();			/* fulfill the data pond			*/
int 	input_buffer(int);		/* fill the cache with data(with proprecessor)	*/ 
char  	buffer_getchar();		/* get the next character in the cache		*/
void	buffer_ungetchar();		/* backward one character			*/
int     buffer_get_interval(int,int,char*);      /* get a sub-string from the selection         */
int     buffer_cond_read(read_cmp,char*);        /* more powerful than the others            */
void    buffer_flush(int);		/* clear up the cache				*/ 
int 	buffer_getline(char*);		/* read across the buffer gap			*/
void 	buffer_skip_sentence();		/* drop this sentence				*/

int 	buffer_searchc(const char,int);	/* (character)just in current buffer		*/
//int 	buffer_searchs(const char*,int);/* (string)just in current buffer		*/
int 	buffer_gbl_searchc(const char,int);	/* read until at the end of next buffer	*/
int     buffer_searchc_until(const char ,int,const char); /* find  the character until the delimeter */
int	move_to(int);			/* directly move to the given position		*/
int 	move_add_to(int);		/* argument is the offset			*/
int	get_pointer();			/* get current buffer pointer position		*/

int 	lock_buffer(int,int);		/* if locked , then cannot be flushed		*/
int 	unlock_buffer(int,int);		/* the inverse operation			*/
int 	is_read_only(int);		/* if I could overlay the specified buffer	*/
int	is_boundary(int);		/* if the cursor can move out the buffer	*/
void	sweep_swlock(int);		/* clean the self-swapping lock			*/

int	has_no_data();			/* if there's no data in the source file	*/
int     GET_LOCK(int,int);	/* ever since it's a macro			*/

#endif
