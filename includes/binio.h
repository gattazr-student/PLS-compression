
/**
 * bRead
 * lit aBits bits du fichie aFile en mode binaire et retourne la valeur entière associée. aBits doit être inférieur à sizeof(Code)*8.
 * @param aFile
 * @param aBits
 * @return
 */
Code bRead(FILE* aFile, int aBits);


/**
 * bWrite
 * Ecrit en mode binaire dans le fichier aFile le code aCode en le complétant avec des 0 afin d'obtenir un code sur aBits.
 * @param aFile
 * @param aBits
 * @param aCode
 * @return
 */
void bWrite(FILE* aFile, int aBits, Code aCode);
