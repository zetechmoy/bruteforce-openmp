#
##

CC	=	gcc
CFLAGS	=  	-Wall

PREFIX ?= 	${DESTDIR}/usr
LIBDIR	=	${PREFIX}/lib
INCDIR	=	${PREFIX}/include
LIBS    =   -lcrypto -fopenmp

CIBLES	=	dict2sha multiattack_mono multiattack_omp shadowFaker

all: $(CIBLES)

clean:
	$(RM) $(CIBLES) dict.sha shadow.sha

dict2sha:
	$(CC) $(CFLAGS) -o $@ $@.c  $(LIBS)

multiattack_mono:
	$(CC) $(CFLAGS) -o $@ $@.c  $(LIBS)

multiattack_omp:
	$(CC) $(CFLAGS) -o $@ $@.c  $(LIBS)

shadowFaker:
	$(CC) $(CFLAGS) -o $@ $@.c  $(LIBS)
