# Some simples constants.
CFLAGS = -Wall
CC = gcc
MKDIR := mkdir -p
RM = rm

# Internal variables.
SOURCES = $(wildcard *.c)
objects = $(subst .c,.o,$(SOURCES))
LEXSRCS = $(wildcard *.l)
lextrgt = $(subst .l,.c,$(lexsrcs))
#lextrgt = $(lexsrcs:.l=.c)
libraries = libtest.so
targets = libtest.so test1 test2 test2a test3 test4 test5 test6 test7 test8 test9 testdb1

# A function or two.
maybe-make-dir = $(if $(wildcard $1),,$(MKDIR) $1)
assert-not-null = $(if $1,,$(error Illegal null value.))

# vpath directives
vpath %.so ~/Sources/C/tests

# Some targets.
.PRECIOUS: $(lextrgt)

.PHONY: all
all: $(targets)

.PHONY: clean
clean:
	$(RM) -f $(objects) $(targets) $(lextrgt) $(libraries)

libtest.so: failtest.o rnd.o
	$(CC) -shared -fPIC $^ $(OUTPUT_OPTION)

test2: test2.o -ltest -lm
test2a: test2a.o -ltest -lm
test3: test3.o -ltest -lm
test4: test4.o -ltest -lm
test5: test5.o -ltest -lm
testdb1: testdb1.o -lsqlite3

test6: test6.o
	$(LINK.c) $^ -pthread $(OUTPUT_OPTION)

test7: test7.o -ltest -lm -lncurses
	$(LINK.c) $^ -pthread -L$(PWD) $(OUTPUT_OPTION)

test8: test8.o -lncurses
	$(LINK.c) $^ $(OUTPUT_OPTION)

test9: test9.o -lncurses -lm
	$(LINK.c) $^ $(OUTPUT_OPTION)

failtest.o: failtest.h rnd.h
	$(COMPILE.c) -fPIC $(OUTPUT_OPTION) $(subst .o,.c,$(@))

rnd.o: rnd.h
	$(COMPILE.c) -fPIC $(OUTPUT_OPTION) $(subst .o,.c,$(@))

test7.o: rnd.h failtest.h

#include $(subst .c,.d,$(SOURCES))

#%.d: %.c
#	$(CC) -M $(CPPFLAGS) $< > $@.$$$$;               \
#	sed 's,\($*\)\.o[:]*,\1.o $@ :,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$
