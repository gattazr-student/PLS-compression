#include <types.h>
#include <dictionnaire.h>
#include <binio.h>
#include <string.h>

/**
 * decompression_LZW
 * fonction de décompression selon l'algorithme LZW
 * @param aFileIn : pointeur sur le fichier à décompresser
 * @param aBufferIn : buffer utilisé pour la lecture du fichier aFileIne
 * @param pointeur : sur le fichier destination de la décompression
 * @param aBufferOut : buffer utilisé pour l'écriture du fichier aFileOut
 * @return 0 si la décompression n'a pas généré d'erreur..
 */
int decompression_LZW(FILE *aFileIn, Buffer *aBufferIn, FILE *aFileOut, Buffer *aBufferOut){

	Code wPrefix;
	Code wMono;
	int wSize;
	char *wString;
	int wStrLen;
	int wMonoExist;
	int wI;

	/* Une des structure nécessaire à la décompression n'est pas utilisable */
	if(aFileIn == NULL || aBufferIn == NULL || aFileOut == NULL || aBufferOut == NULL){
		return -1;
	}

	initialiser(); /* Initiailisation du dictionnaire */
	wSize = 9; /* La taille de base est toujours 9 en décompression */
	wPrefix = bRead(aFileIn, wSize, aBufferIn);
	bWrite(aFileOut, wSize, wPrefix, aBufferOut);
	while(bfeof(aFileIn, aBufferIn, wSize) == 0){
		wMono = bRead(aFileIn, wSize, aBufferIn);
		/* Actions spéciales*/
		if(wMono > INI_TAB_SIZE-1 && wMono < FIRST_AVAILABLE){
			switch (wMono) {
				case 256:
					/* EOF */
					return 0;
				break;
				case 257:
					/* Agrandir le dictionnaire */
					wSize++;
				break;
				case 258:
					/* Réinitialiser le dictionnaire */
					initialiser();
				break;
			}
			continue;
		}
		wMonoExist = existe_code(wMono);
		if(wMonoExist == 0){
			wString = codeVersSequence(wMono);
		}else{
			wString = codeVersSequence(wPrefix);
		}
		/* Ecrire wString dans aFileOut */
		wStrLen = strlen(wString);
		for(wI = 0; wI < wStrLen; wI++){
			bWrite(aFileOut, wSize, wString[wI], aBufferOut);
		}
		/* Ecris aussi wString[0] si necessaire */
		if(wMonoExist == 0){
			bWrite(aFileOut, wSize, wString[0], aBufferOut);
		}
		wPrefix = inserer(wPrefix, wString[0]);
		free(wString);
	}
	/* La compression s'est bien déroulé*/
	return 0;
}
