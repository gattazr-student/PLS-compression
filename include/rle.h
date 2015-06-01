#include <stdio.h>
#include <stdlib.h>
#include <binio.h>

/*
* Coder RLE
* Permet de pré traiter le fichier avec un codage RLE
* @param : aFilein : fichier d'entrée
* @param : aFileout : fichier de sortie
*/

void coderRle(FILE* aFilein, FILE* aFileout);

/*
* Decoder RLE
* Permet de décoder le fichier pré traité par RLE
* @param : aFilein : fichier d'entrée
* @param : aFileout : fichier de sortie
*/

void decoderRle(FILE* aFilein, FILE* aFileout);
