# Makefile to build and run the app files
# Links .c files with .o files and create executable file for the app

# Explanations:

# Dependencies:
# These are files that the created file is dependent on.
# These files have to exist and be up to date for the target file to be created.
# For example: kaliShell.c and kaliShell.h are the dependencies for the kaliShell.o object file.
# If any of them changes, kaliShell.o needs to be rebuilt.

# Executable file creation:
# An executable files is created from object files.
# For example: In the line ${CC} ${FLAGS} -o kaliShell kaliShell.o kaliFunctions.o
# The gcc compiler is used along with the -Wall -g flags, and an output flag -o.
# The output is kaliShell, meaning kaliShell will be the executable file.
# kaliShell executable file will be created using kaliShell.o and kaliFunctions.o (input files for kaliShell).

# Object file creation:
# The line using the compiler and flags creates the object file by using -c flag.
# In this scenario, kaliFunctions.o is compiled from kaliFunctions.c.

# GCC compiler and flags
CC = gcc
FLAGS = -Wall -g

# Default target - Runs all targets when using "make", then runs kaliShell executable file.
all: clean kaliShell
	./kaliShell

# Executable target - Creates executable file from kaliShell.o and kaliFunctions.o object files.
kaliShell: kaliShell.o kaliFunctions.o assistFunctions.o
	$(CC) $(FLAGS) -o kaliShell kaliShell.o kaliFunctions.o assistFunctions.o

# Main app file object file target - Creates kaliShell.o and specifies dependecies and rules.
kaliShell.o: kaliShell.c kaliShell.h
	$(CC) $(FLAGS) -c kaliShell.c

# Functions source object file dependecies and rules.
kaliFunctions.o: kaliFunctions.c kaliFunctions.h
	$(CC) $(FLAGS) -c kaliFunctions.c

# Assist functions source object file dependencties and rules.
assistFunctions.o: assistFunctions.c assistFunctions.h
	$(CC) $(FLAGS) -c assistFunctions.c

# Clean target - Removes all object files and executable file.
clean:
	rm -f *.o *.out kaliShell