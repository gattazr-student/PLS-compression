#include <stdio.h>
#include <stdlib.h>
#include <binio.h>
#include <getopt.h>
#include <decompression.h>
#include <compression.h>
#include <rle.h>


void printHelp(FILE* aFileOut);

/* 	la gestion des paramètres se fait selon le modèle GCC.
	Le parsing des paramètres est très inspiré de la documentation GCC
 	http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html */
int main(int aArgc, char** aArgv){

	int wParam, wOption_Index;
	Buffer *wBufferIn, *wBufferOut;
	FILE *wFileIn, *wFileOut;
	FILE *wFileTemp;
	char *wFileInName;
	int wFunction;
	int wRLE;
	const char *wOption_string_ = "c::d::hr"; /* option_params pour getopt_long */

	/* Structure des paramètres pour getopt_long */
	static struct option wOption_long[] = {
		{"compress", optional_argument, NULL, 'c'},
		{"decompress", optional_argument, NULL, 'd'},
		{"rle", no_argument, NULL, 'r'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};


	/* Gestion des paramètres */
	wOption_Index = 0;
	wFileIn = NULL;
	wBufferIn = NULL;
	wFunction = 0;
	wRLE = 0;
	while (1){

		wParam = getopt_long (aArgc, aArgv, wOption_string_, wOption_long, &wOption_Index);

		/* Detection de la fin des paramètres */
		if (wParam == -1){
			break;
		}

		switch (wParam) {
			case 'c':
			case 'd':
				/* Compress or decompress */
				if(wFunction > 0){
					fprintf(stderr, "LZW: cannot compress and decompress a file at the same time\n");
					exit(1);
				}
				wFunction = wParam - 'c' + 1; /* wFunction = 1 ou 2 */
				wFileInName = optarg;
				break;
			case 'h':
				printHelp(stdout);
				exit(0);
				break;
			case 'r':
				wRLE = 1;
				break;

			default:
				/* Un paramètre non géré a été trouvé */
				printHelp(stderr);
				exit(1);
		}
	}

	if(wFunction == 0){
		fprintf(stderr, "LZW: the flag -d or -c must be set\n");
		printHelp(stderr);
		exit(1);
	}

	/* Ouverture du fichier d'entré et du buffer */
	if(wFileInName != NULL){
		wFileIn = bOpen(wFileInName, "r");
		if(wFileIn == NULL){
			fprintf(stderr, "LZW: cannot open the file %s\n", wFileInName);
			exit(1);
		}
	}else{
		wFileIn = stdin;
	}
	wBufferIn = bMakeBuffer();

	/* Ouverture d'un fichier temporaire si le flag RLE est présent */
	if(wRLE == 1){
		wFileTemp = bOpen("/tmp/lzw_temp", "w+b");
		if(wFileTemp == NULL){
			fprintf(stderr, "LZW: cannot open the file %s\n", wFileInName);
			exit(1);
		}
	}


	wFileOut = stdout;
	wBufferOut = bMakeBuffer();
	if(wFunction == 1){
		/* Pré-traitement RLE */
		if(wRLE == 1){
			coderRle(wFileIn, wFileTemp); /* Pré-traitement */
			fclose(wFileIn); /* Fermeture du fichier in */
			rewind(wFileTemp); /* Replace wFileTemp au début du fichier */
			wFileIn = wFileTemp; /* Remplacer wFileIn */
		}
		/* compression en cours */
		compression_LZW(wFileIn, wBufferIn, wFileOut, wBufferOut);
	}else if(wFunction == 2){
		/* Post-traitement RLE */
		if(wRLE == 1){
			wFileOut = wFileTemp;
		}
		/* décompression */
		decompression_LZW(wFileIn, wBufferIn, wFileOut, wBufferOut);
		/* Post-traitement RLE */
		if(wRLE == 1){
			fclose(wFileIn);
			bFlush_force(wFileOut, wBufferOut);
			wBufferOut = NULL;
			rewind(wFileTemp); /* Replace wFileTemp au début du fichier */
			decoderRle(wFileOut, stdout);
		}
	}
	/* fermeture des fichiers et des buffers */
	bClose(wFileIn, NULL);
	bClose(wFileOut, wBufferOut);
	bCloseBuffer(wBufferIn);
	bCloseBuffer(wBufferOut);

	return 0;
}

void printHelp(FILE* aFileOut){
	fprintf(aFileOut, "LZW, version 0.1\n\n");

	fprintf(aFileOut, "usage : LZM [-r] [-c<input_file | -d<input_file>]\n");
	fprintf(aFileOut, "\t-c<input_file>\t: compress the input file\n");
	fprintf(aFileOut, "\t-d<input_file>\t: decompress the input file\n");
	fprintf(aFileOut, "\t-r \t\t: activate the RLE pre/post treatment\n\n");
}
