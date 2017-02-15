/*
	this file contains enough macros and variables for debuging
	error checking .				
									*/

#ifndef	_TRACE_H

#define _TRACE_H


#define DEBUG			0x9075028

#define LEXICAL			"lexical"
#define PARSER			"parser"
#define PREPROCESSOR		"preprocessor"
#define READSENTENCE		"read next statement"
#define DYNAMIC_BINDING		"dynamic_binding"
#define EXPRCHECK		"check expression"
#define PDEF			"define"
#define PETN			"extern"
#define PPRT			"print"
#define PHAD			"head"
#define PTAL			"tail"
#define PDPT			"depth"
#define PCNT			"count"
#define PINV			"inverse"

#define BYTE_PER_WORD		4
#define BITS_PER_BYTE		8

#ifdef	DEBUG
	#define TRACE_ME(fff)		current_proc=fff;
#else
	#define TRACE_ME(fff)		current_proc=""
#endif

extern char* current_proc;	/*	point to one of above		*/

#endif



