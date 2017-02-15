/*
	Just a easy job , so I don't explain more here .

	The first edition of this program don't have enough parameters to 
	control the behaviour , so I just declare them here .

									*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "opt.h"
#include "lk_list.h"
#include "sys_ds.h"
#include "err.h"

FILE* src_descriptor=NULL;
char my_src_file[128];
char prefix[4]="lst";
char* arg_vector[ARGNUM+1];
int arguments=0;

static void fill_arguments()
{
	arg_vector[1]="help";
	arg_vector[2]="version";
	arg_vector[3]="hide";
	my_src_file[0]='\0';		/* indicate no file		*/
	
}

/*	Really very augly routine					*/
int parse_parameter(int argc,char** argv)
{
  	int index=1;
	int i=1;

	if (argc==1)
	{
		fprintf(stderr,"No input file . Abort. \n");
		return FALSE;
	}
	
	fill_arguments();
	
	while (index<argc)
	{
		if (argv[index][0]=='-')
		{
			for (;i<=ARGNUM;++i)
				if (strcmp(argv[index]+1,arg_vector[i])==0)
				{
					arguments|=(2<<(i-1));
					break;
				}

			if (i>ARGNUM)
			{
				fprintf(stderr,"Invalid arguments , please try -help to get more details.\n");
				return FALSE;
			}

			break;
		}
		
		else
		{
			strcpy(my_src_file,argv[index]);
			break;
		}
	}

	return TRUE;
}

int chk_open_file()
{
	
	int len=strlen(my_src_file);
	int i=len-2;

  	if (*my_src_file=='\0')
	{
		fprintf(stderr,"No input file . Abort .\n");
		return FALSE;
	}

	for (;i<=len;++i)
		if (my_src_file[i-1]!=prefix[i-len+2])
		{
			fprintf(stderr,"This is not a list processing file.\n");
			return FALSE;
		}

	src_descriptor=fopen(my_src_file,"r");

	if (src_descriptor==NULL)
	{
		perror("\n");
		return FALSE;
	}

	return TRUE;
}

int print_usage()
{
	printf("\n\n");
	printf("Usage :  slisp [options] [filename]\n");
	printf("         --help 	Show this message\n");
	printf("         --version      Show version information\n");
	printf("\nFor bug report , email me at : frogxx@gmail.com\n\n");

	return 0;
}

int print_version()
{
	printf("\nReading specs from the resource file : \n\n");
	printf("The version is :                 %s\n",VERSION);
	printf("Development platform :           %s\n",PLATFORM);
	printf("Language:                        %s\n",LANG);
	printf("Compiler:			 %s\n",NGCC);
	printf("Compiler version:		 %f\n",VGCC);
	printf("The author is:			 %s\n\n",AUTHOR);

	return 0;
}

int print_hide()
{
	/* Information about this program something about author	*/
  	
	printf("\n\n\n");
	printf("\t\tIntroduction to this program and the author\n");
	printf("1.About Program:\n");
	printf("\tNormally , this was just a so easy homework of the data structure \n");
	printf("course , but I don't think I only have ability to archive a small program , \n");
	printf("so I modify the needs , and expand it into a small language . I know I l\n");
	printf("don't have enough time and knowleage to complete a full functionality compiler \n");
	printf(", so I just want to make a easy-to-use interpreter .\n");
	printf("\tAfter starting the work , I have just done nothing but coding . This \n");
	printf("didn't seem so difficult to me , but I don't have enough time also . I just \n");
	printf("planed to have 5 days to work out , and at last I archive my aim .\n");
	printf("\tI know I have many things to improve , but really I don't have enough \n");
	printf("time , so sorry I delay a long time to release this program .\n");
	printf("\n\n");
	printf("2.About the author:\n");
	printf("\tReally a computer techical fans , and want to be a real hacker (not the \n");
	printf("cracker today , but the hacker in 60-70 era) . I like operating system and \n");
	printf("compiler design very much , hope anyone who like it please contact me ...\n");
	printf("\n\n");

	return 0;
}

int tell_argument()
{
	if (arguments!=0)
	{
		if (arguments==HELP)
			print_usage();
		
		else if (arguments==VERSION)
			print_version();

		else if (arguments==HIDE)
			print_hide();

		return FALSE;			/* outwit the main proc	*/
		
	}

	else
		if (chk_open_file()==FALSE)
			return FALSE;

	return TRUE;
}

int exit_execution()
{
	/* The main task is to release all used memory 			*/

	def_list_release();
	dec_list_release();
	sys_stack_clean();
	
	return TRUE;
}
