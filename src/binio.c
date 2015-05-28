#include <types.h>
#include <binio.h>
#include <stdlib.h>
#include <string.h>

/* Fonctions utilisés uniquement en interne */
int mask_faible(int aBits, char *aMask);
int mask_fort(int aBits, char *aMask);

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
	aBuffers[0].significatif = 8;
	aBuffers[1].content = calloc(BUFFER_LENGTH, sizeof(char)); /* Buffer d'écriture */
	aBuffers[1].courant = aBuffers[1].content;
	aBuffers[0].significatif = 0;
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
	Code wResult, wTemp;
	char wMask;
	int wDisponible, wBitsLus;
	int wRead;

	/* Calcul du nombre de bits disponible dans le buffer */
	wDisponible = strlen(aBuffer->courant)*8 - (8 - aBuffer->significatif);

	/* Le buffer ne contient pas sufisament de bits */
	if(wDisponible < aBits){

		/* Déplace le pointeur avec un offset de -strlen(aBuffer->courant) bytes */
		if(fseek(aFile, -strlen(aBuffer->courant), SEEK_CUR) != 0){
			fprintf(stderr, "Error while reading the file\n");
			exit(1);
		}

		/* Lit entre 0 et BUFFER_LENGTH octets */
		wRead = fread(aBuffer->content, sizeof(char), BUFFER_LENGTH, aFile);

		/* Calcul a nouveau du nombre de bits disponible dans le buffer */
		wDisponible = strlen(aBuffer->courant)*8 - (8 - aBuffer->significatif);

		/* Le buffer ne contient toujours pas sufisament de bits ou fin du fichier lus une deuxième fois */
		if(wRead == 0 || wDisponible < aBits){
			/* exit et retourne une erreur car fichier mal formé */
			fprintf(stderr, "Unexpected EOF found\n");
			exit(1);
		}
	}
	wBitsLus = 0;
	wResult = 0;
	if(aBuffer->significatif != 8){
		/* Le nombre de bits demandé est plus grand que ceux restant sur courant */
		if(aBits > aBuffer->significatif){
			mask_faible(aBuffer->significatif, &wMask);
			wResult = (*(aBuffer->courant) & wMask) << (aBits - aBuffer->significatif);
			wBitsLus = aBuffer->significatif;
			aBuffer->courant++; /* Fait avancer courant */
			aBuffer->significatif = 8;
		/* Sinon */
		}else{
			mask_faible(aBits, &wMask);
			wResult = (*(aBuffer->courant) & wMask);
			if(aBuffer->significatif - aBits > 0){
				aBuffer->significatif = aBuffer->significatif - aBits;
			}else{
				aBuffer->courant++; /* Fait avancer courant */
				aBuffer->significatif = 8;
			}
			return 0;
		}
	}

	while(wBitsLus + 8 <= aBits){
		/* Place l'octet dans un code pour assurer que le décalage soit correct */
		wTemp = *(aBuffer->courant);
		wBitsLus += 8;
		wResult = wResult | (wTemp << (aBits - wBitsLus));
		aBuffer->courant++;
	}

	/* si il reste entre 1 et 7 bits à lire */
	if(wBitsLus != aBits){
		mask_fort(aBits - wBitsLus, &wMask);
		wTemp = *(aBuffer->courant);
		wResult = wResult | (wTemp & wMask);
		aBuffer->significatif = 8 - (aBits - wBitsLus);
	}

	return wResult;
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
	/* 1) Vérifier si il y a des bits libre dans le stream. */
	/* 1') Si il n'y en a pas, bflush */
	char wMask;
	Code wTemp;
	int wBitsEcrits;
	int wRest;

	if((BUFFER_LENGTH - strlen(aBuffer->content))*8 < aBits){
		bFlush(aFile, aBuffer);
	}


	wBitsEcrits = 0;
	if(aBuffer->significatif != 0){
		/* Il y a plus de bits à lire que de bits dans le courant*/
		if(aBits > 8 - aBuffer->significatif){
			wBitsEcrits = (8 - aBuffer->significatif);
			wTemp = aCode >> (aBits - wBitsEcrits);
			*(aBuffer->courant) |= wTemp;
			aBuffer->courant++;
			aBuffer->significatif = 0;
		/* Sinon */
		}else{
			wBitsEcrits = aBits;
			wTemp = aCode >> (8 - wBitsEcrits - aBuffer->significatif);
			*(aBuffer->courant) |= wTemp;
			aBuffer->courant++;
			aBuffer->significatif += aBits;
			return;
		}
	}

	mask_faible(8, &wMask);
	while(wBitsEcrits + 8 <= aBits){
		/* Place l'octet dans un code pour assurer que le décalage soit correct */
		wBitsEcrits += 8;
		wTemp = aCode >> (aBits - wBitsEcrits);
		*(aBuffer->courant) = (wTemp & wMask);
		aBuffer->courant++;
	}

	/* Si il reste entre 1 et 7 bits à écrire */
	wRest = aBits - wBitsEcrits;
	if(wRest != 0){
		mask_faible(wRest, &wMask);
		wTemp = aCode & wMask;
		*(aBuffer->courant) = wTemp << (8 - wRest);
		aBuffer->significatif = wRest;
	}

}

/**
 * mask_faible
 * Créé un char avec des 0 sur les poids forts et des 1 sur les aBits de poids faibles
 * @param aBits : nombre de bits à 1
 * @param aMask : masque produit
 * @return -1 si une erreur. 0 sinon
 */
int mask_faible(int aBits, char *aMask){
	if(aBits > sizeof(char)*8){
		return -1;
	}

	*aMask = (1 << aBits) - 1;
	return 0;
}

/**
 * mask_fort
 * Créé un char avec des 0 sur les bits de poids faibles et des 1 sur les aBits de poids forts
 * @param aBits : nombre de bits à 1
 * @param aMask : masque produit
 * @return -1 si une erreur. 0 sinon
 */
int mask_fort (int aBits, char *aMask){
	if(mask_faible(aBits, aMask) != 0){
		return -1;
	}
	*aMask = *aMask << (sizeof(char)*8 - aBits);
	return 0;
}
