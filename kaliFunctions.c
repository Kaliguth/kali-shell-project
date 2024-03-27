// Main source file implementing functions
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
    printf("Split arguments: ");

    // If there are no arguments
    if (arguments[0] == NULL)
        puts("No arguments");

    for (int i = 0; arguments[i] != NULL; i++)
    {
        if (arguments[i + 1] != NULL) // If next argument is not NULL
        {
            printf("%s, ", arguments[i]);
            // Prints current argument
        }
        else // If next argument is NULL (next argument is the added NULL)
        {
            printf("%s, %s\n", arguments[i], arguments[i + 1]);
            // Prints current argument and and last argument (NULL)
        }
    }

    return arguments;
}

// ls function to show files in current working directory
void ls()
{
    // Open the current working directory
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        puts("kaliShell: ls: Error - Current working directory path not found.");
        return;
    }

    // Read current working directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    // Close the current working directory
    closedir(dir);
}

// cd - Navigation
void cd(char **args)
{
    // Check if args has an argument after "cd"
    if (args[1] == NULL)
    {
        puts("kaliShell: cd: Path not specified");
        return;
    }

    // Current path variable
    char currentPath[BUFF_SIZE];
    // Get current working directory path
    getcwd(currentPath, BUFF_SIZE);

    // Get last argument in args to later check if ends with quote ('"')
    char *lastArgument = NULL;
    for (int i = 1; args[i] != NULL; i++)
    {
        lastArgument = args[i];
    }

    // Allocate memory for new path to navigate to
    char newPath[BUFF_SIZE];

    // cd arguments ("cd ." and "cd ..") - Navigate to previous directory
    if (strcmp(args[1], ".") == 0)
    {
        return; // "cd ." stays in the same directory
    }
    else if (strcmp(args[1], "..") == 0)
    {
        // Find the last slash in the current working directory
        // This string will be the current directory minus the part after the last slash
        char *previousDirectory = strrchr(currentPath, '/');
        // If there is no previous directory to go up to
        if (previousDirectory == NULL)
        {
            return;
        }
        else
        {
            // Remove the current directory from the previous directory string
            *previousDirectory = '\0';

            // Move up to previous directory
            if (chdir(currentPath) != 0)
            {
                return;
            }
        }
    }
    // cd arguments without quotes ('"') - Navigate to input path
    // This expects full paths
    // If full path was not provided, attempts to navigate to input path from current directory
    // Example input: cd mnt/c:/folder
    else if (strncmp(args[1], "\"", 1) != 0)
    {
        strcpy(newPath, "/");     // Add space in the beginning of path
        strcat(newPath, args[1]); // Copy input path into newPath

        // If input contains more arguments
        // Example input: cd c:/ folder
        if (args[2] != NULL)
        {
            puts("kaliShell: cd: Too many arguments");
        }
        else
        {
            printf("Path: %s\n", newPath);
            // Check if the specified path exists
            if (chdir(newPath) != 0)
            {
                // If navigating to new path was intended from current directory by the user
                // Move newPath one address ahead and add a dot to the beginning
                memmove(newPath + 1, newPath, strlen(newPath) + 1);
                newPath[0] = '.';

                // Try navigating to newPath from current directory
                if (chdir(newPath) != 0)
                {
                    printf("kaliShell: cd: '%s': No such file or directory\n", newPath + 2);
                }
            }
        }
    }
    // cd arguments with quotes ('"') - Navigate to folder in existing path
    // Intended to accept only folder names in current directory
    // If given argument starts with '"', contains path and ends with '"'
    // Example input: cd "folder name"
    else if (args[1][0] == '\"' && args[1][1] != '\0' && lastArgument[strlen(lastArgument) - 1] == '\"')
    {
        strcpy(newPath, "./");
        strcat(newPath, args[1] + 1); // Add first argument to newPath excluding the first quote

        for (int i = 2; args[i] != NULL; i++)
        {
            strcat(newPath, " ");     // Add space between arguments
            strcat(newPath, args[i]); // Add current argument
        }
        newPath[strlen(newPath) - 1] = '\0'; // Remove last quote ('"') from end of path string

        printf("%s\n", newPath);
        // Change directory to the extracted folder name
        if (chdir(newPath) != 0)
        {
            printf("kaliShell: cd: '%s': No such file or directory\n", newPath + 2);
        }
    }
    else
    {
        puts("kaliShell: cd: Invalid folder name format");
    }
}

// cp - Copy files or folders
void cp(char **args)
{
    // Check if args has arguments after "cp"
    if (args[1] == NULL)
    {
        puts("kaliShell: cp: No files provided");
        return;
    }
    if (args[2] == NULL)
    {
        puts("kaliShell: cp: Destination file not provided");
        return;
    }

    // Get last argument in args to later check if ends with quote ('"')
    char *lastArgument = NULL;
    for (int i = 1; args[i] != NULL; i++)
    {
        lastArgument = args[i];
    }

    // Allocate memory for source and destination file (for file names with spaces)
    char source[BUFF_SIZE];
    char destination[BUFF_SIZE];

    // Declare last source index variable
    int lastSourceIndex = 1;
    // Find last source index if source starts with quote
    if (strncmp(args[1], "\"", 1) == 0 && args[1][strlen(args[1]) - 1] != '\"')
    {
        for (int i = 1; args[i][strlen(args[i]) - 1] != '\"' && args[i + 1] != NULL; i++)
        {
            lastSourceIndex++;
        }

        // If last source index reached last argument - invalid source name format
        if (args[lastSourceIndex] == lastArgument)
        {
            puts("kaliShell: cp: Invalid source file name format");
            return;
        }
    }

    // Declare string to copy, source and destination files variables
    char ch;
    FILE *src, *des;

    // Error handling if both source and destination files don't start with quotes ('"')
    if (strncmp(args[1], "\"", 1) != 0 && strncmp(args[2], "\"", 1) != 0)
    {
        // Any of them end with quote
        // Example inputs: 'cp source" destination' or ' cp source destination"'
        if (args[1][strlen(args[1]) - 1] == '\"' || args[2][strlen(args[2]) - 1] == '\"')
        {
            puts("kaliShell: cp: Invalid file name format");
            return;
        }

        // More than three arguments inputed
        // Example input: cp source destination hello
        if (args[3] != NULL)
        {
            puts("kaliShell: cp: Too many arguments");
            return;
        }
    }

    // Source file without quotes ('"') - File name without spaces
    // Does not start or end with quotes
    // Example input: cp source destination
    if (strncmp(args[1], "\"", 1) != 0 && args[1][strlen(args[1]) - 1] != '\"')
    {
        // Open source file provided in args[1] for reading
        if ((src = fopen(args[1], "r")) == NULL)
        {
            printf("kaliShell: cp: Error reading file '%s'\n", args[1]);
            return;
        }
    }
    // Source file with quotes ('"') - File name with spaces
    // and is not a single argument source (check with last source index)
    // Example input: cp "source file" destination
    else if (strncmp(args[1], "\"", 1) == 0)
    {
        // Add first argument to source string excluding the first quote
        strcpy(source, args[1] + 1);

        // If current source string does not end with quote
        if (source[strlen(source) - 1] != '\"')
        {
            // Add more arguments to source string as long as previous argument did not end in quote
            for (int i = 2; args[i - 1][strlen(args[i - 1]) - 1] != '\"'; i++)
            {
                strcat(source, " ");     // Add space between arguments
                strcat(source, args[i]); // Add current argument
            }
        }

        // Error handling if source name does not end with quote
        if (source[strlen(source) - 1] != '\"')
        {
            puts("kaliShell: cp: Invalid source file name format");
            return;
        }
        // Remove last quote ('"') from end of source string
        source[strlen(source) - 1] = '\0';

        printf("Source: %s\n", source);

        // Open source file for reading using source string
        if ((src = fopen(source, "r")) == NULL)
        {
            printf("kaliShell: cp: Error reading file '%s'\n", source);
            return;
        }
    }
    else
    {
        puts("kaliShell: cp: Invalid source file name format");
        return;
    }

    // Decalre destination index variable (one argument after source)
    int destinationIndex = lastSourceIndex + 1;

    // Destination file without quotes ('"') - File name without spaces
    // Does not start or end with quotes
    // and destination argument is the last argument
    // Example input: cp source destination
    if (strncmp(args[destinationIndex], "\"", 1) != 0 && lastArgument[strlen(args[destinationIndex]) - 1] != '\"' && args[destinationIndex] == lastArgument)
    {
        // Open destination file provided after last source index for writing
        if ((des = fopen(args[destinationIndex], "w")) == NULL)
        {
            printf("kaliShell: cp: Error writing to file '%s'\n", args[destinationIndex]);
            fclose(src);
            return;
        }
    }
    // Destination file with quotes ("") - File name with spaces
    // Destination starts and ends with quotes
    // Example input: cp source "destination file"
    else if (strncmp(args[destinationIndex], "\"", 1) == 0 && lastArgument[strlen(lastArgument) - 1] == '\"')
    {
        // Add first destination argument to destination string excluding the first quote
        strcpy(destination, args[destinationIndex] + 1);

        // If current destination string does not end with quote
        if (destination[strlen(destination) - 1] != '\"')
        {
            // Add more arguments to destination string as long as previous argument did not end in quote
            for (int i = destinationIndex + 1; args[i - 1][strlen(args[i - 1]) - 1] != '\"'; i++)
            {
                strcat(destination, " ");     // Add space between arguments
                strcat(destination, args[i]); // Add current argument
            }
        }
        // Remove last quote ('"') from end of destination string
        destination[strlen(destination) - 1] = '\0';

        // Open destination file for writing using destination string
        if ((des = fopen(destination, "w")) == NULL)
        {
            printf("kaliShell: cp: Error writing to file '%s'\n", destination);
            fclose(src);
            return;
        }
    }
    else
    {
        puts("kaliShell: cp: Invalid destination file name format");
        return;
    }
    
    // Write contents of source file into destination file
    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }

    // Close both files
    fclose(src);
    fclose(des);
}

void logout()
{
    puts("Thank you for using Kali Shell! Goodbye :)");
    exit(EXIT_SUCCESS); // EXIT_SUCCESS = 0. EXIT_FAILURE = 1.
}