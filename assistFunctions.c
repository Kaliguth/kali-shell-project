// Source file implementing assist functions

#include "assistFunctions.h"

// Function to trim white spaces from start and end of given string
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

    // Add '\0' to indicate the end of the string
    *(end + 1) = '\0';

    return str;
}

// Function to check if given string contains a slash
bool containsSlash(char *str)
{
    // Go over each character in the string
    while (*str != '\0')
    {
        // Check if the current character is a slash
        if (*str == '/')
        {
            return true; // Return true if a slash is found
        }
        str++; // Move to the next character
    }
    return false; // Return false if no slash is found
}

// Function to check if given arguments array contains pipe
bool containsPipe(char **args)
{
    // Go over args array
    for (int i = 0; args[i] != NULL; i++)
    {
        // Check if the current argument is the pipe symbol
        if (strcmp(args[i], "|") == 0)
        {
            return true; // Return true if pipe symbol is found
        }
    }
    return false; // Return false if pipe symbol is not found
}
