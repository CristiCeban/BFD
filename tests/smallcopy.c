#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char **argv){
				char buffer[8];
				ssize_t nbread;
				int fd_src,fd_dest;

				if(argc<3){
					printf("%s: Usage\n%s src dest : copie src dans dest.\n",argv[0],argv[0]);
					return 1;
				}

				fd_src = open(argv[1],O_RDONLY);
				fd_dest = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);

				while((nbread=read(fd_src,buffer,7))>0){
					write(fd_dest,buffer,nbread);
				}
				close(fd_dest);
				close(fd_src);
				return 0;
}
