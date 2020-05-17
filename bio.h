#include <errno.h>
#include <sys/types.h>

#ifndef BIO_H
#define BIO_H

#define BUFSIZE 2048

// Valeur de retour de read quand il n'y a plus rien 
// � lire
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
 * fait dans la fonction. Les diff�rents champs doivent �galement
 * y �tre initialis�s. Les arguments sont:
 * - path : chemin vers le fichier
 * - mode : r pour lecture, w pour �criture 
 *    en tronquant le fichier, a pour ajouter
 *    � la fin du fichier
 *    Si le fichier n'existe pas, celui-ci est cr�� avec les droits 
 *    en �criture uniquement pour l'utilisateur et en lecture pour tous.
 * renvoie NULL si l'ouverture a �chou�, l'erreur est stock�e dans 
 * errno. Si le mode n'est ni "r" ni "w", errno est mis � EINVAL.
 **/
BFD* 
bopen(const char *path, const char *mode);

/* Essaye de lire size octets. Si le buffer contient suffisamment
 * d'octets, ceux-ci sont recopi�s, si ce n'est pas le cas, on 
 * recopie ceux qui y sont et on fait des read de taille 
 * bfd->bufsize jusqu'� ce que soit la fin de fichier soit atteinte,
 * soit size octets aient �t� recopi�s. 
 *
 * Renvoie le nombre d'octets lus ou BERROR en cas d'erreur, 
 * errno est en plus mis � EBADF si le BFD n'est pas valide.
 * */
size_t 
bread(char *buf, size_t size, BFD *bfd);

/* Essaye d'ajouter size octets dans le buffer de bfd. Quand celui-ci est
 * rempli, il est ecrit avec write dans le fichier.
 * 
 * Renvoie le nombre d'octets ecrits,
 *  BERROR en cas d'erreur, errno est en plus mis
 * � EBADF si le BFD n'est pas valide.
 * */
size_t 
bwrite(const char *buf, size_t size, BFD *bfd);

/* Flush :
 * �crit tout ce qui est rest� dans le buffer
 * pour le BFD et renvoie 0 si il a r�ussi.
 * Si une erreur a eu lieu, renvoie BERROR et
 * errno contient l'erreur. Doit v�rifier si le bfd 
 * correspond � un fichier ouvert en �criture. Si l'erreur
 * est li�e au BFD, errno = EBADF.
 * */
int 
bflush(BFD *bfd);

/* Teste si le curseur est � la fin du fichier (est-ce qu'un read sur bfd->fd
 * renverra quelque chose ?).
 *  renvoie 0 si non et une valeur positive si oui.
 *  si oui. Renvoie BERROR en cas d'erreur, si celle-ci est li�e 
 *  au BFD, errno = EBADF. 
 *  */
int
beof(BFD *bfd);

/* Flush le bfd si celui-ci est ouvert en �criture 
 * puis ferme le descripteur et lib�re la structure.
 * Renvoie 0 en cas de succ�s et -1 avec un errno
 * sinon. Si l'erreur est li�e au BFD, errno est mis � EBADF.
 * */
int 
bclose(BFD *bfd);

#endif
