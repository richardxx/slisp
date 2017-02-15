/*
	The system maintained linked list .

	I give the feature later-binding for the small language , so for this 
	I must maintain two kernel lists , one is for who has been defined , and
	another is for who just only has been declared . 

	A list which has not been coped with would be marked , when a list name 
	passed to a command , I will check its comleteness , if could then make
	it available .

	When I mentioned this feature , I will comment that statement .

									*/

#ifndef	_LK_LIST

#define _LK_LIST

/*	The structure has been declared in types.h file			*/
#include "types.h"

/*
	Not like the stack and queue , the symbol here will exported .	*/

extern struct def_list* def_head;
extern struct dec_list* dec_head;


int		 	def_list_add(const char*);		//get the list name myself
void 			def_list_release();			//remove all items
struct def_list* 	def_list_search(const char*);	
void			def_grelease(union Lists*);		//release the allocated memory for the generated list

int 			dec_list_add(const char*);		//pass the list name
void 			dec_list_release();			//remove all items
struct dec_list* 	dec_list_search(const char*);		//TRUE if hits
int 			dec_list_link(struct dec_list*);	//link to the defined item

#endif
