.PHONY: all dist clean distclean doc
NAME ?= AjavonWesley-GoL-5.1.1
CC ?= cc
CFLAGS ?= -O3 -march=native -g3 -ggdb
override CFLAGS += -std=gnu11 -pedantic -pedantic-errors -Wall -Wextra
CPPFLAGS += -Iinclude -I/usr/include/cairo
LDFLAGS += -lcairo -lm -lX11
CPPFLAGS += -I include
EXEC = main
OPATH = obj/
BPATH = bin/
CPATH = src/
HPATH = include/
LPATH = lib/


vpath %.h include/
vpath %.c src/

all: $(EXEC)
%: %.o | $(BPATH)
	ar -crv $(LPATH)libjeu.a $(OPATH)jeu.o $(OPATH)grille.o
	ranlib $(LPATH)libjeu.a
	
	$(CC) -DMODE$(MODE) $(OPATH)*.o -g -o $@ $(LOADLIBES) $(LDLIBS) $(LDFLAGS) 
	mv $@ $(BPATH)

%.o: %.c | $(OPATH)
	$(CC) -DMODE$(MODE) $(CPPFLAGS) $(CFLAGS) -c "$<" -o "$@" 
	mv $@ $(OPATH)
doc:
	doxygen Doxyfile

dist: 
	tar --lzma -cf $(NAME).tar.xz --transform="s,^,$(NAME)/," $(CPATH)*.c $(HPATH)*.h makefile README.md Doxyfile grilles/ \

$(OPATH):
	mkdir $@

$(BPATH):
	mkdir $@

clean: 
	$(RM) $(OPATH)*.o $(BPATH)$(EXEC)  
	rmdir $(OPATH)
	rmdir $(BPATH)
	rm -f $(LDIR)/*.a
	
distclean: clean
	$(RM) -r *.tar.xz doc


main: main.o grille.o jeu.o io.o
main.o: main.c grille.h io.h jeu.h
grille.o: grille.c grille.h 
jeu.o: jeu.c jeu.h grille.h
io.o: io.c io.h grille.h jeu.h
 
