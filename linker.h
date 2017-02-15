/*
   	I create a new file to specially handle the link process . 
	In the last version , I use a very simple method to constructor the linker ( sure not the real linker ) , but 
	in the testing section , I find that I should improve it because most of the time I don't get the right answer .
	
	The thoughts behind the code is very simple to understand , because this is a complete dynamic language , so 
	I don't want a complex design .
														*/

#ifndef _LINKER_H

#define _LINKER_H

int linker(const char*);					/* the external interface			*/

int dynamic_binding(const char*);				/* to bind one list to another			*/

#endif

