##################################################
#        Makefile automatically generated        #
# Brought to you by Jules HABLOT and Rémi GATTAZ #
##################################################

#-------------------------------------------------
#                    VARIABLES
#-------------------------------------------------
CC=gcc
CFLAGS=-g -Wall -pedantic -I../include
LDFLAGS=
LIBRARIES=

SRCS_0=test_dico.c ../src/dictionnaire.c
OBJS_0=${SRCS_0:.c=.o}
EXEC_0=test_dico

SRCS_1=test_r-w_binio.c ../src/binio.c
OBJS_1=${SRCS_1:.c=.o}
EXEC_1=test_r-w_binio

EXECS=$(EXEC_0) $(EXEC_1)

#-------------------------------------------------
#                     LINKINGS
#-------------------------------------------------
all: $(EXECS)

$(EXEC_0): $(OBJS_0)
	$(CC) $(CFLAGS) -o $@ $(OBJS_0) $(LDFLAGS) $(LIBRARIES)

$(EXEC_1): $(OBJS_1)
	$(CC) $(CFLAGS) -o $@ $(OBJS_1) $(LDFLAGS) $(LIBRARIES)


#-------------------------------------------------
#                   DEPENDENCIES
#-------------------------------------------------
../src/%.o:
	make -C ../src $@
test_dico.o: test_dico.c

#-------------------------------------------------
#                     CLEANING
#-------------------------------------------------
clean_0:
	@rm -vf $(OBJS_0)
clean_1:
	@rm -vf $(OBJS_1)

clean: clean_0 clean_1

#-------------------------------------------------
#                   MRPROPING
#-------------------------------------------------
mrproper_0: clean_0
	@rm -vf $(EXEC_0)
mrproper_1: clean_1
	@rm -vf $(EXEC_1)

mrproper: mrproper_0 mrproper_1

#-------------------------------------------------
#                      .PHONY
#-------------------------------------------------
.PHONY: mrproper clean