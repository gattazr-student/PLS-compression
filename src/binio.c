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
 * @return structure FILE*. NULL est retourné si le fichier n'a pas pu être ouvert.
 */
FILE* bOpen(char* aFileName, char* aFlags){
	FILE* wFile;
	char wFlags[10];

	strcpy(wFlags, aFlags);
	strcat(wFlags, "b");
	wFile = fopen(aFileName, aFlags);

	return wFile;
}

/**
 * bMakeBuffer
 * Créé un buffer qui peut être utilisé pour les fonction de lecture ou d'écriture
 * @return structure Buffer*
 */
Buffer* bMakeBuffer(){
	Buffer* wBuffer;
	wBuffer = malloc(sizeof(Buffer));
	wBuffer->content = calloc(BUFFER_LENGTH, sizeof(char)); /* Buffer de lecture */
	wBuffer->courant = wBuffer->content;
	wBuffer->significatif = 8;
	wBuffer->longeur = 0;
	return wBuffer;
}


/**
 * bClose
 * Fermeture du fichier aFile avec écriture du buffer passé en paramètre si non NULL.
 * @param aFile : fichier ouvert en mode binaire à utiliser
 * @param aBuffer : Buffer d'écriture
 */
void bClose(FILE* aFile, Buffer* aBuffer){
	if(aBuffer != NULL){
		/* Vide le buffer dans le fichier */
		bFlush_force(aFile, aBuffer);
	}

	if(aFile != NULL){
		fclose(aFile);
	}
}

/**
 * bCloseBuffer
 * Ferme un Buffer en libérant la mémoire qui lui a été alloué
 * @param aBuffer : Buffer a fermer
 */
void bCloseBuffer(Buffer *aBuffer){
	if(aBuffer != NULL){
		free(aBuffer->content);
		free(aBuffer);
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
	int wLength;
	char wLast = '\0';
	if (aFile != NULL && aBuffer != NULL) {
		if(aBuffer->content != NULL && aBuffer->longeur > 0){
			wLength = aBuffer->longeur + 1;
			aBuffer->longeur = 0;
			/* Ne copie pas le dernier octet si incomplet */
			if(aBuffer->significatif > 0){
				wLast =  *(aBuffer->courant); /* Sauvegarde le dernier octet */
				wLength--;
			}
			fwrite(aBuffer->content, sizeof(char), wLength, aFile);

			/* Sauvegarde le dernier octet sauvegardé */
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
	if(aBuffer->significatif > 0 && aBuffer->significatif < 8 ){
		fwrite(aBuffer->content, sizeof(char), 1, aFile);
	}
	aBuffer->content[0] = 0;
	aBuffer->significatif = 8;
}

/**
 * bfeof
 * retourne 1 si la fin du fichier est atteinte et que le buffer de lecture est vide. 0 sinon.
 * @param aFile : fichier
 * @param aBuffer : buffer de lecture
 * @param aBits : nombre de bits que l'on souhaitera lire
 * @return 1 si la fin du fichier est atteinte et que le buffer de lecture est vide ou contient du bourrage. 0 sinon.
 */
int bfeof(FILE* aFile, Buffer* aBuffer, int aBits){
	/* Si moins de aBits restant dans buffer et feof atteint, tester les aBits restant dans aBuffer. Si = 0, alors bourrage */
	int wRestant, wTraite;

	wTraite = aBuffer->courant - aBuffer->content;

	if(aFile != NULL && aBuffer != NULL){
		/* Si fin du fichier et fin du buffer atteint */
		if( feof(aFile) != 0) {
			wRestant = (aBuffer->longeur - wTraite)*8 - (8 - aBuffer->significatif);
			if(wRestant < aBits){
				return 1;
			}
		}
	}
	return 0;
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
	Code wResult;
	unsigned char wTemp;
	char wMask;
	int wDisponible, wBitsLus;
	int wRead, wRest;
	int wLengthToCopy;

	/* Calcul du nombre de bits disponible dans le buffer */
	wDisponible = (aBuffer->longeur-(aBuffer->courant-aBuffer->content))*8 - (8 - aBuffer->significatif);

	/* Le buffer ne contient pas sufisament de bits pour faire la lecture complète */
	if(wDisponible < aBits){

		/* copie les derniers octets non lus du buffer avant de le remplir a nouveau */
		wLengthToCopy = aBuffer->longeur - (aBuffer->courant - aBuffer->content);
		if(wLengthToCopy > 0){
			memcpy(aBuffer->content, aBuffer->courant, wLengthToCopy );
		}

		/* Lit entre 0 et BUFFER_LENGTH-wLengthToCopy octets */
		wRead = fread(aBuffer->content + wLengthToCopy, sizeof(char), BUFFER_LENGTH - wLengthToCopy, aFile);
		aBuffer->courant = aBuffer->content;
		aBuffer->longeur = wRead + wLengthToCopy;

		/* Calcul a nouveau du nombre de bits disponible dans le buffer */
		wDisponible = wRead*8 - (8 - aBuffer->significatif);

		/* Le buffer ne contient toujours pas sufisament de bits ou fin du fichier lus une deuxième fois */
		if(wRead == 0 || wDisponible < aBits){
			/* exit et retourne une erreur car fichier mal formé */
			fprintf(stderr, "Unexpected EOF reached\n");
			exit(1);
		}
	}
	wBitsLus = 0;
	wResult = 0;

	/* Il y a pas de bits libre dans courant */
	if(aBuffer->significatif == 0){
		aBuffer->courant++;
		aBuffer->significatif = 8;
	/* Si il y en a moins de 8 */
	} else if(aBuffer->significatif < 8){
		/* Le nombre de bits demandé est plus grand que ceux restant sur courant */
		if(aBits > aBuffer->significatif){
			mask_faible(aBuffer->significatif, &wMask);
			wResult = (*(aBuffer->courant) & wMask) << (aBits - aBuffer->significatif);
			wBitsLus = aBuffer->significatif;
			aBuffer->courant++; /* Fait avancer courant */
			aBuffer->significatif = 8;
		}
	}

	while(wBitsLus + 8 <= aBits){
		/* Place l'octet dans un code pour assurer que le décalage soit correct */
		wTemp = *(aBuffer->courant);
		wBitsLus += 8;
		wResult = wResult | (wTemp << (aBits - wBitsLus));
		aBuffer->courant++;
	}

	/* Il reste entre 0 et 7 bits à lire */
	wRest = aBits - wBitsLus;
	if(wRest > 0){
		mask_fort(wRest, &wMask);
		wTemp = *(aBuffer->courant) & wMask;
		wResult |= (wTemp >> (aBuffer->significatif - wRest));
		aBuffer->significatif = aBuffer->significatif - wRest;
	}

	#ifdef DEBUG_BINIO
		fprintf(stderr, "%d : %d\n", wResult, aBits);
	#endif

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
	/* 1) Vérifier si il y a sufisamment de bits libre dans le buffer d'écriture pour y écrire aCode */
	/* 1') Si il n'y en a pas sufisamnt, bflush le buffer */
	/* 2) Ecrire la valeur aCode dans courant, avance courant si nécessaire */
	char wMask;
	Code wTemp;
	int wBitsEcrits;
	int wRest;

	/* Vide le buffer si il n'est pas sufisament grand pour contenir le code */
	if((BUFFER_LENGTH - (aBuffer->courant - aBuffer->content))*8 < aBits){
		bFlush(aFile, aBuffer);
	}


	wBitsEcrits = 0;

	/* Il y a pas de bits libre dans courant */
	if(aBuffer->significatif == 0){
		aBuffer->courant++;
		aBuffer->longeur++;
		aBuffer->significatif = 8;
	/* Si il y en a moins de 8 */
	} else if(aBuffer->significatif < 8){
		/* Il y a plus de bits à écrire que de bits dans le courant*/
		if(aBits > aBuffer->significatif){
			wTemp = aCode >> (aBits - aBuffer->significatif);
			wBitsEcrits = aBuffer->significatif;
			*(aBuffer->courant) |= wTemp;
			aBuffer->courant++;
			aBuffer->longeur++;
			aBuffer->significatif = 8;
		}
	}

	mask_faible(8, &wMask);
	while(wBitsEcrits + 8 <= aBits){
		/* Place l'octet dans un code pour assurer que le décalage soit correct */
		wBitsEcrits += 8;
		wTemp = aCode >> (aBits - wBitsEcrits);
		*(aBuffer->courant) = (wTemp & wMask);
		aBuffer->courant++;
		aBuffer->longeur++;
	}

	/* Il reste entre 0 et 7 bits à écrire */
	wRest = aBits - wBitsEcrits;
	if(wRest > 0){
		mask_faible(wRest, &wMask);
		wTemp = aCode & wMask;
		*(aBuffer->courant) = wTemp << (aBuffer->significatif - wRest);
		aBuffer->significatif = aBuffer->significatif - wRest;
	}

	#ifdef DEBUG_BINIO
		fprintf(stderr, "%d : %d\n", aCode, aBits);
	#endif
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
