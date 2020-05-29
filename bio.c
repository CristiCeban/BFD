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

BFD* 
bopen(const char *path, const char *mode){
	BFD *bp =malloc(sizeof(BFD));
	//Check if memory was alocated.
	
	if(bp==NULL)
		return NULL;
	
	//Check mode and open file descriptor with the coresponding flags.

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
	//Check if it was open for write.
	if(bfd->mode!=BWRITE){
		errno = EBADF;
		return BERROR;
	}
	//Number of bytes writen(return value)
	size_t nbh_w_b = 0;
	//Check if size of bfd->buffer is enought for buf
	if(bfd->bufsize + size < BUFSIZE){
		//copy buffer
		for(int i = 0;i<size;i++){
			bfd->buffer[bfd->pos+i] = buf[i]; 
		}
		//update fields 
		bfd->pos += size;
		bfd->bufsize +=size;
		bfd->end +=size;
		nbh_w_b += size;
	}
	else{
		//useful data in the bfd->buffer
		size_t t_len = BUFSIZE-bfd->bufsize;
		//Copy buffer
		for(int i = 0; i <t_len ;i++){
			bfd->buffer[bfd->pos+i] = buf[i];
		}
		nbh_w_b+=t_len;
		//Decrease the size of the buffer
		size-=t_len;
		//Move pointer to the new position.
		buf+=t_len;
		bfd->bufsize +=t_len;
		//flush buffer and set reset fields
		bflush(bfd);

		//If it contains more than buffer size then it read to fd directly.
		//also here bufsize is always 0.
		while(size + bfd->bufsize >=BUFSIZE){
			//If error or eof.
			if((write(bfd->fd,buf,BUFSIZE)<=0))
				return BERROR;
			size-=BUFSIZE; 
			buf+=BUFSIZE; 
		}

		//Copy remains buffer 
		memcpy(bfd->buffer,buf,size);
		//Update fields.
		bfd->pos+=size;
		bfd->end+=size;
		bfd->bufsize+=size;
		nbh_w_b+=size;
	}
	return nbh_w_b;
}

size_t 
bread(char *buf, size_t size, BFD *bfd){
	//if it's opened for read and file descriptor is OK
	if(bfd->mode!=BREAD || bfd->fd < 0){
		errno = EBADF;
		return BERROR;
	}
	//Useful buffer size
	size_t bfs_u = bfd->end-bfd->pos;
	//number of bytes readed by buf.(Return value)
	size_t nbh_r_b =0;
	//If it's enought useful bfd->buffer to copy to buf
	if(bfs_u>=size){
		for(int i = 0;i<size;i++){
			buf[i] =bfd->buffer[bfd->pos+i];
		}
		bfd->pos+=size;
		nbh_r_b+=size;
	}
	//If it's not enought space(maybe empty).
	else{
		//Copy useful buffer
		for(int i = 0;i<bfs_u;i++){
			buf[i] = bfd->buffer[bfd->pos+i];
		}
		//update return value
		nbh_r_b +=bfs_u;
		//Decrease the size of the buffer which is needed to be copied.
		size-=bfs_u;
		//Move the pointer to the new position
		buf+=bfs_u;
		//update pos in buffer
		bfd->pos+=bfs_u;
		//if not remains nothing to read by buf
		if(size==0)
			return nbh_r_b;
		//update fields of bfd
		bfd->pos=0;
		bfd->end=0;
		bfd->bufsize=0;
		//number of bytes readed
		size_t nbh_r = read(bfd->fd,bfd->buffer,BUFSIZE);
		//Error
		if(nbh_r<0)
			return BERROR;
		//eof
		else if(nbh_r == 0)
			return nbh_r_b;
		
		//Set fields to default.
		bfd->cur+=nbh_r;
		//copy buffer
		memmove(buf,bfd->buffer,size);
		//update fields of bfd.
		nbh_r_b+=size;
		bfd->pos+=size;
		bfd->end+=nbh_r;
		bfd->bufsize+=nbh_r;
	}
	return nbh_r_b;
}

int 
bflush(BFD *bfd){
	//If it's oppened for write
	if(bfd->mode==BWRITE && bfd->fd>=0){
		//If data are in buffer.
		if(bfd->bufsize>0){
			//Write the buffer to the file.
			ssize_t nbh_w = write(bfd->fd,bfd->buffer,bfd->bufsize);
			//Check if write was ok (>0).
			if(nbh_w>0){
				//Free buffer(it can't work and without this memset,but difference of time is not huge)
				//with memset 1.48%   0.009978s
				//without 1.43%    0.010558s
				memset(bfd->buffer,'\0',bfd->bufsize);
				//Update the fields of the bfd.
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
	//check if fd return something
	if ((nbr=read(bfd->fd,&c,1))==0)
		return 1;
	//If returned something.
	else if(nbr>0)
		return 0;
	//If it's a problem with read.
	errno = EBADF;
	return BERROR;
}

int 
bclose(BFD *bfd){
	//Check if file was opened
	if((bfd->mode!=BREAD && bfd->mode!=BWRITE)||(bfd->fd<0)){
		errno = EBADF;
		return BERROR;
	}

	//If it's in the writen mode
	if(bfd->mode==BWRITE)
		if((bflush(bfd))!=0){
			return BERROR;
		}
	//close the file descriptor
	if((close(bfd->fd))==0){
		free(bfd);
		//Free alocated memory for bfd
		return 0;
	}
	errno = EBADF;
	return BERROR;

}
