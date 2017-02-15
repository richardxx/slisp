/*
	The implementaion of the auxiliary functions .

	Once upon there are full of utilities , but since the seconde version of this program ,
	I combine most of them into the buffer engine , so the speed is highly improved .
*/
#include <string.h>
#include "types.h"
#include "interpreter.h"
#include "lk_list.h"
#include "buf.h"
#include "aux.h"
#include "err.h"

/*
	It seems so easy . It used to justify if the two arguments match each 
	other . I input many unreached point in the code , so may it looks 
	very ugly.							*/


int match(int flag,const char* expr)
{
	
	const char* myexpr=expr;		/*   reserve the original one  */

	switch (flag)	{
	
	case LEFT_BRACE:
	
		if (*myexpr=='(')	
			return TRUE;

		return FALSE;

		break;
	
	case RIGHT_BRACE:
		
		if (*myexpr==')')
			return TRUE;

		return FALSE;

		break;

	case COMMA:
		
		if (*myexpr==',')
			return TRUE;

		return FALSE;

		break;
		
	case SEMICOLON:
	  
		if (*myexpr==';')
			return TRUE;

		return FALSE;

		break;

	case LIST_NAME:

		while (*myexpr!='\0')
		{
			if (*myexpr>=65 && *myexpr<=90)
			{
				++myexpr;
				continue;
			}

			return FALSE;
		}

		return TRUE;

		break;

	case ELEM_NAME:

		while (*myexpr!='\0')
		{
			if (*myexpr>=97 && *myexpr<=122)
			{
				++myexpr;
				continue;
			}

			return FALSE;
		}

		return TRUE;

		break;

	default:
		
		break;

	}

	return FALSE;
}


/*	Once upon I have deleted this function :) , then I find it useful		*/

int exist(const char* ename)
{
	if (def_list_search(ename)!=NULL)
		return TRUE;

	if (dec_list_search(ename)!=NULL)
		return TRUE;

	return FALSE;
}

int chk_braces_match()
{
	int l_brace=0;
	int r_brace=0;
	int i=-1;
	char expr=buffer_getchar();

	while (1)
	{
		if (expr=='(')
		{
			++l_brace;
			continue;
		}

		if (expr==')')
		{	
			++r_brace;
			continue;
		}

		if (r_brace>=l_brace)
			break;
		
		expr=buffer_getchar();
	}
		
	if (r_brace!=l_brace)
	{
		err_set(E_WRONG_BRACE);
		err_attach(RP_LINE);
		return FALSE;
	}

	return TRUE;
}
	

/*	There are two type of things to be checked :
	1.if it has enough braces and a equal;
	2.if it is a re-difinition ;
	Return:
		The the position of =
									*/
int chk_expr(const char* lst_name)
{
	/*
		The expr must be this form (with leading spaces removed):
			expr:"A=(df,dfg,er,Y);"
		otherwise it will return error .
		The most confrused thing is that , the expression is like:
			A=)df,dfd,)yh,ui((
		Silly , isn't it ?

									*/

	int rt_pos=0;
	int i=0;
	char expr=0;
	int ipos=buffer_searchc_until('=',0,';');
	TRACE_ME(EXPRCHECK);
	
	if (ipos==FALSE)
	{
		err_set(E_MISS_EQUAL);
		err_attach(RP_LINE | RP_PROCEDURE);
		return FALSE;
	}
	
	move_add_to(ipos);
	rt_pos=get_pointer();

	if (chk_braces_match()==FALSE)
		return FALSE;

	move_to(rt_pos);

	return ipos+1;
}

/*
	Return: The position of the character k(start at 0)		*/

int instrc(const char* str,const char k,int offset)
{
	/*
		I will not check the offset , so the calling function must
		promise it's less than the length of str .	
									*/

	int i=offset;

	while (str[i]!='\0')
		if (str[i++]==k)	
		  	return i-1;

	return FALSE;
}

/*	Brute-force algorithm to find a pattern matching		*/

int instr(const char* str,const char* substr,int offset)
{
	int i=offset;
	const char* s=str;

	while (s[i]!='\0')
	{
		if (s[i++]==*substr)
		{	
			int j=i-1;
			int k=1;
			while (s[i]!='\0' && substr[k]!='\0')
			{
				if (s[i++]==substr[k++])	
					continue;
				break;
			}

			if (substr[k]=='\0')	return j;
			
			else	i=j+1;
		}
	}

	return FALSE;
}

void get_random_list_name(char* rt_lst_name)
{
	/*	To make this procedure run more quickly , I have to reduce the recurrence times */

	const char base='A';
	const char raw[]="GHDJLETUBVKVLCLCDFLSDFSFDSDTZPXX";
	const int interval=7;				// magic number
	const int upbound=2;				// magic number
	static int seq_pik=-3;				        // magic number
	static int i = 0;
	
	strcpy(rt_lst_name,raw);

	for(i=0;i<=upbound;++i)
	{
		if ((seq_pik+=interval)>=26)
			seq_pik-=26;
		
		rt_lst_name[seq_pik]=seq_pik+base;
	}
}

void remove_all_spaces()
{

	char expr=buffer_getchar();

	while (1)
	{
		if (expr==' ' || expr=='\t')
			expr=buffer_getchar();
		else
			break;
	}
	
	buffer_ungetchar();
}

int cond_upper(char update)
{
	if (update>=65 && update<=90)
		return TRUE;

	return FALSE;
}

int cond_lower(char update)
{
	if (update>=97 && update<=122)
		return TRUE;

	return FALSE;
}

int cond_separator (char update)
{
	if (update==',' || update==';' || update=='=')
		return TRUE;

	return FALSE;
}
