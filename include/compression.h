/**
 * compression_LZW
 * Compresse les données du fichier aInput dans un fichier aOutput,
 * en utilisant l'algorithme de Lampel Ziv Welch.
 * @param aInput : fichier d'entrée contenant les données à compresser
 * @param aBuffersInput : Buffer lié au fichier d'entrée aInput
 * @param aOutput : fichier de sortie contenant les données compressées
 * @param aBuffersOutput : Buffer lié au fichier de sortie aOutput
 * @return 0 si pas d'erreur et 1 sinon.
 */
 int compression_LZW(FILE* aInput, Buffer* aBufferInput, FILE* aOutput, Buffer* aBufferOutput);
