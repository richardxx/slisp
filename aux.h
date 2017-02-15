/*
	There comes more auxiliary funtions .
	
	Something more often used utilities are all placed here , they are 
	mainly used by the parser , lexical and command interpreter .

	I will explain the usage when overcomming .
									*/

#ifndef _AUX_H

#define _AUX_H


#ifndef MAX_DIRECTIVE_LENGTH
	#define MAX_DIRECTIVE_LENGTH	5
#endif

#define COND_UPPER 	upper
#define COND_LOWER 	lower
#define COND_SEPARATOR 	separator


int	match(int,const char*);			/*  inpect if it matchese the flag      */

int	exist(const char*);			/*  if the symbol has ever appeared	*/

int 	chk_braces_match();			/*  To check if the braces are in pair	*/

int 	chk_expr(const char*);			/*  check if a expression is leagle     */

int	instrc(const char*,const char,int);	/*  get the position of the char       	*/

int 	instr(const char*,const char*,int);	/*  get the position of the sub string 	*/

void get_random_list_name(char*);	/*  Give a name for a unamed list	*/

void remove_all_spaces();		/*  Remove all leading spaces		*/

int cond_upper(char);
int cond_lower(char);
int cond_separator(char);

#endif
