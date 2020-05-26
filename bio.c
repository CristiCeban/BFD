#include "bio.h"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>

#ifdef DEBUG
#define debug_printf(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define debug_printf(fmt, args...)    /* Don't do anything in release builds */
#endif

// typedef struct bfd {
	// int bufsize; // taille du buffer
	// char buffer[BUFSIZE]; // buffer
	// char mode; // BREAD, BWRITE
	// int fd; // descripteur de fichier
	// position dans le buffer
	// int pos;
	// int end;
	// taille et position dans le fichier 
	// off_t fsize;
	// off_t cur;
// } BFD;
BFD* 
bopen(const char *path, const char *mode){
	BFD *bp =malloc(sizeof(BFD));
	//Check if memory was alocated.
	
	if(bp==NULL)
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
	
	//Set the size of buffer to 0
	bp->bufsize = 0;
	//intialises the cursors in the buffer;
	bp->pos = 0;
	bp->end = 0;
		
	return bp;
}

size_t 
bwrite(const char *buf, size_t size, BFD *bfd){
	//good case
	if(bfd->bufsize + size < BUFSIZE){
		for(int i = 0;i<size;i++){
			bfd->buffer[bfd->pos+i] = buf[i]; 
		}
		bfd->pos += size;
		bfd->bufsize +=size;
		bfd->end +=size;
		//printf("First case\n");
	}
	else{
		//printf("second case\n");
		//TODO GOOD MAKE CASE WITH ALOCATIONS
		//flush buffer and realoc it
		bflush(bfd);

		//read to buffer
		while(size + bfd->bufsize >2048){
			if((write(bfd->fd,buf,BUFSIZE)<0))
				return -1;
			size-=BUFSIZE; //-1?
			buf+=BUFSIZE; //-1?
		}

		//Copy buffer
		memcpy(bfd->buffer,buf,size);
		bfd->pos+=size;
		bfd->end+=size;
		bfd->bufsize+=size;

	}
	return 0;
}

size_t 
bread(char *buf, size_t size, BFD *bfd){
	return read(bfd->fd,buf,size);
}

int 
bflush(BFD *bfd){
	//flash file

	//If it's oppened for write
	if(bfd->mode==BWRITE){
		if(bfd->bufsize>0){
			ssize_t nbh_w = write(bfd->fd,bfd->buffer,bfd->bufsize);
			if(nbh_w>0){
				memset(bfd->buffer,'\0',bfd->bufsize);
				bfd->bufsize = 0;
				bfd->pos = 0;
				bfd->end = 0;
				bfd->fsize += nbh_w;
				bfd->cur += nbh_w;
				return 0;
			}
			else return BERROR;
		}
	}
	//nothing to flush
	errno = EBADF;
	return BERROR;
}

int
beof(BFD *bfd){
	char c;
	int nbr;
	//check if not return nothing.
	if ((nbr=read(bfd->fd,&c,1))==0)
		return 1;
	//If returned something.
	else if(nbr>0)
		return 0;
	//If it's a problem with read.
	else {
		errno = EBADF;
		return BERROR;
	}
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
		if((bflush(bfd))!=0){
			return -1;
		}
	//close the file descriptor
	if((close(bfd->fd))==0){
		free(bfd);
		//Free alocated memory for bfd
		return 0;
	}
	errno = EBADF;
	return -1;

}
