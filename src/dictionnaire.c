
#include <dictionnaire.h>
#include <stdio.h>

Dict * dictionnaire = NULL;

/**
 * initialiser
 * initialise le dictionnaire avec toutes les monoséquences et initialise le nombre d'éléments nbElements à 259 (256+3 "spéciaux")
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
	
	dictionnaire->nbElements = 259;
	dictionnaire->tailleDico = INI_TAB_SIZE;
}


/**
 * inserer
 * Insere le Code aMono préfixé par le code aPrefixe dans le dictionnaire
 * @param aPrefixe :
 * @param aMono :
 * @return Code de la nouvelle séquence
 */
Code inserer (Code aPrefixe, Code aMono){

	Code *aCode = NULL;
	Arbre * aKid = NULL;
	Arbre * aNew = NULL;
	Arbre ** aTmp = NULL;
	int i = 0;
	
	/* Cas où le code de aPrefixe.aMono existe déjà -> on retourne le code existant */
	if(existe_seq(aPrefixe, aMono, aCode) == 0){
		printf("Insertion impossible, le code existe déjà\n");
		return *aCode;
	}
	/* Cas où le code de aPrefixe.aMono n'existe pas */
	else {
		aKid = dictionnaire->ids[aPrefixe-FIRST_AVAILABLE]->enfant;

		/* On va jusqu'au dernier frère */
		while(aKid->frere != NULL){
			aKid->frere = aKid->frere->frere;
		}

		dictionnaire->nbElements++;
		
		/* Vérification taille du tableau -> si on dépasse, on double la taille de ids */
		if(dictionnaire->nbElements > dictionnaire->tailleDico){
			aTmp = realloc(dictionnaire->ids, dictionnaire->tailleDico*2);
			if (aTmp != NULL){
				dictionnaire->ids = aTmp;
			}
			else{
				perror("Reallocation failed\n");
				exit(EXIT_FAILURE);
			}
		}	
		
		aNew->valeur = aMono;
		aNew->parent = dictionnaire->ids[aPrefixe-FIRST_AVAILABLE];
		aNew->frere = aKid;
		aKid = aNew;
		aNew->enfant = NULL;
		
		i = dictionnaire->nbElements;
		aNew->id = &(dictionnaire->ids[i]);
		dictionnaire->ids[i] = aNew;
		
		return i;
	}


}



/**
 * existe
 * retourne 0 ou 1 signifiant si le Code aPrefixe.aMono existe.
 * @param aPrefixe : le code du préfixe de la séquence
 * @param aMono : le code du mono de la séquence
 * @param aCode : pointe sur le code aPrefixe.aMono s'il existe
 * @return 0 si le code existe. 1 sinon.
 */
int existe_seq (Code aPrefixe, Code aMono, Code *aCode){

	Arbre* aKid = NULL;
	Arbre ** aCodekid = NULL;
	aKid = dictionnaire->ids[aPrefixe-FIRST_AVAILABLE]->enfant;
	aCodekid = dictionnaire->ids[aPrefixe-FIRST_AVAILABLE]->enfant->id;

	/* Cas où le code aPrefixe existe */
	if(existe_code(aPrefixe) == 0){
		if(aKid->valeur == aMono){
			*aCode = aCodekid-dictionnaire->ids+FIRST_AVAILABLE;
			return 0;
		}
		/* On regarde tous les frères */		
		else {
			while(aKid->frere != NULL){
				if(aKid->frere->valeur == aMono){
					*aCode = aCodekid-dictionnaire->ids+FIRST_AVAILABLE;
					return 0;
				} else {
					aKid->frere = aKid->frere->frere;
				}
			}
			return 1;		
		}	
	/* Cas où le code aPrefixe n'existe pas */	
	} else {
		return 1;
	}
}
  
/**
 * existe_code
 * retourne 0 ou 1 signifiant si le Code aCode existe.
 * @param aCode : le code dont on vérifie l'existence
 * @return 0 si le code existe. 1 sinon.
 */
int existe_code (Code aCode){

	if(aCode < (*dictionnaire).nbElements){
		return 0;
	} else {
		return 1;
	}

}

