/*

	The lexical funtion goes here .

	It processes forward to split the whole expression into small pieces ,
	and pass the result to the syntax parser .
	
	In future I will unify the buffer engine and lexical analyzer , and then
	it will be more like a real compiler .

*/

#ifndef _LEXICAL_H

#define _LEXICAL_H

extern char rt_token[MAX_TOKEN_NAME+1];		// comes from types.h

#include "types.h"

char* sys_token_get();				// get the next matched symbol

void sys_token_clean();				// prepare for next calling

int lexical();					// lexical analyzer

#endif
