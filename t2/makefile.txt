all: tempo

tempo: tempo.o smpl.o rand.o avere.o lista_t.o
	$(LINK.c) -o $@ -Bstatic tempo.o smpl.o rand.o avere.o lista_t.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

tempo.o: tempo.c smpl.h
	$(COMPILE.c) -g  tempo.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

avere.o: avere.c
	$(COMPILE.c) -g avere.c

lista_t.o: lista_t.c
	$(COMPILE.c) -g lista_t.c

clean:
	$(RM) *.o tempo relat saida

 