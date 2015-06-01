#!/bin/sh

PROGNAME=LZW
PATH_TO_PROGNAME=../src/
EXEC=$PATH_TO_PROGNAME$PROGNAME
OUTPUT_DIR=data
FILE_NAME=$(basename $1)

# Compile le programm LZW
echo "Compilation du programme: "
make -C $PATH_TO_PROGNAME $PROGNAME

if [ $? -ne 0 ]; then
	echo "La compilation du programme n'a pas fonctionné"
	exit
fi

# Compresse

echo "Compression: $EXEC -c$1 > $OUTPUT_DIR/res1_$FILE_NAME 2> $OUTPUT_DIR/log1_$FILE_NAME.log"
$EXEC -c$1 > $OUTPUT_DIR/res1_$FILE_NAME 2> $OUTPUT_DIR/log1_$FILE_NAME.log
if [ $? -ne 0 ]; then
	echo "La compression du fichier n'a pas fonctionné correctement"
	exit
fi
# Décompresse
echo "Décompression: $EXEC -d$OUTPUT_DIR/res1_$FILE_NAME > $OUTPUT_DIR/res2_$FILE_NAME1 2> $OUTPUT_DIR/log2_$FILE_NAME.log"
$EXEC -d$OUTPUT_DIR/res1_$FILE_NAME > $OUTPUT_DIR/res2_$FILE_NAME 2> $OUTPUT_DIR/log2_$FILE_NAME.log
if [ $? -ne 0 ]; then
	echo "La décompression du fichier n'a pas fonctionné correctement"
	exit
fi

# Puis fais un diff si tous s'est bien déroulé jusqu'ici
echo "Diff: "
diff $1 $OUTPUT_DIR/res2_$FILE_NAME
