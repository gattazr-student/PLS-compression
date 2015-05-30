
#include <stdint.h>
#include <stdlib.h>
#include <types.h>

#define FIRST_AVAILABLE 259
#define INI_TAB_SIZE 256

typedef struct _arbre Arbre;
typedef struct _dict Dict;

struct _arbre {
	char valeur;
	Arbre* enfant;
	Arbre* frere;
	Arbre* parent;
	Arbre** id;
};

struct _dict {
	int nbElements;		/* nombre d'élements présents dans le dictionnaire */
	int tailleDico;		/* dernier élément du dictionnaire dans lequel on peut écrire */
	Arbre** ids;		/* tableau de pointeur sur Arbre */
	Arbre* dico;		/* tableau d'arbres (1er étage) */
};


/**
 * initialiser
 * initialise le dictionnaire avec toutes les monoséquences
 */
void initialiser();

/**
 * inserer
 * Insere le Code aMono préfixé par le code aPrefixe dans le dictionnaire
 * @param aPrefixe :
 * @param aMono :
 * @return Code de la nouvelle séquence
 */
Code inserer (Code aPrefixe, Code aMono);

/**
 * codeVersSequence
 * Retourne la chaine de caractère lié au code passé en paramètre
 * la string a retourner doit avoir été malloc
 * remonter l'arbre en insérerant en queue et retourner la chaine de caractères à la fin
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
 * existe_code
 * retourne 0 ou 1 signifiant si le Code aCode existe.
 * @param aCode :
 * @return 0 si le code existe. 1 sinon.
 */
int existe_code (Code aCode);
