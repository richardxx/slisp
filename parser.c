/*
	I use a simple way named predict analysis , and for such a small
	language , it is enough .

									*/

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "lexical.h"
#include "parser.h"
#include "sys_ds.h"
#include "aux.h"
#include "err.h"

/*	The following function just for internal using		*/

static union Lists** handle_first_brace(struct def_list* dll)
{
  	char* token=NULL;

	token=sys_token_get();

	/*	The first token must be brace				*/
	
	if (token==NULL)
	{
		err_set(E_WRONG_SYMBOL);
		err_attach(RP_LINE);
		return NULL;
	}

	if (match(LEFT_BRACE,token)==TRUE)
		;
	else
	{
		err_set(E_SYNTAX_ERROR);
		err_attach(RP_LINE);
		return NULL;
	}

	return &dll->entry;
}

/*
***
***/

int parser(struct def_list* dl)
{
	/*
		pl plays a very important part in my program . Since I can use
		recursive routine to complete this , but I develop a non-recursive
		routine , so I use pl to decide the program control flow .*/

	union Lists** pl=NULL;
	union Lists* t=NULL;
	char* token=NULL;
	char* __std_rand_name=NULL;
	int symbol_mark=TRUE;		// FALSE=has no reference

	pl=handle_first_brace(dl);
	if (pl==NULL)
		return FALSE;
	
	sys_stack_clean();
	
	while (1)		
	{
		/*
			According to the combination , every time read one
			word over head to control the program flow .	*/

		token=sys_token_get();
		
		/*	Simply drop comma , because I have checked the 
			validation in lexical analysis .		*/

		if (match(COMMA,token)==TRUE)
			;

		/*	When encounter this , I know it will allocate 
				a new memory to hold the sub list	*/
		 
		else if (match(LEFT_BRACE,token)==TRUE)
		{
			
			t=(union Lists*)NEW(union Lists);
			*pl=t;
			
			get_random_list_name((*pl)->list_node.list_name);

			INIT4((*pl),list_node.flag,0,list_node.local_variable,0,
				list_node.sub_next,NULL,list_node.next,NULL);

			/*	When follow into the sub list , I must store the
				pointer before .			*/

			sys_stack_push(*pl);
			pl=&((*pl)->list_node.sub_next);
			
		}
		
		/*	Reference to another defined or declared list	*/

		else if (match(LIST_NAME,token)==TRUE)
		{
		  	if (strcmp(token,dl->list_name)==0)
			{
			  	/* This is a recursive definition list			*/

				t=(union Lists*)NEW(union Lists);
				*pl=t;

				INIT5((*pl),list_node.flag,0,list_node.local_variable,1,
					list_node.sub_next,NULL,list_node.next,NULL,
					list_node.recursive,TRUE);
				
				(*pl)->list_node.sub_next=dl->entry;
			}

			else 
			{
				if (exist(token)==FALSE)
				{
					err_set(E_UNRESOLVED_SYMBOL);
					err_attach(RP_LINE);
					return FALSE;
				}

				/*
					If it is a list name , then it just leave 
					the item in the lists structure , and it will
					be linked later during the execution .	
										*/
				t=(union Lists*)NEW(union Lists);
				*pl=t;
				
				INIT5((*pl),list_node.flag,0,list_node.local_variable,1,
					list_node.sub_next,NULL,list_node.next,NULL,
					list_node.recursive,FALSE);
			
				symbol_mark=TRUE;	
			}

			strcpy((*pl)->list_node.list_name,token);
	
			pl=&((*pl)->list_node.next);
		}
		
		/*	An element in the sub list			*/

		else if (match(ELEM_NAME,token)==TRUE)
		{

			/*
				Just add the name to the List , and
				follow the link to go			*/

			t=(union Lists*)NEW(union Lists);
			*pl=t;
			
			INIT2((*pl),elem_node.flag,1,elem_node.next,NULL);
			
			strcpy((*pl)->elem_node.elem_name,token);
		
			pl=&((*pl)->elem_node.next);

		}
		
		/*	Encounter an end of the definition of sub list	*/

		else if (match(RIGHT_BRACE,token)==TRUE)
		{
		
			t=sys_stack_pop();
			if (t==NULL)	continue;
			pl=&(t->list_node.next);
			
		}
		
		else if (match(SEMICOLON,token)==TRUE)
			break;

		else
		{
			/*	Unexpected symbol occurs..		*/
			/*	But I will not let it reach here	*/

			err_set(E_SYNTAX_ERROR);
			err_attach(RP_LINE);
			return FALSE;
		}
	
	}

	/*	If I didn't dectect uppercase , skip dynamic_binding	*/

	switch (symbol_mark)
	{
		case TRUE:
			dl->has_no_export_symbol=FALSE;
			break;
			
		case FALSE:
			dl->has_no_export_symbol=TRUE;
			break;
	}

	sys_token_clean();

	return TRUE;
}

