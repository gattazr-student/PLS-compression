Compression LZW
===============

Structures de données :
------------------------

### Dictionnaire :
- **int** nbElements : nombre d'élements dans le dictionnaire
- **Abre**** ids : Tableau de pointeur sur Arbre
- **Abre*** dico : Tableau d'arbres

### Arbre :
- **char** valeur
- **Arbre*** enfant
- **Arbre*** parent
- **Arbre**\*\*\* id

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
└── tests
```
