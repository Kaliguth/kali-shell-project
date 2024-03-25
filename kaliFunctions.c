// Source file implementing functions using header file
#include "kaliFunctions.h"

// Function to display the terminal line ("username@hostname:path$ ")
void getLocation()
{
    // Variables for each part of the terminal line
    char username[BUFF_SIZE];
    char hostname[BUFF_SIZE];
    char path[BUFF_SIZE];

    // Get current working directory path
    if (getcwd(path, BUFF_SIZE) == NULL)
    {
        puts("Error - Current working directory path not found.");
        exit(EXIT_FAILURE);
    }

    // Get the hostname (Computer's name)
    if (gethostname(hostname, BUFF_SIZE) != 0)
    {
        puts("Error - Host name not found.");
        exit(EXIT_FAILURE);
    }

    // Get current user into pw struct
    // getlogin_r does not work in WSL - Many people in forums complain
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL)
    {
        puts("Error - Username not found.");
        exit(EXIT_FAILURE);
    }
    // Copy username from pw into username variable
    strcpy(username, pw->pw_name);

    // Print terminal line with all variables collected
    green();
    bold();
    printf("%s@%s", username, hostname);
    boldOff();
    resetColor();
    printf(":");
    blue();
    bold();
    printf("%s", path);
    boldOff();
    resetColor();
    printf("$ ");
}

// Function to get input from user
char *getInputFromUser()
{
    // Declare char variable and string to contain all chars inputed
    // Initial size 1 - size will grow if more than one char inputed
    char ch;
    int size = 1;
    int index = 0;
    // Size of str is size of a char
    char *str = (char *)malloc(size * sizeof(char));

    // Handle memory allocation failure
    if (str == NULL)
    {
        puts("Error allocating memory");
        return NULL;
    }

    // While char in inputed string is not next line (Enter)
    while ((ch = getchar()) != '\n')
    {
        // Add the next char of the input to str
        *(str + index) = ch;
        // Increment size and index
        size++;
        index++;
        // Add space for another char in str
        str = (char *)realloc(str, size * sizeof(char));

        // Handle memory reallocation failure
        if (str == NULL)
        {
            puts("Error reallocating memory");
            free(str); // Free str from memory
            return NULL;
        }
    }

    // When the while ends (no more chars to add), add \0 to the end of the string
    // Indicates the end of str
    *(str + index) = '\0';

    return str;
}

// Function to split given string by spaces
char **splitArguments(char *str)
{
    // Handle NULL input string
    if (str == NULL)
    {
        puts("NULL input string");
        return NULL;
    }

    // Example:
    // Given string = "cp filename filename"
    // Split arguments: [cp,file,file,NULL]
    // (Split the string by spaces)

    // Creates subStr with initial size of 2 (size will grow as needed)
    // Example for unchanged size:
    // If string is "cp", subStr will be [cp,NULL] - size of 2
    char *subStr;
    int size = 2;
    int index = 0;

    // strtok replaces spaces with \0, indicates end of an argument
    subStr = strtok(str, " ");

    // arguments variable declared with size of char, will include all string's arguments
    char **arguments = (char **)malloc(size * sizeof(char *));

    // Handle memory allocation failure
    if (arguments == NULL)
    {
        puts("Error allocating memory");
        return NULL;
    }

    // Add first subStr to arguments
    *(arguments + index) = subStr;

    // While strtok on subStr is not null (there is another argument)
    while ((subStr = strtok(NULL, " ")) != NULL)
    {
        // Increment size and index
        size++;
        index++;
        // Add subStr (next argument) to arguments
        *(arguments + index) = subStr;
        // Increase size of arguments by size of one char
        arguments = (char **)realloc(arguments, size * sizeof(char *));
    }
    // When the while ends (no more arguments to add), add NULL as the last argument
    // Indicates the end of arguments
    *(arguments + (index + 1)) = NULL;

    // Test split arguments by printing them:
    // printf("Split arguments: ");
    // for (int i = 0; arguments[i] != NULL; i++)
    // {
    //     if (arguments[i + 1] != NULL) // If next argument is not NULL
    //     {
    //         printf("%s, ", arguments[i]);
    //         // Prints current argument
    //     }
    //     else // If next argument is NULL (next argument is the added NULL)
    //     {
    //         printf("%s, %s\n", arguments[i], arguments[i+1]);
    //         // Prints current argument and and last argument (NULL)
    //     }
    // }

    return arguments;
}

void logout(char *input)
{
    free(input);
    puts("Thank you for using Kali Shell! Goodbye :)");
    exit(EXIT_SUCCESS); // EXIT_SUCCESS = 0. EXIT_FAILURE = 1.
}