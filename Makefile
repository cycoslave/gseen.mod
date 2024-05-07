# Makefile for src/mod/gseen.mod/

srcdir = .

doofus:
	@echo ""
	@echo "Let's try this from the right directory..."
	@echo ""
	@cd ../../../; make

clean:
	@rm -f *.o *.$(MOD_EXT) *~

static: ../gseen.o

modules: ../../../gseen.$(MOD_EXT)

../gseen.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -DMAKING_MODS -c $(srcdir)/gseen.c && mv -f gseen.o ../

../../../gseen.$(MOD_EXT): ../gseen.o
	$(LD) -o ../../../gseen.$(MOD_EXT) ../gseen.o $(XLIBS) $(MODULE_XLIBS) && $(STRIP) ../../../gseen.$(MOD_EXT)

../gseen.o: .././gseen.mod/gseen.c \
 ../../../src/mod/module.h \
 ../modvals.h ../../eggdrop.h \
 .././gseen.mod/datahandling.c \
 .././gseen.mod/sensors.c \
 .././gseen.mod/gseencmds.c \
 .././gseen.mod/do_seen.c \
 .././gseen.mod/ai.c \
 .././gseen.mod/misc.c \
 .././gseen.mod/seentree.c \
 .././gseen.mod/generic_binary_tree.c \
 .././gseen.mod/seenlang.h \
 .././gseen.mod/gseen.h
