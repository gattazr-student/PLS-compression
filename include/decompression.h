/**
 * decompression_LZW
 * fonction de décompression selon l'algorithme LZW
 * @param aFileIn : pointeur sur le fichier à décompresser
 * @param aBufferIn : buffer utilisé pour la lecture du fichier aFileIne
 * @param pointeur : sur le fichier destination de la décompression
 * @param aBufferOut : buffer utilisé pour l'écriture du fichier aFileOut
 * @return 0 si la décompression n'a pas généré d'erreur..
 */
int decompression_LZW(FILE *aFileIn, Buffer *aBufferIn, FILE *aFileOut, Buffer *aBufferOut);
