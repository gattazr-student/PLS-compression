#include <compression.h>
#include <dictionnaire.h>
#include <math.h> 

 
 /**
  * Compresse les données du fichier aInput dans un fichier aOutput, 
  * en utilisant l'algorithme de Lampel Ziv Welch.  
  * @param aInput : fichier d'entrée contenant les données à compresser
  * @param aOutput : fichier de sortie contenant les données compressées
  * @param aBuffersInput : Buffer lié au fichier d'entrée aInput
  * @param aBuffersOutput : Buffer lié au fichier de sortie aOutput
  * @return 0 si pas d'erreur et 1 sinon.
  */
int compression_LZW(FILE* aInput, FILE* aOutput, Buffer* aBuffersInput, Buffer* aBuffersOutput){
	Code wPrefixe, wMono, wPrefixeMono;
	int wLengthBitsToWrite;

	if(aInput == NULL && aOutput==NULL && aBuffersInput==NULL && aBuffersOutput){
		return 1;
	}	
	
	wLengthBitsToWrite=9;
	
	initialiser();
	wPrefixe=bRead(aInput, 8, &(aBuffersInput[0]));
	while(bfeof(aInput, aBuffersInput, 8) == 0){
		wMono=bRead(aInput, 8, &(aBuffersInput[0]));
		if(existe_seq(wPrefixe,wMono, &wPrefixeMono)==0){
			wPrefixe=wPrefixeMono;		
		}else{	
			/* Calcul le nombre de bits à écrire dans le fichier de sortie et écrit la nouvelle valeur dans le fichier de sortie*/	
			if((log(wPrefixe)/log(2)) > wLengthBitsToWrite){
				bWrite(aOutput,wLengthBitsToWrite,(uint16_t)257, &(aBuffersOutput[1]));
				wLengthBitsToWrite++; 			
			}
			
			bWrite(aOutput,wLengthBitsToWrite,wPrefixe, &(aBuffersOutput[1]));
			inserer(wPrefixe,wMono);
			wPrefixe=wMono;
		}		
	}
	bWrite(aOutput,wLengthBitsToWrite,wPrefixe,&(aBuffersOutput[1]));
	
	/* Ecriture de EOF*/	
	bWrite(aOutput,wLengthBitsToWrite,(uint16_t)256,&(aBuffersOutput[1]));	
	
	return 0;
}


