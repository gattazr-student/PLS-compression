#include <rle.h>
#include <binio.h>

/*
* Coder RLE
* Permet de pré traiter le fichier avec un codage RLE
* @param : aFilein : fichier d'entrée
* @param : aFileout : fichier de sortie
*/

void coderRle(FILE* aFilein, FILE* aFileout){

	int wEtat = 1;
	Code wC, wCcourant, wI = 0;
	Buffer* wBlect = NULL;
	Buffer* wBecr = NULL;
	
	wBlect = bMakeBuffer();
	wBecr = bMakeBuffer();
	wC = bRead(aFilein, 8, wBlect);

	while (!bfeof(aFilein, wBlect, 8)) {
		if (wEtat == 1) {                  
			wCcourant = wC;
			wEtat = 2;
			bWrite(aFileout, 8, wCcourant, wBecr);
		} else if (wEtat == 2) {           
			if (wC == wCcourant) {
				wEtat = 3;
			} else {
				wCcourant = wC;
			}
			bWrite(aFileout, 8, wCcourant, wBecr);
		} else {                            
			if (wC == wCcourant && wI < 255){
				wI++;
			} else {
				bWrite(aFileout, 8, wI, wBecr);
				wCcourant = wC;
				bWrite(aFileout, 8, wCcourant, wBecr);
				wI = 0;
				wEtat = 2;
			}
		}
		wC = bRead(aFilein, 8, wBlect);
	}
}


/*
* Decoder RLE
* Permet de décoder le fichier pré traité par RLE
* @param : aFilein : fichier d'entrée
* @param : aFileout : fichier de sortie
*/

void decoderRle(FILE* aFilein, FILE* aFileout){

	int wEtat = 1, wJ;
	Code wC, wCcourant, wI = 0;
	Buffer* wBlect = NULL;
	Buffer* wBecr = NULL;

	wBlect = bMakeBuffer();
	wBecr = bMakeBuffer();
	wC = bRead(aFilein, 8, wBlect);

	while (!bfeof(aFilein, wBlect, 8)) {
		if (wEtat == 1){
			wCcourant = wC;
			bWrite(aFileout, 8, wC, wBecr);
			wEtat = 2;
		} else if (wEtat == 2) {
			if (wCcourant != wC) {
				wCcourant = wC;
				bWrite(aFileout, 8, wC, wBecr);
			} else {
				bWrite(aFileout, 8, wC, wBecr);
				wEtat = 3;
			}
		} else {
			wI = wC;
			for (wJ = 0 ; wJ < wI ; wI++) {
				bWrite(aFileout, 8, wCcourant, wBecr);
			}
			wEtat = 1;
		}
		wC = bRead(aFilein, 8, wBlect);
	}
}
