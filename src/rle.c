#include <stdio.h>
#include <rle.h>

/*
* Coder RLE
* Permet de pré traiter le fichier avec un codage RLE
* @param : aFilein : fichier d'entrée
* @param : aFileout : fichier de sortie
*/

void coderRle(FILE* aFilein, FILE* aFileout){

	int wEtat = 1;
	unsigned char i = 1;
	int wC;
	int wCcourant;

	wCcourant = fgetc(aFilein);
	wC = fgetc(aFilein);

	while (!feof(aFilein)) {
		if(wEtat == 1){
			if(wC == wCcourant){
				i++;
				wEtat = 2;
			} else {
				fputc(1, aFileout);
				fputc(wCcourant, aFileout);
				wCcourant = wC;
			}
		}
		else if (wEtat == 2){
			if(wC == wCcourant){
				i++;
				if(i == 255){
					fputc(i, aFileout);
					fputc(wCcourant, aFileout);
					i = 1;
					wEtat = 1;
				}
			}
			else{
				fputc(i, aFileout);
				fputc(wCcourant, aFileout);
				i = 1;
				wEtat = 1;
			}
		}
		wCcourant = wC;
		wC = fgetc(aFilein);
		if(feof(aFilein)){
			fputc(i, aFileout);
			fputc(wCcourant, aFileout);
		}
	}

}

/*
* Decoder RLE
* Permet de décoder le fichier pré traité par RLE
* @param : aFilein : fichier d'entrée
* @param : aFileout : fichier de sortie
*/

void decoderRle(FILE* aFilein, FILE* aFileout){

	int wJ;
	int wC;
	int wCcourant;

	while (!feof(aFilein)) {
		wCcourant = fgetc(aFilein);
		wC = fgetc(aFilein);

		for(wJ=0; wJ<wCcourant; wJ++){
			fputc(wC, aFileout);
		}
	}
}
