#include <stdio.h>
#include "../bio.h"

int main(int argc, char **argv){
        char buffer[1024];
				ssize_t nbread;
				BFD *bfd_src,*bfd_dest;

				if(argc<3){
					printf("%s: Usage\n%s src dest : copie src dans dest.\n",argv[0],argv[0]);
					return 1;
				}

				bfd_src = bopen(argv[1],"r");
				bfd_dest = bopen(argv[2],"w");

				while((nbread=bread(buffer,1022,bfd_src))>0){
					bwrite(buffer,nbread,bfd_dest);
				}
				bclose(bfd_dest);
				bclose(bfd_src);
				return 0;
        

}