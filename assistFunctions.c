// Source file implementing assist functions

#include "assistFunctions.h"

char *trimWhitespaces(char *str)
{
    // Trim leading white spaces
    while (isspace((unsigned char)*str))
    {
        str++;
    }

    // If the string becomes empty, return it
    if (*str == '\0')
    {
        return str;
    }

    // Trim trailing white spaces
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
    {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}