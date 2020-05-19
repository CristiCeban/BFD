#!/bin/bash 
# On génère un fichier aléatoire
RED="\033[1;31m"
NC="\033[0m"
GREEN="\033[1;32m"
PURPLE="\033[1;35m"

size=2345677

printf "${RED}Create random file${NC}\n" 
dd if=/dev/urandom of=srcfile bs=${size} count=1 
cp srcfile savefile

for testprog in $(find tests/ -executable -type f) 
do
	printf "\n\n${RED}${testprog} :${NC}\n\n"
	rm -f destfile
	./$testprog srcfile destfile
	if cmp -s srcfile savefile
 	then
		if cmp -s srcfile destfile
	 	then
			printf "${GREEN}Succes${NC}\n"
		else
			printf "${PURPLE}Error : Copied file is different from the original one${NC}\n"
		fi
	else
		echo "Error : the program modified the file it was supposed to copy"
		rm -f srcfile destfile savefile
		exit 1
	fi
	printf "\n"
	LSAN_OPTIONS="detect_leaks=0" strace -c --trace=read,write ./$testprog srcfile destfile
	printf "\n"
done
rm -f srcfile destfile savefile

