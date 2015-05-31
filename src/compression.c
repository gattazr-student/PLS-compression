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
		if(existe_seq(wPrefixe,wMono, &wPrefixeMono)==0){
			wPrefixe=wPrefixeMono;
		}else{
			/* Calcul le nombre de bits à écrire dans le fichier de sortie et écrit la nouvelle valeur dans le fichier de sortie*/
			if((log(wPrefixe)/log(2)) > wLengthBitsToWrite){
				bWrite(aOutput,wLengthBitsToWrite,(Code)257, aBufferOutput);
				wLengthBitsToWrite++;
			}

			bWrite(aOutput,wLengthBitsToWrite,wPrefixe, aBufferOutput);

			inserer(wPrefixe, wMono);
			wPrefixe=wMono;
		}
	}
	bWrite(aOutput,wLengthBitsToWrite,wPrefixe,aBufferOutput);

	/* Ecriture de EOF*/
	bWrite(aOutput,wLengthBitsToWrite,(Code)256,aBufferOutput);

	return 0;
}
