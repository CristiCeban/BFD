#include "bio.h"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


#ifdef DEBUG
#define debug_printf(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define debug_printf(fmt, args...)    /* Don't do anything in release builds */
#endif


BFD* 
bopen(const char *path, const char *mode){
BFD *bp ;
	//Check if memory was alocated.
	if((bp=malloc(sizeof(BFD)))==NULL)
		return NULL;
	
	//Check mode and opens file descriptor with the coresponding flags.

	//write
	if(strcmp("w",mode)==0){
		//Open file
		bp->fd = open(path,O_WRONLY | O_TRUNC | O_CREAT,0644);
		//Store the mode
		bp->mode =  BWRITE;
		//Store the size of the file
		bp->fsize = lseek(bp->fd,0,SEEK_END);
		//Reset the cursor in the file
		lseek(bp->fd,0,SEEK_SET);
		//Store the begining of the cursor
		bp->cur = SEEK_SET;	
	}
	//read
	else if(strcmp("r",mode)==0){
		//open file
		bp->fd = open(path,O_RDONLY);
		//store the mode
		bp->mode = BREAD;
		//Store the size of the file
		bp->fsize = lseek(bp->fd,0,SEEK_END);
		//Reset the cursor in the file
		lseek(bp->fd,0,SEEK_SET);
		//Store the begining of the cursor
		bp->cur = SEEK_SET;
	}
	//append
	else if(strcmp("a",mode)==0){
		//Open file
		bp->fd = open(path,O_WRONLY| O_APPEND | O_CREAT,0644);
		//store the mode
		bp->mode = BWRITE;
		//Store the size of the file
		bp->fsize = lseek(bp->fd,0,SEEK_END);
		//store the current cursor possition.
		//Because it is in append mode it will be after the final of the file.
		bp->cur = bp->fsize;
	}
	//if it's neither from the modes.
	else {
		errno = EINVAL;
		return NULL;
	}

	//error with file descriptor
	if(bp->fd < 0)
		return NULL;
	
	//intialise the cursors in the buffer;
	bp->pos = 0;
	bp->end = 0;
		
	return bp;
}

size_t 
bwrite(const char *buf, size_t size, BFD *bfd){
	return 0;
}


size_t 
bread(char *buf, size_t size, BFD *bfd){
	return 0;
}

int 
bflush(BFD *bfd){
	return 0;
}

int
beof(BFD *bfd){
	return 0;
}

int 
bclose(BFD *bfd){
	//Check if file was opened
	if(bfd->mode!=BREAD && bfd->mode!=BWRITE){
		errno = EBADF;
		return -1;
	}

	//If it's in the writen mode
	if(bfd->mode==BWRITE)
		bflush(bfd);
	//close the file descriptor
	if((close(bfd->fd))==0){
		free(bfd);
		//Free alocated memory for bfd
		return 0;
	}
	errno = BERROR;
	return -1;
}
