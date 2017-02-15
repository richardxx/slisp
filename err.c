/*
	The error interpretering routines .

	The definition is straight forward .
									*/

#include <stdio.h>
#include "trace.h"
#include "err.h"

char* err_exp_table[ERRNUM+1];
char  err_attach_msg[ATTNUM+1][MAX_ATLENGTH];
int err_no=0;
int att_no=0;
extern int lineno;

void init_err_table()
{
  	err_exp_table[E_NO_THIS_ERROR]="If I was printed out , maybe your computer need pills\n";
	err_exp_table[E_NOT_A_COMMAND]="This command haven't defined yet , please read the language specs\n";
	err_exp_table[E_WRONG_BRACE]="You typed a wrong statement , the braces are mismatched\n";
	err_exp_table[E_MISS_EQUAL]="The definition of a list with a \'=\' missed\n";
	err_exp_table[E_LSTNAME_LONG]="Name of list must less than 32 characters\n";
	err_exp_table[E_REDEFINEMENT ]="You can only define a list once through the program \n";
	err_exp_table[E_RECURSIVE_DEF]="This version of language don't support recursive definition\n";
	err_exp_table[E_EXPR_TOO_LONG]="One statement must less than 512 characters\n";
	err_exp_table[E_NEED_DEFINE]="Execute this command need all lists defined\n";
	err_exp_table[E_NO_SUCH_LIST ]="You haven't ever defined this list\n";
	err_exp_table[E_NODIRECTIVE]="This version of language doesn't have this directive\n";
	err_exp_table[E_WRONG_NAME ]="Name of member in a list go against the name rule\n";
	err_exp_table[E_ELMNAME_LONG]="Name of a element must less than 32 characters\n";
	err_exp_table[E_NODIGIT ]="I don't support digit in this version\n";
	err_exp_table[E_WRONG_SYMBOL ]="You can't give a unrecognized symbol\n";
	err_exp_table[E_UNCAUGHT_SYMBL]="So silly to give such symbol to me\n";
	err_exp_table[E_ILLEGAL_LIST_NAME]="A list name in definition goes wrong\n";
	err_exp_table[E_SYNTAX_ERROR]="You have typed a illegal statement , please check it\n";
	err_exp_table[E_MISS_COMMA ]="Maybe your finger moves so fast , a comma is missed\n";
	err_exp_table[E_UNRESOLVED_SYMBOL ]="You refer to a undefined or undeclared list\n";
	err_exp_table[E_NOT_DECLARED]="This list has never been declared before\n";
	err_exp_table[E_MISS_SEPARATOR]="A name must followed by a separator , such as ',' or ')'\n";
	err_exp_table[E_NOT_A_LISTNAME]="This list name is illegal , please check it\n";
	err_exp_table[E_RUNTIME_LIST_NOT_EXIST]="The list you used as a sub list didn't exist at linking time\n";
	err_exp_table[E_MISS_SEMICOLON]="Your statement miss a semicolon as terminator\n";
	err_exp_table[E_WRONG_CMDNAME]="You give a invalid command name , please check it\n";
}

void err_set(int flag)
{
	/*	This promise you can set more than one at one time	*/

	if (flag<ERRNUM && flag>=0)
		err_no|=(1<<(flag-1));

}

void err_attach(int aflag)
{
	
	int i=0,j=0;
  	for (;i<ATTNUM;++i)
	{
		j=aflag&(1<<i);
		
		if ((att_no&j)!=0)	continue;

		switch (j)
		{
		case RP_LINE:
			sprintf(err_attach_msg[RP_LINE],"Error occur at line: %d\n",lineno);
			break;

		case RP_PROPOSAL:
			sprintf(err_attach_msg[RP_PROPOSAL],"not implemented\n");
			break;
	
		case RP_PROCEDURE:
			sprintf(err_attach_msg[RP_PROCEDURE],"In internal procedure \"%s\":\n",current_proc);
			break;
		
		default:
			break;

		}
	
	}

	att_no=aflag;
}

void err_flush()
{
	int i=0;
	unsigned int j=0;
	
	if (att_no==0)
		;
	else
	{
		for(;j<ATTNUM;++j)
		{
			i=att_no&(1<<j);
			if (i!=0)
				fprintf(stderr,err_attach_msg[i]);
		}
	}

	if (err_no==0)
		;
	else
	{
		
	  	i=ERRNUM-1;
		j=1<<i;
		while (j!=0)
		{
			if ((err_no&j)!=0)
				fprintf(stderr,err_exp_table[i]);
	
			j>>=1;
			--i;
		}
	}

	
	err_no=0;
	att_no=0;
}

#ifdef	DEBUG

void print_internal_status()
{
	fprintf(stderr,"ERROR VECTOR:%x\n",err_no);
	fprintf(stderr,"ATTCH VECTOR:%x\n",att_no);

}

#endif
