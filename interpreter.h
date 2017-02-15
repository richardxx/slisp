/*
	This is the source analyzer tool . Its main task is read the file , then
	try to gather the operator and the operands , if necessary , pass it to
	the parser and build its data structure , or otherwise directly execute
	the instruction .

	If execute , all the error will be dected , and the runtime bliding will
	get over , if the program is right , then you will get right answer .

									*/

#ifndef _INTERPRETER_H

#define _INTERPRETER_H


/*
	In this small language , I only define two types of instruction --
	definition directive and manipulation directive . Then to translate 
	the ascii into executable code , I should distinguite them .
									*/

#define DEFINITION 		0
#define MANIPULATION  		1
#define EXPANDED		2

/*	KEYWORDS(OR CALLED STANDARD OPERATION)				*/

#define STD_NUM			8
#define KEY_NUM			STD_NUM			/* legacy	*/
#define EXP_NUM			32
#define DIR_NUM			40
#define MAX_DIRECTIVE_LENGTH	5

#define DEF_OFFSET		0
#define MAN_OFFSET		2
#define EXP_OFFSET		STD_NUM

#define DEF			0	/*	define		*/
#define ETN			1	/*	declare		*/
#define PRT			2	/*	print		*/
#define HAD			3	/*	get head	*/
#define TAL			4	/*	get tail	*/
#define DPT			5	/*	get depth	*/
#define CNT			6	/*	count element	*/
#define INV			7	/*	inverse		*/
#define NI			8	/*	not impl.	*/

/*	Typedefs and external variables					*/

typedef int 	(*mani_list)(void);	/*	directly use st_expr	*/

extern int 	lineno;			/*	line number	*/
extern int 	cur_dir;		/*	loaded directive*/
extern char*	rd_buff;		/*	A noraml sentences readed in */
extern mani_list	__std_call_table[KEY_NUM];
extern mani_list	__exp_call_table[EXP_NUM];
extern char sys_dir_reg[DIR_NUM][MAX_DIRECTIVE_LENGTH+1];	/*	Identifer table		*/

/*	
   	The work style has been changed since the last version of this program .
	I have modify the code to get a more flexible architecture .
	I will discuss this in the developer guide						*/


/*	Data structures							*/
/*	The preprocessor will fill the structure			*/

struct st_expr
{
	int int_offset;				/* the index in the interupt table	*/
	char operand[MAX_LIST_NAME+1];		/* the object list name			*/
	char arg[MAX_ARG_LENGTH+1];		/* user typed information		*/
};

extern struct st_expr	st_expr_info;

/*	Functions and interfaces	                 					*/


void 	interpreter();
int 	preprocessor();
int 	execute();

int 	pp_std_cmd();			/*	prepare standard	*/
int 	pp_exp_cmd();			/*	prepare expanded	*/
int 	pp_dir_nam();			/*	prepare directive	*/

int 	cmp_dir(const char*);		/*	compare directive	*/
int 	syntax_parse();			/*	not the parser		*/
int	get_cmd_name(char*);		/*	split out the command	*/
int     get_list_name(char*);		/*	get the list name	*/

#endif
