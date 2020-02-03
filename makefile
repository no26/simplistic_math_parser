CC=gcc
FLAGS=-Wall -Wextra -Wformat -std=c99 -pedantic -lm

PARSER=math_parser
MAIN=test_parser

RM=rm -rf

SRC_MAIN=$(PARSER).c $(MAIN).c
OBJ_MAIN=$(SRC_MAIN:.c=.o) #transform .c -> .o

all:	$(MAIN)

# $@ = cible de la commande
# $^ = toute les dépendances	
$(MAIN):	$(OBJ_MAIN)
	$(CC) -o $@ $^ $(FLAGS)
# recip all .o
%.o:	%.c
	$(CC) -o $@ -c $^ $(FLAGS)

clean:
	$(RM) $(MAIN)

clobber: clean
	$(RM) *.o