CC		= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lglfw -lGLU -lGL -lm

BINDIR	= bin/
INCDIR	= inc/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers projet
GLOB_OBJ_TD= $(OBJDIR)3D_tools.o

# Fichiers main
OBJ_MAIN= $(GLOB_OBJ_TD) $(OBJDIR)main/draw_scene.o $(OBJDIR)main/main.o
EXEC_MAIN= main.out

# Fichiers exercice 03
OBJ_TD04_EX03= $(GLOB_OBJ_TD) $(OBJDIR)ex03/draw_scene.o $(OBJDIR)ex03/td04_ex03.o
EXEC_TD04_EX03= td04_ex03.out

# Fichiers exercice 04
OBJ_TD04_EX04= $(GLOB_OBJ_TD) $(OBJDIR)ex04/draw_scene.o $(OBJDIR)ex04/td04_ex04.o
EXEC_TD04_EX04= td04_ex04.out

# Regles compilation TD 04

all : menu, main, ex03, ex04

main : $(OBJ_MAIN)
	$(CC) $(CFLAGS) $(OBJ_MAIN) -o $(BINDIR)$(EXEC_MAIN) $(LDFLAGS)
	
ex03 : $(OBJ_TD04_EX03)
	$(CC) $(CFLAGS) $(OBJ_TD04_EX03) -o $(BINDIR)$(EXEC_TD04_EX03) $(LDFLAGS)
	
ex04 : $(OBJ_TD04_EX04)
	$(CC) $(CFLAGS) $(OBJ_TD04_EX04) -o $(BINDIR)$(EXEC_TD04_EX04) $(LDFLAGS)

clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c $(INCDIR)*.h
	mkdir -p `dirname $@`
	$(CC) -o $@ -I $(INCDIR) -c $< $(CFLAGS)
