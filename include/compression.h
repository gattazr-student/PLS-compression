#include <binio.h>



 /**
  * Compresse les données du fichier aInput dans un fichier aOutput, 
  * en utilisant l'algorithme de Lampel Ziv Welch.  
  * @param aInput : fichier d'entrée contenant les données à compresser
  * @param aOutput : fichier de sortie contenant les données compressées
  * @param aBuffersInput : Buffer lié au fichier d'entrée aInput
  * @param aBuffersOutput : Buffer lié au fichier de sortie aOutput
  * @return 0 si pas d'erreur et 1 sinon.
  */
int compression_LZW(FILE* aInput, FILE* aOutput, Buffer* aBuffersInput, Buffer* aBuffersOutput);
