// Source file header for declaring functions and variables
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFF_SIZE 256
#define blue() printf("\033[0;34m")
#define resetColor() printf("\033[0m")
#define green() printf("\033[0;32m")
#define bold() printf("\e[1m")
#define boldOff() printf("\e[m")

char *getInputFromUser();
char **splitArguments(char *);

void getLocation();
void logout(char *);