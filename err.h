/*
	Error handling procedures .

	I have carefully enumrated all the errors in this edition of language ,
	and all are descriped as intergers here . Like the standard C error
	handling machnism , each error number associated with a short string .
									*/

#ifndef _ERRORS_H

#define _ERRORS_H

/*
	The following table is made from bash shell script		*/

#define ERRNUM				 26

#define E_NO_THIS_ERROR			 0
#define E_NOT_A_COMMAND 		 1
#define E_WRONG_BRACE 			 2
#define E_MISS_EQUAL 			 3
#define E_LSTNAME_LONG 			 4
#define E_REDEFINEMENT 			 5
#define E_RECURSIVE_DEF 		 6
#define E_EXPR_TOO_LONG 		 7
#define E_NEED_DEFINE 			 8
#define E_NO_SUCH_LIST 			 9
#define E_NODIRECTIVE 			 10
#define E_WRONG_NAME 			 11
#define E_ELMNAME_LONG 			 12
#define E_NODIGIT 			 13
#define E_WRONG_SYMBOL 			 14
#define E_UNCAUGHT_SYMBL 		 15
#define E_ILLEGAL_LIST_NAME 		 16
#define E_SYNTAX_ERROR 			 17
#define E_MISS_COMMA 			 18
#define E_UNRESOLVED_SYMBOL 		 19
#define E_NOT_DECLARED			 20
#define E_MISS_SEPARATOR		 21
#define E_NOT_A_LISTNAME		 22
#define E_RUNTIME_LIST_NOT_EXIST	 23
#define E_MISS_SEMICOLON		 24
#define E_WRONG_CMDNAME                  25

#define ATTNUM				3

#define MAX_ATLENGTH			64
#define RP_PROCEDURE			1
#define RP_LINE				2
#define RP_PROPOSAL			4

extern char* 	err_exp_table[ERRNUM+1];
extern char     err_attach_msg[ATTNUM+1][MAX_ATLENGTH];

extern int err_no;				/*  Error bit vector	*/
extern int att_no;				/*  Attachment bit vector */
extern int lineno;				/*  From interpreter.h  */

void init_err_table();
void err_set(int);
void err_attach(int);
void err_flush();

#ifdef	DEBUG
	void print_internal_status();
#endif

#endif




