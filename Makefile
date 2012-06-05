include config.mk

PAGES = index.thc om.thc
SRC = ${PAGES:.thc=.c} thc.c
OBJ = ${SRC:.c=.o}

all: ${PAGES}

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

%.thc: %.o thc.o
	${CC} -o $@ $< thc.o ${CFLAGS}

${OBJ}: config.mk

clean:
	@echo cleaning
	@rm -f ${PAGES} ${OBJ} ${PAGES}

.PHONY: all clean dist
