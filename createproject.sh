#!/bin/zsh

PROJECT_PATH=""
AUTHOR1="Ravix Calliste"
AUTHOR2="Michel Maxime"
AUTHORS="$AUTHOR1 & $AUTHOR2"
ENTETE_C="/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */"
 
# All errors are logged 
exec 2>>/tmp/create_project.log
 
#
# 
#
function error() { 
    echo "Erreur : $1!" >&2 
    exit 1 
} 

#
# 
#
function help() { 
    echo "`basename $0` est un script shell pour créer des projets C."
    exit 0
} 

#
# 
#
function initialize_rep() {
   if [ ! -e $PROJECT_PATH ]; then
      mkdir $PROJECT_PATH
      for repertory in bin/test doc etc include lib src/test
         do
            mkdir -p "$PROJECT_PATH/$repertory"
         done
      return 0
   else
      echo "Le projet existe déjà"
      return 1
   fi
}

#
# 
#
function create_README() {
   cat > "$PROJECT_PATH/README.md"<<EOF
# Projet "$PROJECT_PATH"
bla bla bla
bli bli bli
EOF
}

#
# 
#
function create_INSTALL() {
   cat > "$PROJECT_PATH/INSTALL.txt"<<EOF
How to install this program
 
bla bla bla
bli bli bli
EOF
}

#
# 
#
function create_AUTHORS() {
   cat > "$PROJECT_PATH/AUTHORS.txt"<<EOF
$AUTHORS
EOF
}

#
# 
#
function create_Makefile() {
   echo "CC       = gcc
RM       = rm
DOC      = doxygen
CPPFLAGS = -I./include
CFLAGS   = -Wall -Wextra -ansi -pedantic -g
LDFLAGS  = -L ./lib # Add the libraries you will use, e.g. -lm

.PHONY: all doc clean distclean

all: ./bin/prog.exe

./bin/prog.exe: ./src/main.o # And others .o
	\$(CC) \$< -o \$@ \$(LDFLAGS)

./src/%.o: ./src/%.c
	\$(CC) \$(CPPFLAGS) \$(CFLAGS) \$< -o \$@ -c

doc:
	-@\$(DOC) doc/Doxyfile

clean:
	-@\$(RM) ./src/*.o

distclean: clean
	-@\$(RM) ./bin/prog.exe" > $PROJECT_PATH/Makefile
}

#
# 
#
function create_main() {
   cat > "$PROJECT_PATH/src/main.c"<<EOF
$ENTETE_C 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
   #define USAGE "Usage : %s text_file\n"

   if ((argc > 1) && (!strcasecmp("-h", argv[1]))) {
      fprintf(stderr, USAGE, argv[0]);
      return EXIT_FAILURE;
   }   
   
   /* Your code here ... */
   return EXIT_SUCCESS;
}
EOF
}

#
# 
#
function create_Doxyfile() {
   cat > "$PROJECT_PATH/doc/Doxyfile"<<EOF
PROJECT_NAME = "$PROJECT_PATH"
OUTPUT_DIRECTORY = doc
FILE_PATTERNS = include/*.h src/*.c
GENERATE_HTML = YES
GENERATE_LATEX = NO
GENERATE_RTF = NO
EOF
}

function copy_minunit() {
	wget -P "$PROJECT_PATH/include" https://raw.githubusercontent.com/siu/minunit/master/minunit.h
	wget -P "$PROJECT_PATH/src/test" https://raw.githubusercontent.com/siu/minunit/master/minunit_example.c
}

#
# 
#
function create_project() {
   echo "Entrer le nom du nouveau projet :"
   read PROJECT_PATH
   initialize_rep
   if [ "$?" == 0 ] 
   then
      create_README
      create_INSTALL
      create_AUTHORS
      create_main
      create_Makefile
	  create_Doxyfile
	  copy_minunit
      echo "Projet `basename $PROJECT_PATH` créé."
   fi
}

#
# 
#
function open_project() {
   echo "Entrer le nom du projet à ouvrir :"
   read PROJECT_PATH
   if [ ! -e $PROJECT_PATH ] 
   then
      echo "Projet inexistant. Il va être créé."
      initialize_rep
      if [ "$?" == 0 ] 
      then
         create_README
         create_INSTALL
         create_AUTHORS
         create_main
         create_Makefile
   	     create_Doxyfile
   	     copy_minunit
      fi
      echo "Projet `basename $PROJECT_PATH` créé."
      return 0
   else 
      if [ ! -d $PROJECT_PATH ] 
      then 
         echo "Un fichier porte déjà ce nom."
	 PROJECT_PATH=""
         return 10
      else
         echo "Projet ouvert."	      
      fi
fi
}

#
# Creates a .TGZ compressed archive
# The archive filename indicates both authors and the current date
# using the format:
# PROJECT_DIRECTORY-NAME1-NAME2-day-month-year-hour:min.tgz
#
function create_archive() {
   author1=$( echo $AUTHOR1 | cut -f2 -d' ' )
   author2=$( echo $AUTHOR2 | cut -f2 -d' ' )
   currentdate=$( date +%d−%m−%Y-%H-%M )
   
   if [ -n $PROJECT_PATH ]; then
      cd "${PROJECT_PATH}" ; make distclean; cd .. ;   
      tar czf "$PROJECT_PATH-$author1-$author2-$currentdate.tgz" `basename $PROJECT_PATH`
   else
      error "Projet courant inexistant !" 
   fi
   echo "Archive du projet courant créée."
}

#
# Suppress the current C project
#
function suppress_project() {
   rm -R -f $PROJECT_PATH ;  
   echo "Projet courant supprimé."
}

#
# Compiles the current C project
#
function compile_project() {
   if [ -z $PROJECT_PATH ]
   then
      echo "Aucun projet ouvert."
      return 12
   fi
   cd "${PROJECT_PATH}" ; make ; cd .. ; 
   echo "Projet compilé."
}

#
# The main menu
#
function menu() {
   clear ; loop=y
   while [ $loop = y ]
   do
      echo "Générateur de squelette d'un projet C"  
      echo "====================================="
      echo "N|n : créer un nouveau projet"
      echo "O|o : ouvrir un projet"
      echo "S|s : supprimer le projet courant"
      echo "C|c : compiler le projet courant"
      echo "A|a : archiver le projet courant"
      echo "Q|q : quitter"
      echo
      read choix # silent mode with no echo
      case $choix in
         N|n) create_project ;;
         O|o) open_project ;;
         S|s) suppress_project ;;
         C|c) compile_project ;;
         A|a) create_archive ;;
         Q|q) loop=n ;;
           *) echo "Choix invalide !" ;;
      esac
      echo
   done 
}

# Début du script
if [ $1 = "-h" ] 
then
   help
fi

menu

exit 0