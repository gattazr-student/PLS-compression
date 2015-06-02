#!/bin/sh

PROGNAME=LZW
PATH_TO_PROGNAME=../src/
EXEC=$PATH_TO_PROGNAME$PROGNAME
OUTPUT_DIR=result/
FILE_NAME=$(basename $1)
EXTENSION="${FILE_NAME##*.}"
FILE_NAME="${FILE_NAME%.*}"

# Compile le programm LZW
echo "Compilation du programme: "
make -C $PATH_TO_PROGNAME $PROGNAME

if [ $? -ne 0 ]; then
	echo "La compilation du programme n'a pas fonctionné"
	exit 1
fi

# Compresse
echo "Compression: $EXEC -c$1 > $OUTPUT_DIR$FILE_NAME.lzw 2> $OUTPUT_DIR${FILE_NAME}_compress.log"
$EXEC -c$1 > $OUTPUT_DIR$FILE_NAME.lzw 2> $OUTPUT_DIR${FILE_NAME}_compress.log
if [ $? -ne 0 ]; then
	echo "La compression du fichier n'a pas fonctionné correctement"
	exit 1
fi

# Décompresse
echo "Décompression: $EXEC -d$OUTPUT_DIR$FILE_NAME.lzw > $OUTPUT_DIR$FILE_NAME.$EXTENSION 2> $OUTPUT_DIR${FILE_NAME}_decompress.log"
$EXEC -d$OUTPUT_DIR$FILE_NAME.lzw > $OUTPUT_DIR$FILE_NAME.$EXTENSION 2> $OUTPUT_DIR${FILE_NAME}_decompress.log
if [ $? -ne 0 ]; then
	echo "La décompression du fichier n'a pas fonctionné correctement"
	exit 1
fi

# Puis fais un diff si tous s'est bien déroulé jusqu'ici
echo "Diff: "
diff $1 $OUTPUT_DIR$FILE_NAME.$EXTENSION
