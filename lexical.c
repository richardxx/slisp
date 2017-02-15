/*
	Nothing but a very simple lexicon analyzer .
	The whole expression shuld be preprocessed first , and then check
	error . After that , copy them into the high speed buffer , and pass 
	the control to this function .

									*/

#include <ctype.h>
#include "types.h"
#include "lexical.h"
#include "sys_ds.h"
#include "buf.h"
#include "err.h"

char rt_token[MAX_TOKEN_NAME+1];

/*	I will read the character one by one until a semicolon appeared .
	During the process , I will remove all the spaces and newline ,
	and combine them into one sentence .
									*/
/*
   	Input :	statement="(dfdf,gf,A,(mmn,df));"			*/

int lexical()
{

	int i=0;
	char t=0;

	while (1)
	{
		t=buffer_getchar();

		if(t==' ' || t=='\t' || t=='\n')
			;
		
		else if(t=='(' || t==')' || t==',' || t==';')
		{
			rt_token[0]=t;
			rt_token[1]='\0';
			break;
		}
	
		/*	It is a list name				*/

		else if(isupper(t)!=0)
		{
			rt_token[0]=t;
			t=buffer_getchar();
			i=1;
			while (isupper(t)!=0)
			{
				if (i==MAX_LIST_NAME)
				{
					err_set(E_LSTNAME_LONG);
					err_attach(RP_LINE);
					return FALSE;
				}

				rt_token[i++]=t;
				t=buffer_getchar();
			}
		
			/*	Read one more character			*/
			/*	At the end of the name , there must
				exist a comma or a parenthesis , or it 
				gets error . 
				For example:
					ABDrt is not a illegal name	*/

			if (t!=',' && t!=')')
			{
				err_set(E_MISS_SEPARATOR);
				err_attach(RP_LINE);
				return FALSE;
			}

			buffer_ungetchar();
			MAKE_STRING(rt_token,i);
			break;
	
		}
		
		/*	It is a element	name				*/
		else if (islower(t)!=0)
		{
			rt_token[0]=t;
			t=buffer_getchar();
			i=1;
			while (islower(t)!=0)
			{
				if (i==MAX_ELEM_NAME)
				{
					err_set(E_ELMNAME_LONG);
					err_attach(RP_LINE);
					return FALSE;
				}

				rt_token[i++]=t;
				t=buffer_getchar();
			}
		
			if (t!=',' && t!=')')
			{
				err_set(E_MISS_SEPARATOR);
				err_attach(RP_LINE);
				return FALSE;
			}
			
			buffer_ungetchar();
			MAKE_STRING(rt_token,i);
			break;

		}

		/*	Unexpected characters				*/
		else if (isdigit(t)!=0)
		{
			err_set(E_NODIGIT);
			err_attach(RP_LINE);
			return FALSE;
		}
		
		else if (isascii(t)!=0 || iscntrl(t)!=0)
		{
			err_set(E_WRONG_SYMBOL);
			err_attach(RP_LINE);
			return FALSE;
		}

		else
		{
			err_set(E_UNCAUGHT_SYMBL);
			err_attach(RP_LINE);
			return FALSE;
		}
	
	}

	return TRUE;
}

char* sys_token_get()
{
  	if (lexical()==FALSE)
		return NULL;

	return rt_token;
}

void sys_token_clean()
{
	buffer_ungetchar();
}
