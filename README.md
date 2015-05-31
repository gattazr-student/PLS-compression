Compression LZW
===============

Structures de données :
------------------------

### Dictionnaire :
- **int** nbElements : nombre d'élements dans le dictionnaire
- **int** tailleDico : dernier élément du dictionnaire dans lequel on peut écrire 
- **Arbre**** ids : Tableau de pointeur sur Arbre
- **Arbre*** dico : Tableau d'arbres

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
│   ├── test\_r-w\_binio.c

```

Convention de nommage :
---------------------
- Paramètre des fonctions : aX ``` int main(int aArgc, char** aArgv){} ```
- Variable locale : wX ```char* wBuffer;```
- Variable globale : XXX ```int LINELEN = 1024;```
