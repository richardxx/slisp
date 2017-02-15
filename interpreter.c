/*
  	Really the core of the program .
	I have considered the expansibility of the program , but all this are
	based on source level improvements . To get more information about how
	to enhance this language , please read developer guide.
									*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "types.h"
#include "interpreter.h"
#include "internal.h"
#include "lk_list.h"
#include "linker.h"
#include "sys_ds.h"
#include "buf.h"
#include "err.h"
#include "aux.h"

int lineno=1;
int cur_dir=0;

mani_list	__std_call_table[KEY_NUM];
mani_list	__exp_call_table[EXP_NUM];
char sys_dir_reg[DIR_NUM][MAX_DIRECTIVE_LENGTH+1];
struct st_expr	st_expr_info;

/*	Initialized function vector				*/

int pp_std_cmd()
{
	__std_call_table[0]=def;
	__std_call_table[1]=etn;
	__std_call_table[2]=prt;
	__std_call_table[3]=had;
	__std_call_table[4]=tal;
	__std_call_table[5]=dpt;
	__std_call_table[6]=cnt;
	__std_call_table[7]=inv;

	return 0;
}

int pp_exp_cmd()
{
	/*	What you need, please add it here yourself		*/

	int i=0;
	for(;i<EXP_NUM;++i)
		__exp_call_table[i]=ni;

	return 0;
}

int pp_dir_nam()
{
	/*	All the directives should be lower case			*/
	
  	int i=8;

	strcpy(sys_dir_reg[DEF],"def");
	strcpy(sys_dir_reg[ETN],"etn");
	strcpy(sys_dir_reg[PRT],"prt");
	strcpy(sys_dir_reg[HAD],"had");
	strcpy(sys_dir_reg[TAL],"tal");
	strcpy(sys_dir_reg[DPT],"dpt");
	strcpy(sys_dir_reg[CNT],"cnt");
	strcpy(sys_dir_reg[INV],"inv");
	strcpy(sys_dir_reg[NI],"ni");

	/*	If you add new , you should modify the recurrence		*/

	for(;i<DIR_NUM;++i)
		*sys_dir_reg[i]='\0';

	cur_dir=KEY_NUM;			/*	change me	*/
	return 0;
}

int cmp_dir(const char* per_dir)
{
	int i=0;
	for(;i<cur_dir;++i)
		if (strcmp(per_dir,sys_dir_reg[i])==0)
			return i;

	/*	If a unexpected string , throw out exception		*/

	err_set(E_NODIRECTIVE);
	err_attach(RP_LINE);

	return FALSE;
}

int get_cmd_name(char* cmd_name)
{
	/*	This must be passed a complete unprocessed statement 	*/
  	/*	First remove the leading spaces				*/

  	int ipos=0;
	char myexpr=0;
	
	remove_all_spaces();
	if ((myexpr=buffer_getchar())==END_OF_STREAM)
	  	return END_OF_STREAM;
	
	while (ipos<=MAX_DIRECTIVE_LENGTH)
	{
		if (myexpr!=' ' && myexpr!='\t')
		  	;
		else
			break;
		
		cmd_name[ipos++]=myexpr;
		myexpr=buffer_getchar();
	}
	
	if (ipos>MAX_DIRECTIVE_LENGTH)
	{
		err_set(E_NOT_A_COMMAND);
		err_attach(RP_LINE);
		return FALSE;
	}

	MAKE_STRING(cmd_name,ipos);			/* append '\0'	*/
	
	return TRUE;
}

int get_list_name(char* wname)
{
	char t=0;
	int epos=0;

	remove_all_spaces();
	
	epos=buffer_cond_read(GET_FUNC_NAME(cond_,upper),wname);

	if (epos>MAX_LIST_NAME) 
	{
	  err_set(E_LSTNAME_LONG);
	  err_attach(RP_LINE | RP_PROCEDURE);
	  return FALSE;
	}
	
	if (match(LIST_NAME,wname)==FALSE)
	{
		err_set(E_NOT_A_LISTNAME);
		err_attach(RP_LINE | RP_PROCEDURE);
		return FALSE;
	}

	return epos;
}

int execute()
{
	if (st_expr_info.int_offset<STD_NUM)
	{
	  	if (st_expr_info.int_offset>=MAN_OFFSET)
			if (linker(st_expr_info.operand)==FALSE)
				return FALSE;

	  	if(__std_call_table[st_expr_info.int_offset]()==FALSE)
			return FALSE;
	}

	else
		if(__exp_call_table[st_expr_info.int_offset-EXP_OFFSET]()==FALSE)
			return FALSE;

	return TRUE;
}

int preprocessor()
{
	static char cmd_name[MAX_LIST_NAME+1];
	int index=0;
	int aux_rt=0;
	char t=0;
	int i=0;
	int target_pos=0;

	if ((aux_rt=get_cmd_name(cmd_name))==FALSE)
	{
		err_set(E_WRONG_CMDNAME);
		err_attach(RP_LINE | RP_PROCEDURE);
		return FALSE;
	}
	
	else if (aux_rt==END_OF_STREAM)
	{
		st_expr_info.int_offset=8;
		return TRUE;
	}

	if ((index=cmp_dir(cmd_name))!=FALSE)
	  	/*	Load command interpreter address information	*/
		st_expr_info.int_offset=index;

	else
	{
		err_set(E_NOT_A_COMMAND);
		err_attach(RP_LINE | RP_PROCEDURE);
		return FALSE;
	}

	if ((i=get_list_name(st_expr_info.operand))==FALSE)
		return FALSE;
	
	MAKE_STRING(st_expr_info.operand,i);
	MAKE_STRING(st_expr_info.arg,0);

	if (index>=MAN_OFFSET)
	{
		if (target_pos=buffer_searchc('(',0)!=FALSE)
		{
			move_add_to(target_pos);
			t=buffer_getchar();
			i=0;

			while (t!=')')
			{
				st_expr_info.arg[i++]=t;
				t=buffer_getchar();
			}
			
			MAKE_STRING(st_expr_info.arg,i);
		}
	}

	st_expr_info.int_offset=index;

	return TRUE;
}

void interpreter()
{
	/*	It's just a wrap routine , I sequtially call the functions
		one by one . If get error , then quitely exit .			*/
	
	/*	Prepare for the execution environment				*/

	pp_std_cmd();
	pp_exp_cmd();
	pp_dir_nam();
	
	init_buffer();
	load_data();				/* load data from file		*/
	input_buffer(0);
	
	while (has_no_data()==FALSE)
	{

		TRACE_ME(PREPROCESSOR);
		if (preprocessor()==FALSE)
			goto err_poll;

		if (execute()==FALSE)
			goto err_poll;
		
		buffer_skip_sentence();

		continue;

	err_poll:
		err_flush();
		buffer_skip_sentence();
	}

	return ;						/* no need to report	*/

}

/*	End of file ^&%$##@^&*((^%#$$%&*%$#@@$%^*(I^^$@@		*/
