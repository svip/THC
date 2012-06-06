# Version number
VERSION = 0.1

# includes and libs
INCS =
LIBS =

# flags
CPPFLAGS = -D_POSIX_C_SOURCE=199309L -DVERSION=\"${VERSION}\"
CFLAGS   = -ansi -pedantic -Wall -Wextra -O3 ${INCS} ${CPPFLAGS} -std=c99 -static
LDFLAGS  = -s ${LIBS}

# compiler and linker
CC = gcc
