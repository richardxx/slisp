/*
	The implementation of the two core structures .
	It's very simple , so I will not emphsize the process .
									*/

/*
	I don't want to export them , they are just self-contained .	*/

#include <stdlib.h>
#include <string.h>

#include "sys_ds.h"

#ifdef	DEBUG
	#include <stdio.h>
#endif



/*	The are all predefined by system				*/

struct stk_node* stk_head=NULL;
struct que_node* que_head=NULL;
struct que_node* que_tail=NULL;
union  Lists* last_saved=NULL;


/*=================================================================
		IMPLEMENTATION OF General STACK
=================================================================*/

struct stk_node*  __destroy_stack(struct stk_node* _sth)
{
	struct stk_node* t=NULL;
	while (_sth!=NULL)
	{
		t=_sth->next;
		DELETE(_sth);
		_sth=t;
	}

	return _sth;
}

struct stk_node*  __stack_push(struct stk_node* _sth,union Lists* to_be_saved)
{
  	struct stk_node* t=NULL;

	if (_sth==NULL)
	{
		_sth=NEW(struct stk_node);
		INIT2(_sth,saved,to_be_saved,next,NULL);
		return _sth;
	}

	t=NEW(struct stk_node);
	INIT2(t,saved,to_be_saved,next,_sth);
	_sth=t;

	return _sth;
}

struct stk_node*  __stack_pop(struct stk_node* _sth)
{
	struct stk_node* t=NULL;

	if (_sth==NULL)
	{
		last_saved=NULL;
		return NULL;
	}

	t=_sth->next;
	last_saved=_sth->saved;
	DELETE(_sth);
	_sth=t;
	return _sth;
}

int __stack_empty(struct stk_node* _sth)
{
	if (_sth==NULL)
		return TRUE;
	return FALSE;
}

//Specially treated method

union Lists* sys_stack_pop()
{
	stk_head=__stack_pop(stk_head);
	return last_saved;
}



