/*
 	The linker will rewrite in future versions , but right now I just use a simple strategy :

		1.Go throgh the linked list , and find a reference to other list , then push link it
		like the old one does , but also I push it in a stack when the refered lisk has link
		to another one ;

		2.Link the left ones in the stack ;

		3.When a refered list has not been defined , I don't quit this time , just omit it 
		and use a empty list instead .

												*/

#include "types.h"
#include "linker.h"
#include "lk_list.h"
#include "sys_ds.h"
#include "err.h"
#include "trace.h"

static struct stk_node* aux_stack=__create_new_stack();

int linker(const char* lst_name)
{
	union Lists* plist=NULL;

	/* Start to link all lists recursively				*/

	if (dynamic_binding(lst_name)==FALSE)
		return FALSE;

	while (__stack_empty(aux_stack)==FALSE)
	{
		aux_stack=__stack_pop(aux_stack);
		plist=last_saved;
		
		if (dynamic_binding(plist->list_node.list_name)==FALSE)
		{
			aux_stack=__destroy_stack(aux_stack);
			return FALSE;
		}
	}
	
	aux_stack=__destroy_stack(aux_stack);

	return TRUE;
}

int dynamic_binding(const char* lname)
{
	
	union Lists* pl=NULL;
	struct dec_list* decl=NULL;
	struct def_list* defl=NULL;
	char* tname=NULL;
	struct def_list* dl=def_list_search(lname);
	TRACE_ME(DYNAMIC_BINDING);
	
	if (dl==NULL)
	{
		err_set(E_RUNTIME_LIST_NOT_EXIST);
		err_attach(RP_LINE | RP_PROCEDURE);
		return FALSE;
	}

	if (dl->has_no_export_symbol==TRUE)
		return TRUE;

	if ((pl=dl->entry)==NULL)
		return FALSE;

	sys_stack_clean();
	
	/*	Tranverse the list , and link all the items of the list	*/

	while (1)
	{
	  	if (pl==NULL)
		{
			pl=sys_stack_pop();
			if (pl==NULL)	break;
			pl=pl->list_node.next;
			continue;
		}

		if (pl->flag==0)
		{
			/* found a external linkage,my job is to find its location and link it 	*/
			
		  	if (pl->list_node.local_variable==1)
			{
				tname=pl->list_node.list_name;
				if (pl->list_node.recursive==TRUE)	
				{
					pl->list_node.sub_next=dl->entry;
					pl=pl->list_node.next;
					continue;
				}

				if((decl=dec_list_search(tname))!=NULL)
				{
					if (decl->has_defined==TRUE)
						;
					else
						if (dec_list_link(decl)==FALSE)
						{
							err_attach(RP_LINE | RP_PROCEDURE);
							return FALSE;
						}

					pl->list_node.sub_next=decl->entry;
				}

				else if ((defl=def_list_search(tname))!=NULL)
					pl->list_node.sub_next=defl->entry;

				/* Maybe the external list hasn't been linked			*/

				aux_stack=__stack_push(aux_stack,pl);	
				pl=pl->list_node.next;

				/* I have checked the existance of list	, so it must be somewhere   */
			}
		
			else
			{
				sys_stack_push(pl);
				pl=pl->list_node.sub_next;
			}

		}

		else
			pl=pl->elem_node.next;

	}

	dl->has_no_export_symbol=TRUE;

	return TRUE;
}				
