#include <errno.h>
#include <sys/types.h>

#ifndef BIO_H
#define BIO_H

#define BUFSIZE 2048

// Valeur de retour de read quand il n'y a plus rien 
// à lire
#define BEOF -3
#define BERROR -1

#define BREAD 1
#define BWRITE 2

typedef struct bfd {
	int bufsize; // taille du buffer
	char buffer[BUFSIZE]; // buffer
	char mode; // BREAD, BWRITE
	int fd; // descripteur de fichier
	// position dans le buffer
	int pos;
	int end;
	// taille et position dans le fichier 
	off_t fsize;
	off_t cur;
} BFD;


/* Ouvre un fichier et renvoie un BFD*. L'allocation du BFD se 
 * fait dans la fonction. Les différents champs doivent également
 * y être initialisés. Les arguments sont:
 * - path : chemin vers le fichier
 * - mode : r pour lecture, w pour écriture 
 *    en tronquant le fichier 
 *    Si le fichier n'existe pas, celui-ci est créé avec les droits 
 *    en écriture uniquement pour l'utilisateur et en lecture pour tous.
 * renvoie NULL si l'ouverture a échoué, l'erreur est stockée dans 
 * errno. Si le mode n'est ni "r" ni "w", errno est mis à EINVAL.
 **/
BFD* 
bopen(const char *path, const char *mode);

/* Essaye de lire size octets. Si le buffer contient suffisamment
 * d'octets, ceux-ci sont recopiés, si ce n'est pas le cas, on 
 * recopie ceux qui y sont et on fait des read de taille 
 * bfd->bufsize jusqu'à ce que soit la fin de fichier soit atteinte,
 * soit size octets aient été recopiés. 
 *
 * Renvoie le nombre d'octets lus ou BERROR en cas d'erreur, 
 * errno est en plus mis à EBADF si le BFD n'est pas valide.
 * */
size_t 
bread(char *buf, size_t size, BFD *bfd);

/* Essaye d'ajouter size octets dans le buffer de bfd. Quand celui-ci est
 * rempli, il est ecrit avec write dans le fichier.
 * 
 * Renvoie le nombre d'octets ecrits,
 *  BERROR en cas d'erreur, errno est en plus mis
 * à EBADF si le BFD n'est pas valide.
 * */
size_t 
bwrite(const char *buf, size_t size, BFD *bfd);

/* Flush :
 * écrit tout ce qui est resté dans le buffer
 * pour le BFD et renvoie 0 si il a réussi.
 * Si une erreur a eu lieu, renvoie BERROR et
 * errno contient l'erreur. Doit vérifier si le bfd 
 * correspond à un fichier ouvert en écriture. Si l'erreur
 * est liée au BFD, errno = EBADF.
 * */
int 
bflush(BFD *bfd);

/* Teste si le curseur est à la fin du fichier (est-ce qu'un read sur bfd->fd
 * renverra quelque chose ?).
 *  renvoie 0 si non et une valeur positive si oui.
 *  si oui. Renvoie BERROR en cas d'erreur, si celle-ci est liée 
 *  au BFD, errno = EBADF. 
 *  */
int
beof(BFD *bfd);

/* Flush le bfd si celui-ci est ouvert en écriture 
 * puis ferme le descripteur et libère la structure.
 * Renvoie 0 en cas de succès et -1 avec un errno
 * sinon. Si l'erreur est liée au BFD, errno est mis à EBADF.
 * */
int 
bclose(BFD *bfd);

#endif
