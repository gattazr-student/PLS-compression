
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
	Code code;
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
 * inverserChaine
 * Inverse la chaine de caractère passé en paramètre.
 * @param aSrc : la chaîne de caractères à inverser
 * @param aLength : taille de la chaine
 */
void inverserChaine (char *aSrc, int aLength);

/**
 * codeVersSequence
 * Retourne la chaine de caractère liée au code passé en paramètre
 * la string à retourner doit avoir été allouée avec un malloc
 * remonter l'arbre en insérant en queue et inverser la chaine de caractères à la fin
 * @param aCode : le code à partir duquel on veut retrouver la séquence
 * @return la chaine de caractère liée au code passé en paramètre, null si le code n'existe pas
 */
char* codeVersSequence (Code aCode, int* wLg);

/**
 * sequenceVersCode
 * Retourne le code lié à une séquence
 * @param aSequence : la séquence à partir de laquelle on veut retrouver le code
 * @param aLongueur : longueur de la séquence
 * @return le code correspondant à la sequence aSequence
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

/**
 * liberer
 * libère les structures : ids, dico et le dictionnaire
 */
void liberer();

/**
 * isFull
 * Si le dictionnaire est plein, retourne 1. 0 sinon
 * @return 1 si le dictionnaire est plein. 0 sinon.
 */
int isFull();
