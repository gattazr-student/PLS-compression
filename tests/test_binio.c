#include <binio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int aArgc, char** aArgv){
	FILE *wFileIn, *wFileOut;
	Buffer *wBufferIn, *wBufferOut;
	char *wFilenameOut;
	Code wCode;
	int wRand;
	int wSignificatif;

	srand(time(NULL));

	if (aArgc==2){
		wBufferIn = bMakeBuffer();
		wBufferOut = bMakeBuffer();

		wFilenameOut = malloc(strlen(aArgv[1]) + 3);
		if (wFilenameOut != NULL){

			strcpy(wFilenameOut, aArgv[1]);
			strcat(wFilenameOut, "_8");

			wFileIn = bOpen(aArgv[1], "r");
			wFileOut = bOpen(wFilenameOut, "w");
			wRand = rand() % 8 + 8;

			while(bfeof(wFileIn, wBufferIn, wRand) == 0){
				wCode = bRead(wFileIn, wRand, wBufferIn);
				bWrite(wFileOut, wRand, wCode, wBufferOut);
				wRand = rand() % 8 + 8;
			}

			/* On termine la lecture pour assurer avoir tout récupérer */
			wSignificatif = wBufferIn->significatif;
			while(bfeof(wFileIn, wBufferIn, wSignificatif) == 0){
				wCode = bRead(wFileIn, wSignificatif, wBufferIn);
				bWrite(wFileOut, wSignificatif, wCode, wBufferOut);
				wSignificatif = wBufferIn->significatif;
			}
			bClose(wFileIn, wBufferIn);
			bClose(wFileOut, wBufferOut);
			bCloseBuffer(wBufferIn);
			bCloseBuffer(wBufferOut);
			free (wFilenameOut);

		}

	}else{
		printf("Utilisation:\n");
		printf("\targ1 : path du fichier d'entrée\n");
	}
	return 0;
}
