#include <types.h>
#include <dictionnaire.h>
#include <binio.h>
#include <math.h>
#include <compression.h>



 /**
  * Compresse les données du fichier aInput dans un fichier aOutput,
  * en utilisant l'algorithme de Lampel Ziv Welch.
  * @param aInput : fichier d'entrée contenant les données à compresser
  * @param aOutput : fichier de sortie contenant les données compressées
  * @param aBufferInput : Buffer lié au fichier d'entrée aInput
  * @param aBufferOutput : Buffer lié au fichier de sortie aOutput
  * @return 0 si pas d'erreur et 1 sinon.
  */
int compression_LZW(FILE* aInput, Buffer* aBufferInput, FILE* aOutput, Buffer* aBufferOutput){
	Code wPrefixe, wMono, wPrefixeMono;
	int wLengthBitsToWrite;

	if(aInput == NULL && aBufferInput==NULL && aOutput==NULL && aBufferOutput){
		return 1;
	}

	wLengthBitsToWrite=9;

	initialiser();
	wPrefixe=bRead(aInput, 8, aBufferInput);
	while(bfeof(aInput, aBufferInput, 8) == 0){
		wMono=bRead(aInput, 8, aBufferInput);
		/* Réinitialise le dico si on est sur le point de faire un code sur 14 bits */
		if(isFull()==1 && wLengthBitsToWrite + 1 > 14){
			bWrite(aOutput,wLengthBitsToWrite,(Code)258, aBufferOutput);
			fprintf(stderr, "%d : %d\n", 258, wLengthBitsToWrite);
			wPrefixe = wMono;
			wLengthBitsToWrite = 9;
			initialiser();
			continue;
		}
		if(existe_seq(wPrefixe,wMono, &wPrefixeMono)==0){
			wPrefixe=wPrefixeMono;
		}else{
			/* Il faut augmenter la taille du code car le dictionnaire est plein */
			if(isFull()==1){
				bWrite(aOutput,wLengthBitsToWrite,(Code)257, aBufferOutput);
				fprintf(stderr, "%d : %d\n", 257, wLengthBitsToWrite);
				wLengthBitsToWrite++;
			}

			bWrite(aOutput,wLengthBitsToWrite,wPrefixe, aBufferOutput);
			fprintf(stderr, "%d : %d\n", wPrefixe, wLengthBitsToWrite);
			inserer(wPrefixe, wMono);
			wPrefixe=wMono;
		}
	}
	bWrite(aOutput,wLengthBitsToWrite,wPrefixe,aBufferOutput);
	fprintf(stderr, "%d : %d\n", wPrefixe, wLengthBitsToWrite);

	/* Ecriture de EOF*/
	bWrite(aOutput,wLengthBitsToWrite,(Code)256,aBufferOutput);
	fprintf(stderr, "%d : %d\n", 256, wLengthBitsToWrite);
	liberer();
	return 0;
}
