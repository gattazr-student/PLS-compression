##################################################
#        Makefile automatically generated        #
# Brought to you by Jules HABLOT and Rémi GATTAZ #
##################################################

#-------------------------------------------------
#                    VARIABLES
#-------------------------------------------------
CC=gcc
CFLAGS=-g -Wall -pedantic -I../includes
LDFLAGS=
LIBRARIES=

SRCS_0=main.c binio.c compression.c decompression.c dictionnaire.c
OBJS_0=${SRCS_0:.c=.o}
EXEC_0=LZW

EXECS=$(EXEC_0)

#-------------------------------------------------
#                     LINKINGS
#-------------------------------------------------
all: $(EXEC_0)

$(EXEC_0): $(OBJS_0)
	$(CC) $(CFLAGS) -o $@ $(OBJS_0) $(LDFLAGS) $(LIBRARIES)

#-------------------------------------------------
#                   DEPENDENCIES
#-------------------------------------------------
main.o: main.c ../include/main.h
binio.o: binio.c ../include/binio.h
compression.o: compression.c ../include/compression.h
decompression.o: decompression.c ../include/decompression.h
dictionnaire.o: dictionnaire.c ../include/dictionnaire.h

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
