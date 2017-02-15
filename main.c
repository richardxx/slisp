/*
	The main entry to the program , but it seems nothing but interfaces .

	To understand this program , a better way is to read the developer guide
	first , then read the language specs to know how to program with this
	small language , at last read the sources .

									*/

#include <stdio.h>
#include "trace.h"
#include "opt.h"
#include "interpreter.h"
#include "err.h"

char* current_proc=NULL;

int main(int argc , char** argv)
{

  	int rt=0;

	if ((rt=parse_parameter(argc,argv))==FALSE)
		goto err_end;

	init_err_table();
	if ((rt=tell_argument())==FALSE)
	  	goto err_end;

	interpreter(src_descriptor);

	exit_execution();

err_end:
	
	err_flush();
	exit_execution();
	
	return rt;
}

