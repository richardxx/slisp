/*
	The parser of the list representation .
	For example , if you give a expression like this :

		def A=(dfdf,B,(ggf,C),(fgf,B,ty(D,C)));	,

	the machine learn to construct the proper data structure for it however
	it is complex as above .

	Before do the really job , you must call lexical function to split it
	into small pieces named tokens , and then record the information .

									*/

#ifndef _PARSER_H

#define _PERSER_H

#include "types.h"

int parser(struct def_list*);

#endif
