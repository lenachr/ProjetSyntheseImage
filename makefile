CC		= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lglfw -lGLU -lGL -lm

BINDIR	= bin/
INCDIR	= inc/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers projet
GLOB_OBJ_TD= $(OBJDIR)3D_tools.o

# Fichiers menu
OBJ_menu= $(GLOB_OBJ_TD) $(OBJDIR)menu/draw_scene.o $(OBJDIR)menu.o
EXEC_menu= menu.out

# Fichiers exercice 02
OBJ_TD04_EX02= $(GLOB_OBJ_TD) $(OBJDIR)ex02/draw_scene.o $(OBJDIR)ex02/td04_ex02.o
EXEC_TD04_EX02= td04_ex02.out

# Fichiers exercice 03
OBJ_TD04_EX03= $(GLOB_OBJ_TD) $(OBJDIR)ex03/draw_scene.o $(OBJDIR)ex03/td04_ex03.o
EXEC_TD04_EX03= td04_ex03.out

# Fichiers exercice 04
OBJ_TD04_EX04= $(GLOB_OBJ_TD) $(OBJDIR)ex04/draw_scene.o $(OBJDIR)ex04/td04_ex04.o
EXEC_TD04_EX04= td04_ex04.out

# Regles compilation TD 04

all : menu, ex02, ex03, ex04

menu : $(OBJ_menu)
	$(CC) $(CFLAGS) $(OBJ_menu) -o $(BINDIR)$(EXEC_menu) $(LDFLAGS)
	
ex02 : $(OBJ_TD04_EX02)
	$(CC) $(CFLAGS) $(OBJ_TD04_EX02) -o $(BINDIR)$(EXEC_TD04_EX02) $(LDFLAGS)
	
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
