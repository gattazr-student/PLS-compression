
/**
 * initialiser
 * initialise le dictionnaire avec toutes les monoséquences
 */
void initialiser();

/**
 * inserer
 * Insere le Code aMono préfixée par la code aPrefixe dans le dictionnaire
 * @param aPrefixe :
 * @param aMono :
 * @return Code de la nouvelle séquence
 */
Code inserer (Code aPrefixe, Code aMono);

/**
 * codeVersSequence
 * Retourne la chaine de caractère lié au code passé en paramètre
 * @param aCode :
 * @return
 */
char* codeVersSequence (Code aCode);

/**
 * sequenceVersCode
 * Retourne le code lié à une séquence
 * @param aSequence :
 * @param aLongeur :
 * @return
 */
Code sequenceVersCode (char* aSequence, int aLongueur);

/**
 * existe
 * retourne 0 ou 1 signifiant si le Code aPrefixe.aMono existe.
 * @param aPrefixe :
 * @param aMono :
 * @param aCode : pointe sur le code aPrefixe.aMono si il existe
 * @return 0 si le code existe. 1 sinon.
 */
int existe_seq (Code aPrefixe, Code aMono, Code *aCode);

/**
 * inserer
 * Insere le Code aMono préfixée par la code aPrefixe dans le dictionnaire
 * @param aCode :
 * @return 0 si le code existe. 1 sinon.
 */
int existe_code (Code aCode);
