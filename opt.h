/*
	This file has two task:
	1. Do arguments profiling;
	2. Do safely check out .

									*/

#ifndef _OPTION_H

#define _OPTION_H

#include "types.h"

#ifdef VERSION
	#undef VERSION
	#define VERSION		"0.11"
#endif

#define PLATFORM	"RedHat Linux 7.0"
#define LANG		"Pure C Language"
#define NGCC		"GNU C compiler"
#define VGCC		2.96
#define AUTHOR		"Richard Leon"

/*	Arguments bitmask						*/

#define ARGNUM		3
#define HELP		2
#define VERSION		4
#define HIDE		8

/*	global variables						*/

#ifdef _STDIO_H
	extern FILE* src_descriptor;
#endif

extern char my_src_file[128];
extern char prefix[4];
extern char* arg_vector[ARGNUM+1];
extern int arguments;		


int parse_parameter(int,char**);	/* get the input information	*/

int tell_argument();			/* interpret the arguments	*/

int chk_open_file();			/* Check the input of user	*/

int print_usage();			/* print a short message	*/

int print_version();			/* print the version info	*/

int print_hide();			/* print hide information	*/

/*int prepare_to_execute();		 Build a proper environment 	*/

int exit_execution();			/* release memory and exit	*/


#endif

