#include <binio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int aArgc, char** aArgv){
	FILE *wFile, *wFileOut;
	Buffer *wBuffers, *wBuffersOut;
	char *filenameOut, *filenameOutVerif;
	Code wCode;
	
	if (aArgc==3){
		if(atoi(aArgv[2])>=9 && atoi(aArgv[2])<=16){	
			wBuffers = bMakeBuffer();
			wBuffersOut = bMakeBuffer();
					
			filenameOut = malloc (strlen (aArgv[1]) + 1 + strlen (aArgv[2]) + 1);
			filenameOutVerif = malloc (strlen (aArgv[1]) + 3);
			if (filenameOut != NULL && filenameOutVerif != NULL) {
				strcpy (filenameOut, aArgv[1]);
				strcat (filenameOut, "_");
				strcat (filenameOut, aArgv[2]);
				
				strcpy (filenameOutVerif, aArgv[1]);
				strcat (filenameOutVerif, "_8");					
				 
				wFile = bOpen(aArgv[1], "r");
				wFileOut = bOpen(filenameOut, "w");
				
				while(bfeof(wFile, wBuffers, 8) == 0){
					wCode = bRead(wFile, 8, wBuffers);
					printf("%c",wCode);
					bWrite(wFileOut, atoi(aArgv[2]), wCode, wBuffersOut);
				}
				bClose(wFile, wBuffers);
				bClose(wFileOut, wBuffersOut);
				
				wBuffers = bMakeBuffer();
				wBuffersOut = bMakeBuffer();
			
				wFile = bOpen(filenameOut, "r");
				wFileOut = bOpen(filenameOutVerif, "w");
				
				while(bfeof(wFile, wBuffers, atoi(aArgv[2])) == 0){
					wCode = bRead(wFile, atoi(aArgv[2]), wBuffers);
					bWrite(wFileOut, 8, wCode, wBuffersOut);
				}
				bClose(wFile, wBuffers);
				bClose(wFileOut, wBuffersOut);
				free (filenameOut);
				free (filenameOutVerif);
			}
		}
		
	}else{
		printf("Utilisation:\n");
		printf("\targ1 : path du fichier d'entrée\n");
		printf("\targ2 : nombre de bits compris entre 9 et 16\n");
	}
	return 0;
}
