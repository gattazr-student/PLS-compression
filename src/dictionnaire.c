
#include <dictionnaire.h>
#include <stdio.h>
#include <string.h>

Dict * dictionnaire = NULL;

/**
 * initialiser
 * initialise le dictionnaire avec toutes les monoséquences et initialise le nombre d'éléments nbElements à 258 (255+3 "spéciaux")
 */
void initialiser(){

	int i = 0;
	
	dictionnaire = malloc(sizeof(Dict));
	dictionnaire->ids = (Arbre**) calloc(256, sizeof(Arbre*));
	if (dictionnaire->ids == NULL){
		perror("Ids initialisation failed (calloc)\n");
		exit(EXIT_FAILURE);
	}
	
	dictionnaire->dico = (Arbre*) malloc(256*sizeof(Arbre));
	if (dictionnaire->dico == NULL){
		perror("Dico initialisation failed (malloc)\n");
		exit(EXIT_FAILURE);
	}
	
	for(i=0; i<256; i++){
		dictionnaire->dico[i].valeur = i;
	}
	
	for(i=0; i<256; i++){
		printf("%c ", (*dictionnaire).dico[i].valeur);
	}
	printf("\n");
	
	dictionnaire->nbElements = 258;
	dictionnaire->tailleDico = INI_TAB_SIZE;
}


/**
 * inserer
 * Insere le Code aMono préfixé par le code aPrefixe dans le dictionnaire
 * @param aPrefixe :
 * @param aMono :
 * @return Code de la nouvelle séquence ou -1 s'il s'agit d'un code spécial
 */
Code inserer (Code aPrefixe, Code aMono){

	Code *aCode = NULL;
	Arbre * aParent = NULL;
	Arbre * aNew = NULL;
	Arbre ** aTmp = NULL;
	int i = 0;
	
	/* Cas où le code de aPrefixe.aMono existe déjà -> on retourne le code existant */
	if(existe_seq(aPrefixe, aMono, aCode) == 0){
		printf("Insertion impossible, le code existe déjà\n");
		return *aCode;
	}
	
	if(existe_seq(aPrefixe, aMono, aCode) < 0){
		printf("Insertion impossible : valeur spéciale\n");
		return -1;
	}
	
	aNew = (Arbre*) malloc(sizeof(Arbre));
	
	/* Cas où le code de aPrefixe.aMono n'existe pas */
	if (aPrefixe < INI_TAB_SIZE){
		aParent = &(dictionnaire->dico[aPrefixe]);
	} else {
		aParent = dictionnaire->ids[aPrefixe-FIRST_AVAILABLE];
	}		

	i = ++dictionnaire->nbElements;
	
	/* Vérification taille du tableau -> si on dépasse, on double la taille de ids */
	if((dictionnaire->nbElements-FIRST_AVAILABLE) > dictionnaire->tailleDico){
		aTmp = realloc(dictionnaire->ids, dictionnaire->tailleDico*2);

		if (aTmp != NULL){
			dictionnaire->ids = aTmp;
		}
		else {
			perror("Reallocation failed\n");
			exit(EXIT_FAILURE);
		}
	}	

	aNew->valeur = aMono;
	aNew->frere = aParent->enfant;
	aNew->parent = aParent;
	aParent->enfant = aNew;
	aNew->enfant = NULL;
	/* Rajoute dans le tableau ids le nouveau noeud */
	dictionnaire->ids[i-FIRST_AVAILABLE] = aNew;
	aNew->id = &(dictionnaire->ids[i-FIRST_AVAILABLE]);
	
	/*printf("		BLABLA 5\n");
	printf("		BLABLA 6\n");
	printf("		aNew->parent : %p \n", aNew->parent);
	printf("		dictionnaire->ids[nbElements-FIRST_AVAILABLE] : %p \n", dictionnaire->ids[i-FIRST_AVAILABLE]);
	printf("		aNew->id : %p \n", aNew->id);
	printf("		dictionnaire->dico[aPrefixe].enfant : %p \n", dictionnaire->dico[aPrefixe].enfant);*/
	
	return i;
}

/**
 * inverserChaine
 * Retourne la chaine de caractère aSrc inversée
 * @param aSrc : la chaîne de caractères à inverser
 * @return la chaine de caractère wDest correspondant à la chaîne aSrc inversée
 */
char * inverserChaine (char * aSrc){
	char * wDest = NULL;
	int i;
	int j = 0;
	
	i = strlen(aSrc)-1;
	wDest = (char*)malloc((i+1)*sizeof(char));
	
	while(i>=0){
		wDest[j] = aSrc[i];
		j++;
		i--;
	}
	
	return wDest;
}


/**
 * codeVersSequence
 * Retourne la chaine de caractère liée au code passé en paramètre
 * la string à retourner doit avoir été allouée avec un malloc
 * remonter l'arbre en insérant en queue et inverser la chaine de caractères à la fin
 * @param aCode : le code à partir duquel on veut retrouver la séquence
 * @return la chaine de caractère liée au code passé en paramètre, null si le code n'existe pas
 */
char* codeVersSequence (Code aCode){

	char * wSequence = NULL;
	char * wTmp = NULL;
	Arbre * wDernier = NULL;
	char wCourant;
	int wLg = 1;
	
	/* Cas où le code à rechercher n'existe pas */
	if (existe_code(aCode) == 1){
		printf("Erreur codeVersSequence : le code n'existe pas\n");
		return NULL;
	}
	
	/* Si aCode = 256-7-8 (valeur spéciale) */
	if (aCode>255 && aCode<259){
		printf("Erreur codeVersSequence : valeur spéciale !\n");
		return NULL;
	}
	
	printf("	- codeVersSequence : wLg (longueur chaine) = %d\n", wLg);
	wSequence = (char*)malloc(sizeof(char));
	
	/* Si aCode < 256, le code se trouve dans dico */
	if (aCode < INI_TAB_SIZE){
		*wSequence = aCode;
		return wSequence;
	}
	
	/* Sinon : aCode >= 258. On récupère le code depuis ids en commençant par le dernier élément de la séquence */
	wDernier = dictionnaire->ids[aCode-FIRST_AVAILABLE];
	wCourant = wDernier->valeur;
	*wSequence = wCourant;
	printf("	codeVersSequence : wCourant = %c\n", wCourant);
	/* On remonte l'arbre jusqu'en haut */
	while(wDernier->parent != NULL){
		wDernier = wDernier->parent;
		wLg++;
		printf("	- codeVersSequence : wLg (longueur chaine) = %d\n", wLg);
		wTmp = realloc(wSequence, wLg);
		if (wTmp != NULL){
			wSequence = wTmp;
		}
		else {
			perror("codeVersSequence : reallocation failed\n");
			exit(EXIT_FAILURE);
		}
		wSequence[wLg-1] = wDernier->valeur;
		printf("	codeVersSequence : wCourant = %c\n", wSequence[wLg-1]);
	}
	
	wSequence[wLg] = '\0';
	/* On inverse la chaine */
	wSequence = inverserChaine(wSequence);
		
	return wSequence;
}

/**
 * existe
 * retourne 0 ou 1 signifiant si le Code aPrefixe.aMono existe.
 * @param aPrefixe : le code du préfixe de la séquence
 * @param aMono : le code du mono de la séquence
 * @param aCode : pointe sur le code aPrefixe.aMono s'il existe
 * @return 0 si le code existe. 1 sinon + valeurs spéciales (-1 si fin de contenu, -2 si incrémentation de la taille des codes, -3 si réinitialisation du dictionnaire).
 */
int existe_seq (Code aPrefixe, Code aMono, Code *aCode){

	Arbre* aKid = NULL;
	Arbre ** aCodekid = NULL;
	
	/* Cas où le préfixe n'existe pas : la séquence ne peut pas exister */
	if(existe_code(aPrefixe) == 1){
		return 1;
	}
	
	/* Vérif valeurs spéciales */
	if(aPrefixe>=INI_TAB_SIZE && aPrefixe<FIRST_AVAILABLE){
		return aPrefixe-FIRST_AVAILABLE;
	}
	
	/* Si aPrefix est un mono -> on cherche dans dico */
	if (aPrefixe < INI_TAB_SIZE){
		aKid = dictionnaire->dico[aPrefixe].enfant;
	}	

	/* Si aPrefix est une séquence -> on cherche dans ids */
	else {
		aKid = dictionnaire->ids[aPrefixe-FIRST_AVAILABLE]->enfant;
	}
	
	/* On parcourt les frères */
	while(aKid != NULL && aKid->valeur != aMono){
		aKid = aKid->frere;
	}
	
	/* Si aKid est null : on a parcouru tous les frères, sinon on a trouvé un frère dont la valeur est égale à aMono */
	if(aKid == NULL){
		return 1;
	}else{
		aCodekid = aKid->id;
		*aCode = aCodekid-(dictionnaire->ids)+FIRST_AVAILABLE;
		return 0;
	}
}
  
/**
 * existe_code
 * retourne 0 ou 1 signifiant si le Code aCode existe.
 * @param aCode : le code dont on vérifie l'existence
 * @return 0 si le code existe. 1 sinon.
 */
int existe_code (Code aCode){

	if(aCode <= (*dictionnaire).nbElements){
		return 0;
	} else {
		return 1;
	}

}

