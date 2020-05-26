#!/bin/bash 
# On génère un fichier aléatoire
RED="\033[1;31m"
NC="\033[0m"
GREEN="\033[1;32m"
PURPLE="\033[1;35m"

size=2345677

printf "${RED}Creation d'un fichier aléatoire${NC}\n" 
dd if=/dev/urandom of=srcfile bs=${size} count=1 
cp srcfile savefile

for testprog in $(find tests/ -perm -u+x -type f) 
do
	printf "\n\n${RED}${testprog} :${NC}\n\n"
	rm -f destfile
	./$testprog srcfile destfile
	if cmp -s srcfile savefile
 	then
		if cmp -s srcfile destfile
	 	then
			printf "${GREEN}Réussi${NC}\n"
		else
			printf "${PURPLE}Echec : le fichier copié est différent${NC}\n"
		fi
	else
		echo "Echec : le programme a modifié le fichier qu'il devait copier"
		rm -f srcfile destfile savefile
		exit 1
	fi
	printf "\n"
	LSAN_OPTIONS="detect_leaks=0" strace -c -e trace=read,write ./$testprog srcfile destfile
	printf "\n"
done
rm -f srcfile destfile savefile

