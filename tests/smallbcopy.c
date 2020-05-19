#include <stdio.h>
#include "../bio.h"

int main(int argc, char **argv){
    FILE *f_src,*f_dest;
    int a;
    if(argc<3){
		printf("%s: Usage\n%s src dest : copie src dans dest.\n",argv[0],argv[0]);
        return 1;
    }
    f_src = fopen(argv[1],"r");
    f_dest = fopen(argv[2],"w");
    while( (a = fgetc(f_src)) != EOF )
    {
      fputc(a, f_dest);
    }

    fclose(f_src);
    fclose(f_dest);
    return 0;
}