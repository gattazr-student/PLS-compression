Compression LZW
===============

Structures de données :
------------------------

### Dictionnaire :
- **int** nbElements : nombre d'élements présents dans le dictionnaire
- **int** tailleDico : dernier élément du dictionnaire dans lequel on peut écrire 
- **Arbre**** ids : Tableau de pointeur sur Arbre
- **Arbre*** dico : Tableau d'arbres (1er étage)

### Arbre :
- **char** valeur
- **Arbre*** enfant
- **Arbre*** frere
- **Arbre*** parent
- **Code** code

### Code :
- typedef uint16_t


Structure du projet :
---------------------
```
LZW
├── README.md
├── doc
│   └── sujet
│       ├── complements.pdf
│       ├── compression.pdf
│       ├── lzw_I386.tgz
│       ├── lzw_amd64.tgz
│       └── rle_mtf.pdf
├── includes
│   ├── binio.h
│   ├── compression.h
│   ├── decompression.h
│   ├── dictionnaire.h
│   └── main.h
├── src
│   ├── Makefile
│   ├── binio.c
│   ├── compression.c
│   ├── decompression.c
│   ├── dictionnaire.c
│   └── main.c
├── tests
│   ├── data
│   │   ├── test1
│   │   └── test2
│   ├── Makefile
│   ├── test_dico.c
│   ├── test_r-w_binio.c

```

Convention de nommage :
---------------------
- Paramètre des fonctions : aX ``` int main(int aArgc, char** aArgv){} ```
- Variable locale : wX ```char* wBuffer;```
- Variable globale : XXX ```int LINELEN = 1024;```


Module dictionnaire :
---------------------

### Fonctions : 

- **initialiser** : initialise dico avec les 255 monoséquences, alloue la mémoire pour ids et initialise nbElements à 258 (255+3 "spéciaux")
- **inserer** :  insère le Code correspondant au code "aPrefixe.aMono" dans le dictionnaire en doublant la taille de ids si nécessaire
- **inverserChaine** : fonction auxiliaire permettant d'inverser une chaine de caractère
- **codeVersSequence** : retourne la chaine de caractère (séquence) correspondant à un code en remontant l'arbre puis en inversant la chaine de caractères à la fin
- **sequenceVersCode** : retourne le code correspondant à une séquence
- **existe_seq** : retourne 0 si le code de la séquence "prefixe.mono" existe (recherche à partir des codes de prefixe et mono qui sont respectivement aPrefixe et aMono). 1 sinon
- **existe_code** : retourne 0 si le code aCode existe. 1 sinon
- **liberer** : libère ids, dico et le dictionnaire
- **isFull** : retourne 1 si le dictionnaire est plein


Module binio :
---------------------

### Fonctions : 

- **bOpen** : Ouvre un fichier en mode binaire.
- **bMakeBuffer** : Créer un buffer pour la lecture ou l'écriture d'un fichier.
- **bRead** : Lit un nombre spécifique de bits dans le fichier de lecture et retourne sa valeur entière. 
- **bWrite** : Ecrit un code dans le fichier d'écriture en mode binaire.
- **bFlush** : Ecrit le maximum d'octets contenus dans le buffer d'écriture dans le fichier associé.
- **bFlush_Force** : Ecrit tout le contenu du buffer d'écriture dans le fichier associé.
- **bClose** : Vide le buffer d'écriture et ferme le fichier associé.
- **bCloseBuffer** : Ferme le buffer.
- **bfeof** : Indique si la fin de fichier est atteinte et que le buffer est vide.

Module RLE :
---------------------

### Fonctions : 

- **coderRle** : permet de pré-traiter le fichier donné avec un codage RLE
- **decoderRle** : permet de décoder un fichier pré-traité par RLE

Le codage se fait avec un automate à 2 états. On compare à chaque fois l'élément courant à l'élément précédent : si les deux sont égaux, on passe dans l'état 2 en incrémentant un compteur.
Le résultat du codage est de la forme suivante :
	abbbbcddddd -> 1a4b1c5d
Ainsi, pour un fichier où il y a peu de redondance, nous ajoutons un "1" devant chaque élément -> la taille du fichier va donc doubler...
Nous avons choisi de faire des paquets de 255 maximum.


Difficultés rencontrées :
---------------------



