/*
	The implementaion of the internal standard commands .
	All the expanded commands go to the expand.c and any other files .
	Better keep this file origin to be , just improve their algorithms .
	
									*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "internal.h"
#include "sys_ds.h"
#include "lk_list.h"
#include "parser.h"
#include "lexical.h"
#include "interpreter.h"
#include "buf.h"
#include "aux.h"
#include "err.h"

/*	Preserve for this file only					*/

static struct def_list* print_information()
{
	struct def_list* dl=NULL;

	if ((dl=def_list_search(st_expr_info.operand))==NULL)
	{
		if (dec_list_search(st_expr_info.operand)!=NULL)
		{
			err_set(E_NEED_DEFINE);
			err_attach(RP_LINE | RP_PROCEDURE);
		}
		
		else
		{
			err_set(E_NO_SUCH_LIST);
			err_attach(RP_LINE | RP_PROCEDURE);
		}

		return NULL;
	}
	
	fprintf(stdout,st_expr_info.arg);

	return dl;
}

int def(void)
{
	int pos=0;
	
	/*	Looks so ugly , but it's a efficial and unique way		*/

	TRACE_ME(PDEF);
	if ((pos=chk_expr(st_expr_info.operand))==FALSE)
		return FALSE;
	TRACE_ME(PDEF);
	
	move_add_to(pos);	/* move to the position of the equalvalance	*/
	
	if (def_list_add(st_expr_info.operand)==FALSE)
		return FALSE;

	remove_all_spaces();

	/*	The newest item is now pointed by def_head		*/
	if (parser(def_head)==FALSE)
		return FALSE;

	return TRUE;
}

int etn(void)
{
	/*	Inspect if it has been declared . If it is , just
		omit it and report no error .				*/

	TRACE_ME(PETN);
	
	if (dec_list_search(st_expr_info.operand)!=NULL)	/* promise redeclare	*/
	  	return TRUE;

	if (dec_list_add(st_expr_info.operand)==FALSE)
		return FALSE;

	return TRUE;
}

int prt(void)
{
	/*	First , print out the user specified information	*/
	
	int depth=0;				/* for recursion	*/
	struct def_list* dl=NULL;
	union Lists* pl=NULL;
	TRACE_ME(PPRT);
	
	if ((dl=print_information())==NULL)
		return FALSE;

	/*	Second , do the same as to construct a list structure	*/

	sys_stack_clean();
	pl=dl->entry;
	
	fprintf(stdout,"(");

	while (1)
	{
		if (pl==NULL)
		{
			pl=sys_stack_pop();
			fprintf(stdout,")");
			if (pl==NULL)	break;
			if (pl->list_node.next!=NULL)
			{
				fprintf(stdout,",");
				pl=pl->list_node.next;
			}

			else
				pl=NULL;

			continue;
		}

		if (pl->flag==0)
		{
		  	if (pl->list_node.recursive==TRUE)
				if (depth<MAX_RECURSIVE_PRINT_DEPTH)
					++depth;
				else
				{
					fprintf(stdout,"<...>");
					if (pl->list_node.next!=NULL)
					{
						pl=pl->list_node.next;
						fprintf(stdout,",");
					}
			
					else
						pl=NULL;

					continue;
				}

			sys_stack_push(pl);
			fprintf(stdout,"(");
			pl=pl->list_node.sub_next;
		}

		else
		{
			fprintf(stdout,pl->elem_node.elem_name);
			if ((pl=pl->elem_node.next)!=NULL)
				fprintf(stdout,",");
		}	
	}

	fprintf(stdout,"\n");
	fflush(stdout);

	return TRUE;
}

int had(void)
{
	/* We cannot directly call prt to manage the bottom process	*/

	int depth=0;
	struct def_list* dl=def_list_search(st_expr_info.operand);
	union Lists* pl=NULL;
	TRACE_ME(PHAD);
	
	if (dl==NULL)
	{
		err_set(E_NO_SUCH_LIST);
		err_attach(RP_LINE | RP_PROCEDURE);
		return FALSE;
	}

	pl=dl->entry;

	if (pl->flag==0)
	{
		pl=pl->list_node.sub_next;
		sys_stack_clean();
		fprintf(stdout,"(");

		while (1)
		{
			if (pl==NULL)
			{
				pl=sys_stack_pop();
				fprintf(stdout,")");
				if (pl==NULL)	break;
				if (pl->list_node.next!=NULL)
				{
					fprintf(stdout,",");
					pl=pl->list_node.next;
				}

				else
					pl=NULL;

				continue;
			}

			if (pl->flag==0)
			{
				if (pl->list_node.recursive==TRUE)
				if (depth<MAX_RECURSIVE_PRINT_DEPTH)
					++depth;
				else
				{
					fprintf(stdout,"<...>");
					if (pl->list_node.next!=NULL)
					{
						pl=pl->list_node.next;
						fprintf(stdout,",");
					}
			
					else
						pl=NULL;

					continue;
				}

				sys_stack_push(pl);
				fprintf(stdout,"(");
				pl=pl->list_node.sub_next;
			}
		
			else
			{
				fprintf(stdout,pl->elem_node.elem_name);
				if ((pl=pl->elem_node.next)!=NULL)
				fprintf(stdout,",");
			}
		}	
	}
	
	else
	{
		if (print_information()==NULL)
			return FALSE;

		fprintf(stdout,pl->elem_node.elem_name);
	}

	fprintf(stdout,"\n");
	fflush(stdout);
	
	return TRUE;
}

int tal(void)
{
	/*	It seems to be more triky than I imagine .		*/

  	int depth=0;
	struct def_list* dl=NULL;
	union  Lists* pl=NULL;
	TRACE_ME(PTAL);
	
	if ((dl=print_information())==NULL)
		return FALSE;

	sys_stack_clean();
	pl=dl->entry;

	if (pl->flag==0)
		pl=pl->list_node.next;
	else
		pl=pl->elem_node.next;

	fprintf(stdout,"(");

	while (1)
	{
		if (pl==NULL)
		{
			pl=sys_stack_pop();
			fprintf(stdout,")");
			if (pl==NULL)	break;
			if (pl->list_node.next!=NULL)
			{
				fprintf(stdout,",");
				pl=pl->list_node.next;
			}

			else
				pl=NULL;

			continue;
		}

		if (pl->flag==0)
		{
			if (pl->list_node.recursive==TRUE)
				if (depth<MAX_RECURSIVE_PRINT_DEPTH)
					++depth;
				else
				{
					fprintf(stdout,"<...>");
					if (pl->list_node.next!=NULL)
					{
						pl=pl->list_node.next;
						fprintf(stdout,",");
					}
			
					else
						pl=NULL;

					continue;
				}

			sys_stack_push(pl);
			fprintf(stdout,"(");
			pl=pl->list_node.sub_next;
		}

		else
		{
			fprintf(stdout,pl->elem_node.elem_name);
			if ((pl=pl->elem_node.next)!=NULL)
				fprintf(stdout,",");
		}	
	}

	fprintf(stdout,"\n");
	fflush(stdout);

	return TRUE;
}

int dpt(void)
{
	/*	Normally the recursive algorithm is the best , but for
		unify the form of the functions , I use recurrence instead	*/
	/*	Note that if the list is defined recursively , the depth is positive infinity	*/

	struct def_list* dl=NULL;
	union Lists* pl=NULL;
	int depth=1;
	int max_depth=1;
	TRACE_ME(PDPT);
	
	if ((dl=print_information())==NULL)
		return FALSE;
	
	pl=dl->entry;
	sys_stack_clean();

	while (1)
	{
		if (pl==NULL)
		{
			pl=sys_stack_pop();
			if (pl==NULL)	break;
			pl=pl->list_node.next;
			--depth;
			continue;
		}

		if (pl->flag==0)
		{
			if (pl->list_node.recursive==TRUE)
			{
				max_depth=INFINITY;
				break;
			}

			sys_stack_push(pl);
			pl=pl->list_node.sub_next;
			if (++depth>max_depth)
				max_depth=depth;
		}
	
		else
			pl=pl->elem_node.next;

	}

	sys_stack_clean();
	
	if (max_depth==INFINITY)
		fprintf(stdout," infinity\n");
	else
		fprintf(stdout,"%d\n",max_depth);

	return TRUE;
}

int cnt(void)
{
	struct def_list* dl=NULL;
	union Lists* pl=NULL;
	int count=0;
	TRACE_ME(PCNT);
	
	if ((dl=print_information())==NULL)
		return FALSE;

	sys_stack_clean();
	pl=dl->entry;
	
	while (1)
	{
		if (pl==NULL)
		{
			pl=sys_stack_pop();
			if (pl==NULL)	break;
			pl=pl->list_node.next;
			continue;
		}

		if (pl->flag==1)
		{
			pl=pl->elem_node.next;
			++count;
		}

		else
		{
			sys_stack_push(pl);
			pl=pl->list_node.sub_next;
		}
	}

	fprintf(stdout,"%d\n",count);
	fflush(stdout);
	
	return TRUE;
}

int inv(void)
{
	/*	This is the most difficult function , because the architecture
		of this program make the inverse operation will affect all the
		lists refer to the given list .
		For example:
			def A=(fhg,tyg);
			def B=(A,mnb);
			inv B(after manipulating:);
		will get:
			after manipulating:(mnb,(tyg,fhg))

		That's to say , I inverse the list B also .
									*/

	
	struct def_list* dl=NULL;
	union  Lists** pl=NULL;
	union  Lists* t=NULL;
	union  Lists* n=NULL;
	struct stk_node* aux_stack=__create_new_stack();
	TRACE_ME(PINV);
	
	if ((dl=print_information())==NULL)
		return FALSE;
	
	sys_stack_clean();
	pl=&dl->entry;
	t=*pl;

	/*	The basic algorithm is a bit difficult to understand , here I give a short description:
		1.Put all the items ( element and sublist) at the same level into a stack , called stack1;
		2.If a item is a sublist , then put it into the stack2 also ;
		3.Pop out all the items in stack1 , then this will be the inverse order ;
		4.Pop a item from stack2 , and go along with it to the next level then restart ;
		5.If the stack2 is empty , then it gets oveer .
	*/

	while (1)
	{
		while (t!=NULL)
		{
			sys_stack_push(t);
			if (t->flag==0)
			{
				if (t->list_node.recursive==TRUE)
				{
					t=t->list_node.next;
					continue;
				}

				aux_stack=__stack_push(aux_stack,t);
				t=t->list_node.next;
			}
			
			else
				t=t->elem_node.next;
		}

		t=sys_stack_pop();
		*pl=t;
		while (t!=NULL)
		{
			n=sys_stack_pop();
			if (t->flag==0)
				t->list_node.next=n;
			else
				t->elem_node.next=n;
			t=n;
		}

		aux_stack=__stack_pop(aux_stack);
		if (last_saved==NULL)
			break;

		pl=&(last_saved->list_node.sub_next);
		t=*pl;
	}

	aux_stack=__destroy_stack(aux_stack);
	
	/*	then print out the new list 				*/

	sys_stack_clean();
	t=dl->entry;

	fprintf(stdout,"(");

	while (1)
	{
		if (t==NULL)
		{
			t=sys_stack_pop();
			fprintf(stdout,")");
			if (t==NULL)	break;
			if (t->list_node.next!=NULL)
			{
				fprintf(stdout,",");
				t=t->list_node.next;
			}

			else
				t=NULL;

			continue;
		}

		if (t->flag==0)
		{
			sys_stack_push(t);
			fprintf(stdout,"(");
			t=t->list_node.sub_next;
		}

		else
		{
			fprintf(stdout,t->elem_node.elem_name);
			if ((t=t->elem_node.next)!=NULL)
				fprintf(stdout,",");
		}	
	}

	fprintf(stdout,"\n");
	fflush(stdout);

	return TRUE;
}

int ni(void)
{
	return TRUE;
}

/*	Tireless typing....					*/
