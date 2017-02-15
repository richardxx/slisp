/*
	For this such a simple interpreter , I don't plan to use a complete 
	symbol table , so I devised two special purpose structures(stack and
	queue) to substitue it .

	Here's their interfaces for exporting .

									*/

#ifndef _SYS_DS_H

#define _SYS_DS_H

#include "types.h"

/*
	I use linked list to implement them .				*/

struct stk_node
{
  	union Lists* saved;			/* backup the pointer	*/
	struct stk_node* next;			/* point to next member	*/
};

struct que_node
{
	char tk_name[MAX_TOKEN_NAME+1];		/* the token name	*/
	struct que_node* next;			
};

extern struct stk_node* stk_head;
extern struct que_node* que_head;
extern struct que_node* que_tail;
extern union  Lists* last_saved;		/* store popped		*/


/*	Functions to manipulate them .					*/

/*	Generalized operation (only for stack)				*/

#define __create_new_stack()	NULL
struct	stk_node*  	__destroy_stack(struct stk_node*);
struct	stk_node* 	__stack_push(struct stk_node*,union Lists*);
struct	stk_node*	__stack_pop(struct stk_node*);
int			__stack_empty(struct stk_node*);

/*	I have crated a special purpose stack				*/		

union Lists*  sys_stack_pop();	/*	special treated			*/

#define sys_stack_clean()	stk_head=__destroy_stack(stk_head);
#define sys_stack_push(TTT) 	stk_head=__stack_push(stk_head,(TTT))
#define sys_stack_isempty()	__stack_empty(stk_head)	

/*	All are the same as above					*/

char*		sys_token_get();
void		sys_token_put(char*);
void 		sys_token_clean();
int		sys_token_isempty();


#endif
