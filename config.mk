# Version number
VERSION = 0.1

# includes and libs
INCS =
LIBS =

# flags
CPPFLAGS = -D_POSIX_C_SOURCE=199309L -DVERSION=\"${VERSION}\"
CFLAGS   = -ansi -pedantic -Wall -Wextra -Werror -Os ${INCS} ${CPPFLAGS} -ggdb
LDFLAGS  = -s ${LIBS}

# compiler and linker
CC = gcc
