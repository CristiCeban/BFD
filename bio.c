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
	return NULL;
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
	return 0;
}