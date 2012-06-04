# Version number
VERSION = 0.1

# includes and libs
INCS =
LIBS =

# flags
CPPFLAGS = -D_BSD_SOURCE -DVERSION=\"${VERSION}\"
CFLAGS   = -ansi -pedantic -Wall -Wextra -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = -s ${LIBS}

# compiler and linker
CC = gcc
