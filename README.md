# Devoir Maison final de Systèmes d'exploitation

Le but de ce devoir maison de ce devoir maison est d'écrire des fonctions de
lecture/écriture bufferisées au fonctionnement analogue à celui de
`fopen`,`fread`,`fwrite`... que vous avez vues au cours de C du semestre
dernier.

Les fonctions de lecture/écriture bufferisées on pour but de minimiser le nombre
d'appels systèmes : chaque appel système engendrant un changement de contexte,
ceux-ci sont très coûteux.

Nos fonctions bufferisées auront pour nom `bopen`,`bread`,`bwrite`, vous devrez
compléter leur code dans les fichiers qui vous sont fournis. 


## Mise en place et rendu du Devoir

Le code à compléter est dans le dépôt https://git-etudiants.lacl.fr/root/l2-2020-sysexp-dm

Vous devrez rendre le devoir grâce au dépôt git qui a été créé explicitement
pour cela. Pour faire cela, une fois que vous avez cloné votre dépôt personnel (celui-ci sera vide par 
défaut),
vous allez ajouter une nouvelle source git :

```
git remote add upstream https://git-etudiants.lacl.fr/root/l2-2020-sysexp-dm
git pull upstream master
```

Le devoir maison est à rendre pour le **29 mai 23h59**. Seuls les 
devoirs qui sont dans le dépôt créé à cet effet seront acceptés,
aucun retard ne sera accepté.

**Votre code doit compiler pour être noté.**

## Organisation du code

La structure du code est la suivante :

```
.
├── Makefile
├── README.md
├── bio.c
├── bio.h
├── runtests.sh
└── tests
    ├── bcp.c
    ├── smallbcopy.c
    └── smallcopy.c
```

Vous devez compléter les fichiers `bio.c`, `smallbcopy.c` et `bcp.c`.

## Tests 

### Fichiers de tests
Le répertoire `tests` contient les fichiers de test de vos fonctions. Chaque
fichier est un programme dont le premier argument est un fichier à copier et le
second le fichier destination.

Par défaut nous vous fournissons un fichier de test exemple : `smallcopy.c`, ce
fichier ne teste pas vos fonctions mais servira de comparaison et d'exemple de
fichier de test. Il effectue une copie du fichier donné en premier argument
directement avec les fonction `read` et `write` que vous avez vues en cours,
mais avec un tout petit buffer.

Pour exécuter les tests, vous pouvez taper `make test` ou bien `./runtests.sh`.

### Comment lire la sortie des tests

Les tests sont effectués en copiant un fichier généré aléatoirement. Pour chaque
programme de test, on vérifie que le fichier copier est bien identique au
fichier que l'on a copié. Le nombre d'appels système est ensuite compté en
relançant le même programme avec strace. 

La sortie pour le programme de test `smallcopy` fourni est la suivante :
```
tests/smallcopy :

Réussi

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 53,30    2,439370           7    335097           write
 46,70    2,137105           6    335130           read
------ ----------- ----------- --------- --------- ----------------
100,00    4,576475           6    670227           total

```

La colonne `calls` contient le nombre de fois que chaque appel système a été
effectué.

### -fsanitize=address

Le makefile contient l'option `-fsanitize=address` qui permet de détecter les
fuites mémoire : buffer overflow, stack smashing, oubli de `free`... Cette
option est là pour vous aider à debugger.

## Fichiers à compléter
### `bio.c`

Le fichier `bio.c` contient les principales fonctions que vous devrez écrire,
leur fonctionnement est décrit dans `bio.h`. Pour les tester, vous devrez écrire
les programmes de test décrits dans la section suivante.

### `smallbcopy.c`

Le fichier `smallbcopy.c` sera l'équivalent de l'exemple donne `smallcopy.c`,
mais devra utiliser uniquement les fonctions de `bio.h`. Le buffer aura une
taille 7, comme dans `smallcopy.c`.

### `bcp.c`

Le fichier `bcp.c` devra contenir un analogue à `cp` : vous devez recopier le
fichier donné en premier argument dans le fichier donné en second argument. 
Pour cela vous utiliserez uniquement les fonctions de `bio.h` ainsi qu'un buffer
de taille 1024.




