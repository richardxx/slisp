/*
	The implemetation of kernel lists .
	I have introduced them yet , so will not be again here .
									*/

#include <stdlib.h>
#include <string.h>
#include "lk_list.h"
#include "sys_ds.h"
#include "aux.h"
#include "err.h"

struct def_list* def_head=NULL;
struct dec_list* dec_head=NULL;

/*=================================================================
		IMPLEMENTAION OF DEFINED LIST
=================================================================*/

int def_list_add(const char* lname)
{
	/*
		expr:the name of the list .
		I will keep the original texts and modify it later .
	*/

	
	if (match(LIST_NAME,lname)==FALSE)
	{
		err_set(E_ILLEGAL_LIST_NAME);
		err_attach(RP_LINE);
		return FALSE;
	}

	if (def_head==NULL)
	{
		def_head=NEW(struct def_list);
		INIT3(def_head,has_no_export_symbol,FALSE,entry,NULL,next,NULL);

	}

	else
	{
		struct def_list* t=NEW(struct def_list);
		INIT3(t,has_no_export_symbol,FALSE,entry,NULL,next,def_head);
		def_head=t;
	}

	strcpy(def_head->list_name,lname);
	
	return TRUE;
}

void def_list_release()
{
	/* 	Recursive deleting all the list items			*/

	struct def_list* t=NULL;
	union Lists* pl=NULL;
	union Lists* nl=NULL;
	sys_stack_clean();

	while (def_head!=NULL)
	{
		t=def_head->next;
		pl=def_head->entry;
		
		def_grelease(pl);	/*	memory in dark area	*/

		DELETE(def_head);
		def_head=t;
	}
}

/*	Release the memory occupied by the lists' items					*/

void def_grelease(union Lists* pl)
{
	union Lists* nstore=NULL;

	while (1)
	{
		if (pl==NULL)
		{
			pl=sys_stack_pop();
			if (pl==NULL)	break;
			nstore=pl->list_node.next;
			DELETE(pl);
			pl=nstore;
			continue;
		}

		if (pl->flag==0)
		{
			/*	When a list reference another list ,
				don't touch them			*/

			if (pl->list_node.local_variable==1)
			{
				pl=pl->list_node.next;
				continue;
			}

			sys_stack_push(pl);
			pl=pl->list_node.sub_next;
		}

		else
		{
			nstore=pl->elem_node.next;
			DELETE(pl);
			pl=nstore;
		}
	}
}


struct def_list* def_list_search(const char* sname)
{
	struct def_list* t=def_head;
	
	if (*sname=='\0')
	  return NULL;
	
	while (t!=NULL)
	{
		if (strcmp(t->list_name,sname)==TRUE)
			return t;
		t=t->next;
	}

	return NULL;
}

/*==================================================================
		IMPLEMENTATION OF DECLARED LIST
==================================================================*/
int dec_list_add(const char* expr)
{
	/*
		pass the argument of the dec directive directly to me .
		for example,
			dec A;
		then expr will be A , and before all I should promise the length
		isn't too long .
	*/

	if (match(LIST_NAME,expr)==FALSE)
	{
		err_set(E_ILLEGAL_LIST_NAME);
		return FALSE;
	}

	if (dec_head==NULL)
	{
		dec_head=NEW(struct dec_list);
		INIT3(dec_head,has_defined,FALSE,entry,NULL,next,NULL);
	}
	
	else
	{
		struct dec_list* t=NEW(struct dec_list);
		INIT3(t,has_defined,FALSE,entry,NULL,next,dec_head);
		dec_head=t;
	}

	strcpy(dec_head->list_name,expr);

	return TRUE;
}

void dec_list_release()
{
	struct dec_list* t=NULL;
	while (dec_head!=NULL)
	{
		t=dec_head->next;
		DELETE(dec_head);
		dec_head=t;
	}
}

struct dec_list* dec_list_search(const char* sname)
{
	struct dec_list* t=dec_head;
	
	if (*sname=='\0')
	  return NULL;
	
	while (t!=NULL)
	{
		if (strcmp(t->list_name,sname)==TRUE)
			return t;
		t=t->next;
	}

	return NULL;
}

/*
	The most important part of this file .
	The following function link a symbol to the real memory space , then
	if once done , later we can use this list .
*/

int dec_list_link(struct dec_list* decl)
{
	struct def_list* defl=def_list_search(decl->list_name);
	
	if (defl==NULL)
	{
		err_set(E_NO_SUCH_LIST);
		err_attach(RP_LINE);
		return FALSE;
	}

	else
	{		
		decl->entry=defl->entry;
		decl->has_defined=TRUE;

	}
	
	return TRUE;
}

/*
		END . . . % ^ 3 & * @ ! ~ | / \ ' 			*/
