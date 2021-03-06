#include <types.h>
#include <stdio.h>

#define BUFFER_LENGTH 1024

/* Structure d'un buffer utilisé pour le lecture/écriture */
typedef struct s_buffer {
	char* content; /* Contenu du buffer*/
	char* courant; /* Pointeur sur élément courant du buffer */
	int significatif; /* nombre de bit de poids faibles de courant significatif */
	int longeur; /* Longuer de content significative */
} Buffer;


/**
 * bOpen
 * Ouvre un fichier en mode binaire avec les flags données. Le flag "b" ne doit pas être donné.
 * Deux buffers sont initialisés, un pour la lecture et un pour l'écriture dans le fichier.
 * @param aFileName : fichier à ouvrir
 * @param aFlags : flags d'ouvertures
 * @return structure FILE*
 */
FILE* bOpen(char* aFileName, char* aFlags);

/**
 * bMakeBuffer
 * Créé un buffer qui peut être utilisé pour les fonction de lecture ou d'écriture
 * @return structure Buffer*
 */
Buffer* bMakeBuffer();

/**
 * bClose
 * Ecriture de tout le contenu du buffer d'écriture passé en paramètre dans le fichier puis fermeture de ce fichier
 * @param aFile : fichier ouvert en mode binaire à utiliser
 * @param aBuffer : Buffer d'écriture
 */
void bClose(FILE* aFile, Buffer* aBuffers);

/**
 * bCloseBuffer
 * Ferme un Buffer en libérant la mémoire qui lui a été alloué
 * @param aBuffer : Buffer a fermer
 */
void bCloseBuffer(Buffer *aBuffer);

/**
 * bfeof
 * retourne 1 si la fin du fichier est atteinte et que le buffer de lecture est vide. 0 sinon.
 * @param aFile : fichier
 * @param aBuffer : buffer de lecture
 * @param aBits : nombre de bits que l'on souhaitera lire
 * @return 1 si la fin du fichier est atteinte et que le buffer de lecture est vide ou contient du bourrage. 0 sinon.
 */
int bfeof(FILE* aFile, Buffer* aBuffer, int aBits);

/**
 * bflush
 * Vide le buffer d'écriture aBuffer dans le fichier aFile
 * Si le dernier octet du buffer ne contient pas 8 bits significatif, l'octet n'est pas écrit
 * et la valeur est reportée dans aBuffer qui a été vidé.
 * @param aFile : fichier de sortie
 * @param aBuffer : buffer d'écriture
 */
void bFlush(FILE* aFile, Buffer* aBuffer);

/**
 * bflush_force
 * Vide le buffer d'écriture aBuffer dans le fichier aFile
 * Si le dernier octet du buffer ne contient pas 8 bits significatif, l'octet est quand même écrit
 * @param aFile : fichier de sortie
 * @param aBuffer : buffer d'écriture
 */
void bFlush_force(FILE* aFile, Buffer* aBuffer);

/**
 * bRead
 * lit aBits bits du fichie aFile en mode binaire et retourne la valeur entière associée. aBits doit être inférieur à sizeof(Code)*8.
 * @param aFile : fichier à lire
 * @param aBits : nombre de bits à lire
 * @param aBuffer : buffer de lecture
 * @return Code lu
 */
Code bRead(FILE* aFile, int aBits, Buffer* aBuffer);


/**
 * bWrite
 * Ecrit en mode binaire dans le fichier aFile le code aCode en le complétant avec des 0 afin d'obtenir un code sur aBits.
 * @param aFile : fichier dans lequel écrire
 * @param aBits : nombre de bits à écrire
 * @param aCode : Code à écrire
 * @param aBuffer : buffer d'écriture du fichier
 */
void bWrite(FILE* aFile, int aBits, Code aCode, Buffer* aBuffer);
