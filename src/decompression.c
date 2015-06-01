#include <types.h>
#include <dictionnaire.h>
#include <binio.h>
#include <string.h>
#include <decompression.h>

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
	/* Le premier préfixe doit être un mono */
	if(wPrefix > INI_TAB_SIZE-1){
		fprintf(stderr, "The file format is wrong\n");
		return -1;
	}
	bWrite(aFileOut, 8, wPrefix, aBufferOut);
	while(bfeof(aFileIn, aBufferIn, wSize) == 0){
		wMono = bRead(aFileIn, wSize, aBufferIn);
		/* Actions spéciales*/
		if(wMono > INI_TAB_SIZE-1 && wMono < FIRST_AVAILABLE){
			switch (wMono) {
				case 256:
					/* EOF */
					liberer();
					return 0;
				break;
				case 257:
					/* Agrandir le nombre de bits sur lequel lire */
					wSize++;
				break;
				case 258:
					/* Réinitialiser le dictionnaire */
					initialiser();
					wPrefix = bRead(aFileIn, wSize, aBufferIn);
					wSize = 9;
					if(wPrefix > INI_TAB_SIZE-1){
						fprintf(stderr, "The file format is wrong\n");
						return -1;
					}
					bWrite(aFileOut, 8, wPrefix, aBufferOut);
					continue;
				break;
			}
			continue;
		}
		wMonoExist = existe_code(wMono);
		if(wMonoExist == 0){
			wString = codeVersSequence(wMono, &wStrLen);
		}else{
			wString = codeVersSequence(wPrefix, &wStrLen);
		}

		/* Ecrire wString dans aFileOut */
		for(wI = 0; wI < wStrLen; wI++){
			bWrite(aFileOut, 8, wString[wI], aBufferOut);
		}

		wPrefix = inserer(wPrefix, wString[0]);

		/* Ecris aussi wString[0] si necessaire */
		if(wMonoExist != 0){
			bWrite(aFileOut, 8, wString[0], aBufferOut);
		}else{
			wPrefix = wMono;
		}

		free(wString);
	}

	/* La compression s'est bien déroulée */
	liberer();
	return 0;
}
