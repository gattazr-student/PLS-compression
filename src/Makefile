##################################################
#        Makefile automatically generated        #
# Brought to you by Jules HABLOT and Rémi GATTAZ #
##################################################

#-------------------------------------------------
#                    VARIABLES
#-------------------------------------------------
CC=gcc
CFLAGS=-g -Wall -pedantic -I../include # -DDEBUG_DICO # -DDEBUG_BINIO
LDFLAGS=
LIBRARIES= -lm

SRCS_0=main.c binio.c compression.c decompression.c dictionnaire.c rle.c
OBJS_0=${SRCS_0:.c=.o}
EXEC_0=LZW

EXECS=$(EXEC_0)

#-------------------------------------------------
#                     LINKINGS
#-------------------------------------------------
all: $(EXECS)

$(EXEC_0): $(OBJS_0)
	$(CC) $(CFLAGS) -o $@ $(OBJS_0) $(LDFLAGS) $(LIBRARIES)

#-------------------------------------------------
#                   DEPENDENCIES
#-------------------------------------------------
main.o: main.c ../include/types.h ../include/binio.h ../include/dictionnaire.h ../include/compression.h ../include/decompression.h
binio.o: binio.c ../include/binio.h ../include/types.h
compression.o: compression.c ../include/compression.h ../include/binio.h ../include/dictionnaire.h
decompression.o: decompression.c ../include/decompression.h ../include/binio.h ../include/dictionnaire.h
dictionnaire.o: dictionnaire.c ../include/dictionnaire.h
rle.o: rle.c ../include/rle.h ../include/binio.h

#-------------------------------------------------
#                     CLEANING
#-------------------------------------------------
clean_0:
	@rm -vf $(OBJS_0)

clean: clean_0

#-------------------------------------------------
#                   MRPROPING
#-------------------------------------------------
mrproper_0: clean_0
	@rm -vf $(EXEC_0)

mrproper: mrproper_0

#-------------------------------------------------
#                      .PHONY
#-------------------------------------------------
.PHONY: mrproper clean
