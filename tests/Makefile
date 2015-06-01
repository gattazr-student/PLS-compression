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

SRCS_2=test_binio.c ../src/binio.c
OBJS_2=${SRCS_2:.c=.o}
EXEC_2=test_binio

SRCS_3=test_rle.c ../src/rle.c ../src/binio.c
OBJS_3=${SRCS_3:.c=.o}
EXEC_3=test_rle

EXECS=$(EXEC_0) $(EXEC_1) $(EXEC_2) $(EXEC_3)

#-------------------------------------------------
#                     LINKINGS
#-------------------------------------------------
all: $(EXECS)

$(EXEC_0): $(OBJS_0)
	$(CC) $(CFLAGS) -o $@ $(OBJS_0) $(LDFLAGS) $(LIBRARIES)

$(EXEC_1): $(OBJS_1)
	$(CC) $(CFLAGS) -o $@ $(OBJS_1) $(LDFLAGS) $(LIBRARIES)

$(EXEC_2): $(OBJS_2)
		$(CC) $(CFLAGS) -o $@ $(OBJS_2) $(LDFLAGS) $(LIBRARIES)
		
$(EXEC_3): $(OBJS_3)
		$(CC) $(CFLAGS) -o $@ $(OBJS_3) $(LDFLAGS) $(LIBRARIES)


#-------------------------------------------------
#                   DEPENDENCIES
#-------------------------------------------------
../src/%.o: ../src/%.c ../include/%.h
	make -C ../src $@
test_dico.o: test_dico.c ../include/dictionnaire.h
test_r-w_binio.o: test_r-w_binio.c ../include/binio.h
test_binio.o: test_binio.c ../include/binio.h
test_rle.o: test_rle.c ../include/rle.h ../include/binio.h

#-------------------------------------------------
#                     CLEANING
#-------------------------------------------------
clean_0:
	@rm -vf $(OBJS_0)
clean_1:
	@rm -vf $(OBJS_1)
clean_2:
	@rm -vf $(OBJS_2)
clean_3:
	@rm -vf $(OBJS_3)

clean: clean_0 clean_1 clean_2 clean_3

#-------------------------------------------------
#                   MRPROPING
#-------------------------------------------------
mrproper_0: clean_0
	@rm -vf $(EXEC_0)
mrproper_1: clean_1
	@rm -vf $(EXEC_1)
mrproper_2: clean_2
	@rm -vf $(EXEC_2)
mrproper_3: clean_3
	@rm -vf $(EXEC_3)

mrproper: mrproper_0 mrproper_1 mrproper_2 mrproper_3

#-------------------------------------------------
#                      .PHONY
#-------------------------------------------------
.PHONY: mrproper clean
