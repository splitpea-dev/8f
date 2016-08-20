# 8f makefile

# files to compile
SOURCE = main.c deck.c player.c

# c compiler
CC = cc

# compiler flags
# -ansi for ansi c standard
# -Wall for compiler warnings
CFLAGS = -Wall -ansi -pedantic

# output file (executable)
OUT = 8f

# compile
all :   $(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) -o $(OUT)

# clean
clean :
	-rm $(OUT)