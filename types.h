/*
	The constants , macros and generalized data structures go here .

									*/
#ifndef	_TYPES_H

#define _TYPES_H

#include "trace.h"

#define TRUE		0
#define FALSE		-1
#define END		-2
#ifndef NULL
	#define NULL	0				/* bad idea  */
#else
	#undef NULL
	#define NULL	0
#endif

#define	MAX_LIST_NAME			32
#define MAX_ELEM_NAME			32
#define MAX_TOKEN_NAME 			32
#define MAX_EXPR_LENGTH			256
#define MAX_ARG_LENGTH			128
#define MAX_RECURSIVE_PRINT_DEPTH	1
#define INFINITY			-1987

/*	The tokens elements used in my program , name as it is .	*/

#define LEFT_BRACE		1		/* (  */
#define RIGHT_BRACE		2		/* )  */
#define LIST_NAME		3		
#define	ELEM_NAME		4
#define	COMMA			5		/* ,  */
#define SEMICOLON		6		/* ;  */

#define EMPTY_LIST		"ABCDEFGGFEDCBA\0"
#define END_OF_EXPRESSION	"abcdefggfedcba\0"
#define END_OF_STREAM		124

#define EMPTY_LIST_SIZE		15
#define EMPTY_EXPR_SIZE		15

#define GET_FUNC_NAME(part1,part2) part1##part2
#define _PROTOTYPE(rt_type,m_name,a_name,arg_type,arg) rt_type m_name##a_name(arg_type arg)
/*
	Some most likely useful macros			
	But if you want to use them , don't remember to include the stdlib.h
	first .						
									*/

#define NEW(STRUCT)		malloc(sizeof(STRUCT))
#define NEW_ARRAY(STRUCT,NUM)	malloc(sizeof(STRUCT)*(NUM))
#define DELETE(POINTER)		free(POINTER)

#define NUMBER_OF(PSTRING,RET)	 \
	do					\
	{					\
		while((PSTRING[RET++])!='\0'); \
		--RET;				\
	}					\
	while(0)

#define MAKE_STRING(PSTRING,POS)	PSTRING[(POS)]='\0'

#define EXPAND_STRING(PSTRING,MORE_SIZE) \
   	do							\
	{							\
		while(*PSTRING++!='\0');			\
		--PSTRING;					\
		int __macro_index=0;				\
		while (__macro_index<MORE_SIZE)			\
			PSTRING[__macro_index++]=' ';		\
		MAKE_STRING(PSTRING,__macro_index);		\
	}							\
	while(0)
				
#define INIT1(POINTER,arg1,val1) \
	do				\
	{				\
		POINTER->arg1=(val1); 	\
	}				\
	while(0)			

#define INIT2(POINTER,arg1,val1,arg2,val2) \
	do				\
	{				\
		POINTER->arg1=(val1);	\
		POINTER->arg2=(val2);	\
	}				\
	while(0)

#define INIT3(POINTER,arg1,val1,arg2,val2,arg3,val3) \
	do				\
	{				\
		POINTER->arg1=(val1);	\
		POINTER->arg2=(val2);	\
		POINTER->arg3=(val3);	\
	}				\
	while(0)

#define INIT4(POINTER,arg1,val1,arg2,val2,arg3,val3,arg4,val4) \
	do				\
	{				\
		POINTER->arg1=(val1);	\
		POINTER->arg2=(val2);	\
		POINTER->arg3=(val3);	\
		POINTER->arg4=(val4);	\
	}				\
	while(0)

#define INIT5(POINTER,arg1,val1,arg2,val2,arg3,val3,arg4,val4,arg5,val5) \
	do				\
	{				\
		POINTER->arg1=(val1);	\
		POINTER->arg2=(val2);	\
		POINTER->arg3=(val3);	\
		POINTER->arg4=(val4);	\
		POINTER->arg5=(val5);	\
	}				\
	while(0)


/*	the most important structures 					*/

union Lists;

struct __std_list
{
	int flag;			//	0
	int local_variable;          	//	0 if it is , 1 otherwise 
	union Lists* sub_next;		//	point to the sub list
	union Lists* next;		//	point to the next list
	int recursive;			//	true if sub_next point to itself
	char list_name[MAX_LIST_NAME+1];
};

struct __std_elem
{
	int flag;			//1
	union Lists* next;		//point to next element
	char elem_name[MAX_ELEM_NAME+1];
};

union Lists
{
	int flag;				//indicate which structure is  used
	
	struct __std_list list_node;

	struct __std_elem elem_node;
};

/*
	this structure stores the information about a defined list ,
	later you can only use them but not the only declared items to be
	the argument of the internal command .				*/

struct def_list
{
	int has_no_export_symbol;		//if it is , it can be used , or else be not
	char list_name[MAX_LIST_NAME+1];	//the name of the list
	union Lists* entry;			//point to the real list
	struct def_list* next;
};

struct dec_list
{
	int has_defined;			//if it is , it can be used
	char list_name[MAX_LIST_NAME+1];	//only declare but not define
	union Lists* entry;			//when possible , connect them
	struct dec_list* next;
};

#endif
