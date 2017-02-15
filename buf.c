/*
	This component plays a very important part in the program . 
	If I can make it work a bit faster , maybe I will make the whole
	interpreter be more powerful , so after I design the prototype of
	the interpreter , the first thing was to improve these functions .

									*/

/*
	A figure for my scheme:
		{-------------------------------}	
		{	   data pond		}
		{-------------------------------}
			      | |
			      | |    load_data()
			      \_/
				
		0-buffer_size-1 buffer_size+1-2buffer_size
		+-------+-----+-+-------+-----+-+
		|	      |0|	      |0|
		+-------+-----+-+-------+-----+-+
			^		^
			|		|
		      buffer1	     buffer2
									*/
		
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "buf.h"
#include "err.h"

static char data_pond[DATA_POND_SIZE+1];	/* the data center			*/
static char buffer[BUFFER_SIZE<<1+2];		/* add two more senitel			*/

static int offset_data_pond=0;		/* how many data have been accessed		*/
static int data_length=0;		/* how many data stored in the pond		*/
static int forward=0;			/* forward reading pointer			*/
static int current_used_buffer=0;	/* as the name					*/

static int lock=0;			/* indicate if the buffer can be refreshed	*/
static int end_of_file=FALSE;

extern FILE* src_descriptor;		/* from the opt.h file				*/

void init_buffer()
{
	int end=0;

	memset(buffer,'\0',(BUFFER_SIZE<<1+2));		/* initialize the container	*/
	end=BUFFER_ENDPOS(0)+1;
	buffer[end]=EOS;
	end=BUFFER_ENDPOS(1)+1;
	buffer[end]=EOS;
}

int load_data()
{
	data_length=fread(data_pond,sizeof(char),DATA_POND_SIZE,src_descriptor);
	if (data_length==0)
	{
		fclose(src_descriptor);
		return FALSE;
	}

	MAKE_STRING(data_pond,data_length);
	offset_data_pond=0;
}

int input_buffer(int which_buffer)	
{
  /* I don't justify the property of the buffer you gave                   */
  
	int buffer_start=BUFFER_STAPOS(which_buffer);
	int buffer_end=BUFFER_ENDPOS(which_buffer);
	int is_leading_spaces=FALSE;
	
	if (data_length==0)
		return FALSE;

	while (1)
	{
		if (offset_data_pond==data_length)
		{
			if (load_data()==FALSE)
			{
				offset_data_pond=data_length;
				break;
			}
		}
	
		/* A simple preprocessor included							*/
		/* Preserve all newline control character but remove all leading spaces			*/

		while(buffer_start<=buffer_end)
		{
			if (data_pond[offset_data_pond]==END_OF_LINE)
			{
				buffer_start=buffer_end;
				break;
			}

			if (data_pond[offset_data_pond]=='#')			/* remove comments	*/	
			{
				while (data_pond[++offset_data_pond]!='\n' && offset_data_pond<data_length);
				is_leading_spaces=TRUE;
				continue;
			}

			if (is_leading_spaces==TRUE &&
				(data_pond[offset_data_pond]==' ' ||		/* remove spaces	*/
				data_pond[offset_data_pond]=='\t'))
			{	
				while (data_pond[++offset_data_pond]==' ' && offset_data_pond<data_length);
				while (data_pond[offset_data_pond++]=='\t' && offset_data_pond<data_length);
				--offset_data_pond;
			}
		
			if (offset_data_pond==data_length)
				break;
			
			while((buffer[buffer_start++]=data_pond[offset_data_pond++])!='#' &&
				offset_data_pond<data_length && buffer_start<=buffer_end);

			if (--buffer_start==buffer_end)
				break;
			if (offset_data_pond==data_length)
				break;
			else
				--offset_data_pond;
			
			is_leading_spaces=FALSE;

								/* read one more characters	*/
		}

		if (buffer_start<buffer_end)
			;
		else
			break;

	}

	forward=BUFFER_STAPOS(which_buffer);
	return TRUE;
}
	
char buffer_getchar()
{
	int n=BUFFER_ENDPOS(current_used_buffer);
	static int next_buffer=0;
	
	/* Maybe there was a section of comments , we must consider this situation	*/
	/* This time I don't remove any spaces , I can't promise to be right		*/

	while (1)
	{
		if (buffer[forward]=='#')
			while (buffer[++forward]!='\n' && forward<=n);

		if (buffer[forward]=='\n')
		{
		  	++lineno;
			while (buffer[++forward]=='\n')	
				++lineno;
		}

		if (buffer[forward]==EOS)
		{
		  	next_buffer=(current_used_buffer+1) & 1;

			if (is_boundary(current_used_buffer)==TRUE)
				return END_OF_STREAM;

			if (is_read_only(next_buffer)==TRUE)
				;
			else
				if (input_buffer(next_buffer)==FALSE)
					return FALSE;

			sweep_swlock(current_used_buffer);
			n=BUFFER_ENDPOS(next_buffer);
			forward=BUFFER_STAPOS(next_buffer);
			current_used_buffer=next_buffer;
			continue;
		}
		
		else if (buffer[forward]==END_OF_FILE)
		{
			end_of_file=TRUE;
			return END_OF_STREAM;
		}

		break;
	}

	return buffer[forward++]; 
}

void buffer_ungetchar()
{
	/* To unget a char is not so easy as you imagine .
		Self swapping lock is devised for this .				*/

	int mylock=0;

	if (is_boundary(current_used_buffer)==TRUE)
	{
		if (forward==(BUFFER_STAPOS(current_used_buffer)))
			return;
		else
			;
	}

	else
	{
		GET_LOCK(SW,current_used_buffer);
		lock &=mylock;

		if (forward==0)
		{
			forward=BUFFER_ENDPOS(1);
			return;
		}
	
		else if (forward==(BUFFER_STAPOS(1)))
		{
			forward=BUFFER_ENDPOS(0);
			return;
		}

	}

	--forward;
}

int buffer_get_interval(int start_offset,int read_offset,char* store)
{
  int start_pos=(forward+start_offset) & (TOTAL_SIZE-1);
  int current_pos=forward;
  int next_buffer=(current_used_buffer+1)&1;
  int i=0;
  int skip_gap=FALSE;
  
  /* lock current buffer and read forward ( omit comments )    */
  lock_buffer(SW,current_used_buffer);
  if (start_pos>(BUFFER_ENDPOS(current_used_buffer))) {
    
    if (is_read_only(next_buffer)==TRUE)
      return FALSE;
    input_buffer(next_buffer);
  }
  
  forward=start_pos;
  
  while (i<read_offset) {
    
    if (buffer[forward]=='#') 
      while (buffer[++forward]!='\n' && buffer[forward]!=END_OF_FILE);
    
    if (buffer[forward]==END_OF_FILE)
      break;
    
    if (buffer[forward]==EOS) {
      if (skip_gap==FALSE) {
	if (is_read_only(next_buffer)==TRUE)
	  break;
	
	input_buffer(next_buffer);
	forward=BUFFER_STAPOS(next_buffer);
      }
      
      else
	break;
    }
    
    store[i]=buffer[forward];
    ++i;
    ++forward;
  }
  
  forward=current_pos;             /* restore the original values and make it locked */
  lock_buffer(SW,next_buffer);
  
  return i+1;
}

int buffer_cond_read(read_cmp my_cond,char* my_store)
{
	int i=FALSE;
  	char t=buffer_getchar();

	while (my_cond(t)==TRUE)
	{
		my_store[++i]=t;
		t=buffer_getchar();
	}
	
	buffer_ungetchar();
	
	return i+1;
}

void buffer_flush(int which_buffer)
{
	/*
		This action will clean up the buffer , caution ! 	*/
	
	if (is_read_only(current_used_buffer)==TRUE)
		return;

	forward=BUFFER_STAPOS(which_buffer);
	MAKE_STRING(buffer,forward);
	
}


int buffer_getline(char* usr_buf)
{
	/*	Do this job , I must solve three problems .
		First is that the current pointed character is a '\n' symbol , so I must skip all of that .
		Second , I also shuld skip all the leading spaces .
		Third , when be at the end of file , but there's not a '\n' symbol , I should add it	
													*/
  	int cup=0;				/* current pos	*/
	char t=0;

	t=buffer_getchar();
	while (t!=END_OF_FILE && t!=';')	/* a wrapper of getchar , so it's safe		*/
	{
		usr_buf[cup++]=t;
		t=buffer_getchar();
	}

	buffer_ungetchar();
	MAKE_STRING(usr_buf,cup);

	return cup;

}

/*	To call getchar to implement is too slow , I want it to run faster			*/

void  buffer_skip_sentence()
{
	char t=0;
	int next_buffer=0;
	int total=0;				/* at most I skip MAX_EXPR_LENGTH characters	*/

  	while (total<=MAX_EXPR_LENGTH)
	{
	  	t=buffer[forward];

		if (t==EOS)
		{
			next_buffer=(current_used_buffer+1)&1;
			if (is_boundary(current_used_buffer)==TRUE)
				unlock_buffer(RB,current_used_buffer);
	
			if (is_read_only(next_buffer)==TRUE)
				;
			else
				if (input_buffer(next_buffer)==FALSE)
				{
					--forward;
					return;
				}

			sweep_swlock(current_used_buffer);
			forward=BUFFER_STAPOS(next_buffer);
			current_used_buffer=next_buffer;
			continue;
		}

		else if (t==';')
			break;

		else if (t==END_OF_FILE)
		{
			end_of_file=TRUE;
			break;
		}

		else if (t=='\n')
		  	++lineno;

		++total;
		++forward;
	}
	
	++forward;			// skip the semiconlon
}	
			
/*=========================More about searching============================================ */

// Just in current buffer
int buffer_searchc(const char k,int offset)
{
	int start_pos=forward+offset;
	int i=start_pos-1;
	int n=BUFFER_ENDPOS(current_used_buffer);
	int en_com=FALSE;				/* if encounter the comments		*/

	while (++i<=n)
	{
		if (buffer[i]=='#')
		{
			en_com=TRUE;
			continue;
		}

		if (en_com==TRUE)
		{
			while (++i<=n && buffer[i]!='\n');
			en_com=FALSE; 
			continue;
		}

		else
		{
			if (buffer[i]==k)
				return i-start_pos;
		}
	}

	return FALSE;
}

// it promises to search until the next buffer ending
int buffer_gbl_searchc(const char k , int offset)
{
	/* To execute properly , you mustn't lock the buffer					*/
	/* To understand my approach is a bit difficult , please read developer guide		*/

	int start_pos=forward+offset;
	int i=start_pos-1;
	int gap=0;					/* if has crossed the gap		*/
	int en_com=FALSE;				/* if encounter the comments		*/
	int n=BUFFER_ENDPOS(current_used_buffer);

	while (1)
	{
		++i;

		if (buffer[i]=='#')
		{
			en_com=TRUE;
			continue;
		}
		
		else if (buffer[i]==EOS)
		{
			if (gap++)
				break;

			current_used_buffer=(current_used_buffer+1)&1;
			if (input_buffer(current_used_buffer)==FALSE)
				return FALSE;

			lock_buffer(SW_LOCK,current_used_buffer);		/* triky	*/
			i=BUFFER_STAPOS(current_used_buffer);
			n=BUFFER_ENDPOS(current_used_buffer);
			continue;
		}

		if (en_com==TRUE)
		{
			while (++i<=n && buffer[i]!='\n');
			en_com=FALSE; 
			continue;
		}

		else
		{
			if (buffer[i]==k)
				return i-start_pos;
		}
	}

	return FALSE;
}

// it also provides the functionality to search in next buffer
int buffer_searchc_until(const char k, int offset , const char d)
{
	int start_pos=forward+offset;
	int i=start_pos-1;
	int gap=0;					/* if has crossed the gap		*/
	int en_com=FALSE;				/* if encounter the comments		*/
	int n=BUFFER_ENDPOS(current_used_buffer);
	int found=0;

	while (1)
	{
		++i;

		if (buffer[i]=='#')
		{
			en_com=TRUE;
			continue;
		}
		
		else if (buffer[i]==EOS)
		{
			if (gap++)
				break;

			current_used_buffer=(current_used_buffer+1)&1;
			if (input_buffer(current_used_buffer)==FALSE)
				return FALSE;

			lock_buffer(SW_LOCK,current_used_buffer);		/* triky	*/
			i=BUFFER_STAPOS(current_used_buffer);
			n=BUFFER_ENDPOS(current_used_buffer);
			continue;
		}

		if (en_com==TRUE)
		{
			while (++i<=n && buffer[i]!='\n');
			en_com=FALSE; 
			continue;
		}

		else
		{
			if (buffer[i]==k)
			{
				found=i;
				break;
			}

			else if (buffer[i]==d)
				break;
		}
	}
	
	if (found==0)
		return FALSE;
	else
		return found-start_pos;
}
  	
int move_to(int next_pos)
{
	/*	We have three conditions:
		1.in the same position;
		2.currently be 1 , wants to be 2;
		2.currently be 2 , wants to be 1;
		and also I must consider the read/write lock .
											*/
	if (is_boundary(current_used_buffer)==TRUE)
	{
		if (next_pos>=(BUFFER_STAPOS(current_used_buffer))  &&
			next_pos<=(BUFFER_ENDPOS(current_used_buffer)))
		{
			forward=next_pos;
			return TRUE;
		}
	}

	else
	{
		forward=next_pos;
		return TRUE;
	}

	return FALSE;
}

int move_add_to(int offset)
{
	if (is_boundary(current_used_buffer)==TRUE)
	{
		if ((forward+=offset)>=(BUFFER_STAPOS(current_used_buffer))  &&
			forward<=(BUFFER_ENDPOS(current_used_buffer)))
			return TRUE;
		
		else
		  	forward-=offset;
	}

	else
	{
		forward+=offset;
		return TRUE;
	}

	return FALSE;
}

/*============================================Misc functions===========================*/
int has_no_data()
{
	/* We justify this from two aspects:
		1.There's no unread data in pond;
		2.There's no unused data in buffer;
									*/

	if (data_length==0 && end_of_file==TRUE)

		return TRUE;

	return FALSE;
}

int get_pointer()
{
	lock_buffer(1,current_used_buffer);		/* hard to understand			*/

	return forward;					/* the absolute offset			*/
}

int lock_buffer(int type,int which)
{
	/*	Why I should bring in the concept of lock ?
		If any function get the buffer pointer stored in its context , and later it wants to
		be restored , but the buffer has been flushed , then it will get a wrong answer .
		
		This is complete new to this version , I will discuss it in the developer guide .
	*/

	int signal=0;
	
  	switch (type)
	{
		case RB_LOCK:
			signal=GET_LOCK(RB,which);
			break;
		case RO_LOCK:
			signal=GET_LOCK(RO,which);
			break;
		case SW_LOCK:
			signal=GET_LOCK(SW,which);
			break;
		default:
			return FALSE;			/* perhaps will not execute forever	*/
	}

	lock &=signal;

	return TRUE;		
}

int unlock_buffer(int type, int which)
{
	int signal=0;
	
	switch (type)
	{
		case RB_LOCK:
			signal=(~GET_LOCK(RB,which));
			break;
		case RO_LOCK:
			signal=(~GET_LOCK(RO,which));
			break;
		case SW_LOCK:
			signal=(~GET_LOCK(SW,which));
			break;
		default:
			return FALSE;
	}
		
	lock &=signal;

	return TRUE;
}

int is_read_only(int which)
{
	int signal=GET_LOCK(RB,which);
	if ((lock & signal)!=0)
		return TRUE;

	signal=GET_LOCK(RO,which);
	if ((lock & signal)!=0)
		return TRUE;

	signal=GET_LOCK(SW,which);
	if ((lock & signal)!=0)
		return TRUE;

	return FALSE;
}

int is_boundary(int which)
{
	int signal=GET_LOCK(RB,which);
	if ((lock & signal)!=0)
		return TRUE;

	return FALSE;
}

void sweep_swlock(int which)
{
	int rev_lock=GET_LOCK(SW,which);
	lock &=(~rev_lock);
}

int GET_LOCK(int type,int which)
{
	switch (type)
	{
		case RB_LOCK:
			if (which==0)
				return MAKE_LOCK(RB,1);
			else
				return MAKE_LOCK(RB,2);

		case RO_LOCK:
			if (which==0)
				return MAKE_LOCK(RO,1);
			else
				return MAKE_LOCK(RO,2);
	
		case SW_LOCK:
			if (which==0)
				return MAKE_LOCK(SW,1);
			else
				return MAKE_LOCK(SW,2);

		default:
			break;
	}

	return 1987224;					/* cannot reach here	*/
}
