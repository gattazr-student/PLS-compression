#include <stdio.h>
#include <stdlib.h>
#include <binio.h>
#include <getopt.h>
#include <decompression.h>


void printHelp(FILE* aFileOut);

/* 	la gestion des paramètres se fait selon le modèle GCC.
	Le parsing des paramètres est très inspiré de la documentation GCC
 	http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html */
int main(int aArgc, char** aArgv){

	int wParam, wOption_Index;
	Buffer *wBufferIn, *wBufferOut;
	FILE *wFileIn, *wFileOut;
	int wFunction;
	const char *wOption_string_ = "c::d::h"; /* option_params pour getopt_long */

	/* Structure des paramètres pour getopt_long */
	static struct option wOption_long[] = {
		{"compress", optional_argument, NULL, 'c'},
		{"decompress", optional_argument, NULL, 'd'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};


	/* Gestion des paramètres */
	wOption_Index = 0;
	wFileIn = NULL;
	wBufferIn = NULL;
	wFunction = 0;
	while (1){

		wParam = getopt_long (aArgc, aArgv, wOption_string_, wOption_long, &wOption_Index);

		/* Detection de la fin des paramètres */
		if (wParam == -1){
			break;
		}

		switch (wParam) {
			case 'c':
			case 'd':
				/* bopen + free useless buffer */
				if(wFunction > 0){
					fprintf(stderr, "LZW: cannot compress and decompress a file at the same time\n");
					exit(1);
				}
				wFunction = wParam - 'c' + 1; /* wFunction = 1 ou 2 */
				if(optarg != NULL){
					wFileIn = bOpen(optarg, "r");
					if(wFileIn == NULL){
						fprintf(stderr, "LZW: cannot open the file %s\n", optarg);
						exit(1);
					}
				}else{
					wFileIn = stdin;
				}
				wBufferIn = bMakeBuffer();
				break;
			case 'h':
				printHelp(stdout);
				exit(0);
				break;

			default:
				/* Un paramètre non géré a été trouvé */
				printHelp(stderr);
				exit(1);
		}
	}

	if(wFileIn == NULL){
		printHelp(stderr);
		exit(1);
	}

	if(wFunction > 0){
		wFileOut = stdout;
		wBufferOut = bMakeBuffer();
		if(wFunction == 1){
			/* compression en cours */
			fprintf(stderr, "compression en cours\n");
		}else if(wFunction == 2){
			/* décompression */
			decompression_LZW(wFileIn, wBufferIn, wFileOut, wBufferOut);
		}
		/* fermeture des fichiers et des buffers */
		bClose(wFileIn, NULL);
		bClose(wFileOut, wBufferOut);
		bCloseBuffer(wBufferIn);
		bCloseBuffer(wBufferOut);
	}

	return 0;
}

void printHelp(FILE* aFileOut){
	fprintf(aFileOut, "LZW, version 0.1\n\n");

	fprintf(aFileOut, "usage :\n");
	fprintf(aFileOut, "LZM -c<input_file> (compress)\n");
	fprintf(aFileOut, "LZM -d<input_file> (decompress)\n\n");
}
