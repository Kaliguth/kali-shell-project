// Assist functions header for declaring functions
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>

char *trimWhitespaces(char *);
bool containsSlash(char *);
bool containsPipe(char **);
bool containsAppend(char **);
bool containsWrite(char **);
