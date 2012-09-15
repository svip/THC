include config.mk

PROGS = dictutil
PAGES = index.thc om.thc videoer.thc ordbog.thc fanfiction.thc brainfuck.thc
SRC = ${PAGES:.thc=.c} thc.c runner.c
OBJ = ${SRC:.c=.o}

all: ${PAGES} ${PROGS}

options:
	@echo webc build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

install: ${PAGES}
	@echo "installing to www/"
	@mv ${PAGES} www

.c.o:
	@echo CC -c $<
	@${CC} -c $< ${CFLAGS}

%.thc: %.o thc.o runner.o
	@echo CC -o $@ $< thc.o runner.o
	@${CC} -o $@ $< thc.o runner.o ${CFLAGS}

dictutil: dictutil.o thc.o
	@echo CC -o $@ $< thc.o
	@${CC} -o $@ $< thc.o ${CFLAGS}

${OBJ}: config.mk

clean:
	@echo cleaning
	@rm -f ${PAGES} ${OBJ} ${PROGS}

.PHONY: all clean dist
