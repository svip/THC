include config.mk

PAGES = index.thc subpage.thc
SRC = ${PAGES:.thc=.c} thc.c
OBJ = ${SRC:.c=.o}

all: ${PAGES}

options:
	@echo webc build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

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
