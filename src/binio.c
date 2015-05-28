#include <types.h>
#include <binio.h>
#include <stdlib.h>
#include <string.h>

/**
 * bOpen
 * Ouvre un fichier en mode binaire avec les flags données. Le flag "b" ne doit pas être donné.
 * Deux buffers sont initialisés, un pour la lecture et un pour l'écriture dans le fichier.
 * @param aFileName : fichier à ouvrir
 * @param aFlags : flags d'ouvertures
 * @param aBuffers : buffers de lecture et d'écriture initialisé
 * @return structure FILE*
 */
FILE* bOpen(char* aFileName, char* aFlags, Buffer* aBuffers){
	FILE* wFile;
	wFile = fopen(aFileName, strcat(aFlags, "b"));
	if(wFile == NULL){
		fprintf(stderr, "The specified file (%s) does not exist\n", aFileName);
		exit(1);
	}
	/* Allocation des buffers */
	aBuffers = malloc(sizeof(Buffer)*2);
	aBuffers[0].content = calloc(BUFFER_LENGTH, sizeof(char)); /* Buffer de lecture */
	aBuffers[0].courant = aBuffers[0].content;
	aBuffers[1].content = calloc(BUFFER_LENGTH, sizeof(char)); /* Buffer d'écriture */
	aBuffers[1].courant = aBuffers[1].content;
	return wFile;
}

/**
 * bClose
 * Ecriture de tout le contenu du buffer d'écriture passé en paramètre dans le fichier puis fermeture de ce fichier
 * @param aFile : fichier ouvert en mode binaire à utiliser
 * @param aBuffers : Tableau de buffers contanant le buffer de lecture et d'écriture du fichier
 */
void bClose(FILE* aFile, Buffer* aBuffers){
	if(aBuffers != NULL){
		/* Destruction du buffer de lecture */
		free(aBuffers[0].content);
		/* Ecrit le contenu du buffer d'écriture dans le fichier */
		bFlush_force(aFile, &(aBuffers[1]));
		/* Destruction du buffer d'écriture */
		free(aBuffers[1].content);
		free(aBuffers);
	}

	if(aFile != NULL){
		fclose(aFile);
	}
}


/**
 * bflush
 * Vide le buffer d'écriture aBuffer dans le fichier aFile
 * Si le dernier octet du buffer ne contient pas 8 bits significatif, l'octet n'est pas écrit
 * et la valeur est reportée dans aBuffer qui a été vidé.
 * @param aFile : fichier de sortie
 * @param aBuffer : buffer d'écriture
 */
void bFlush(FILE* aFile, Buffer* aBuffer){
	int wI, wLength;
	char wLast = '\0';
	if (aFile != NULL && aBuffer != NULL) {
		if(aBuffer->content != NULL){
			wLength = strlen(aBuffer->content);
			/* Ne copie pas le dernier octet si incomplet */
			if(aBuffer->significatif < 8){
				wLength = wLength - 1;
				wLast =  *(aBuffer->courant); /* Sauvegarde le dernier octet */
			}
			fwrite(aBuffer->content, sizeof(char), wLength, aFile);

			/* Réinitialise le buffer */
			for(wI = 1; wI <wLength; wI++){
				aBuffer->content[wI] = '\0';
			}
			aBuffer->content[0] = wLast;
			aBuffer->courant = aBuffer->content;
		}
	}
}

/**
 * bflush_force
 * Vide le buffer d'écriture aBuffer dans le fichier aFile
 * Si le dernier octet du buffer ne contient pas 8 bits significatif, l'octet est quand même écrit
 * @param aFile : fichier de sortie
 * @param aBuffer : buffer d'écriture
 */
void bFlush_force(FILE* aFile, Buffer* aBuffer){
	bFlush(aFile, aBuffer);
	if(aBuffer->content[0] != '\0'){
		fwrite(aBuffer->content, sizeof(char), 1, aFile);
	}
	aBuffer->content[0] = 0;
	aBuffer->significatif = 0;
}

/**
 * bRead
 * lit aBits bits du fichie aFile en mode binaire et retourne la valeur entière associée. aBits doit être inférieur à sizeof(Code)*8.
 * @param aFile : fichier à lire
 * @param aBits : nombre de bits à lire
 * @param aBuffer : buffer de lecture
 * @return Code lu
 */
Code bRead(FILE* aFile, int aBits, Buffer* aBuffer){
	/* 1) Vérifier si aBits disponile dans buffer */
	/* 1') Si pas assez : réculer aFile du nombre de bits dans buffer et lire BUFFER_LENGTH bytes ou jusqu'a fin du fichier si pas autant de disponible */
	/* 2) Construire la valeur que l'on veut lire (avancer courant si nécessaire) */

	/* 	Réculer le pointeur avec fgetpos() et fseek()
		Lire le fichier binaire avec fread
		Lire le buffer avec des masques bits a bits */

	return 0;
}


/**
 * bWrite
 * Ecrit en mode binaire dans le fichier aFile le code aCode en le complétant avec des 0 afin d'obtenir un code sur aBits.
 * @param aFile : fichier dans lequel écrire
 * @param aBits : nombre de bits à écrire
 * @param aCode : Code à écrire
 * @param aBuffer : buffer d'écriture du fichier
 */
void bWrite(FILE* aFile, int aBits, Code aCode, Buffer* aBuffer){
	/* 1) Vérifier si il y a a bits libre dans le stream. */
	/* 1') Si il n'y en a pas, bflush */

}
