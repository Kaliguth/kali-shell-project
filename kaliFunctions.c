// Main source file implementing functions
#include "kaliFunctions.h"
#include "assistFunctions.h"

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

// echo - Function to print given input
void echo(char **args)
{
    while (*(++args))
        printf("%s ", *args);
    puts("");
}

// ls - Function to show files in current working directory
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

// grep - Function to print files or lines containing given string
// This function is not used in main
// mypipe of ls with grep uses external grep automatically
void grep(char *str)
{
    // Error handling if grep string is null or empty
    if (str == NULL || str[0] == '\0' || str[1])
    {
        puts("kaliShell: grep: No grep string provided\n");
        return;
    }

    char line[BUFF_SIZE];

    // Read lines from stdin until the end of the file
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        // Search for the pattern in the line
        if (strstr(line, str) != NULL)
        {
            // If pattern found, print the line
            printf("%s", line);
        }
    }
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
    // and is not a single argument source
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

// delete - Delete files or folders by path or file name
void delete(char **args)
{
    // Check if args has arguments after "delete"
    if (args[1] == NULL)
    {
        puts("kaliShell: delete: No file provided");
        return;
    }

    // Find last argument value and index
    char *lastArgument = args[1];
    int lastArgumentIndex = 1;
    for (int i = 2; args[i] != NULL; i++)
    {
        lastArgument = args[i];
        lastArgumentIndex++;
    }

    // Declare variable for path
    char path[BUFF_SIZE];

    // Path does not start with quotes
    // Example: delete file
    if (strncmp(args[1], "\"", 1) != 0)
    {
        // Add args[1] to path string
        strcpy(path, args[1]);

        // Error handling:
        // If more than one argument for path
        // Example: delete file hello
        if (lastArgumentIndex != 1)
        {
            puts("kaliShell: delete: Too many arguments");
            return;
        }
        // If path ends with quotes
        // Example: path/file"
        if (args[1][strlen(args[1]) - 1] == '\"')
        {
            puts("kaliShell: delete: Invalid path or file name format");
            return;
        }

        // If path contains slash (meaning a full file path)
        // Example: mnt/d/Ariel/file
        if (containsSlash(path))
        {
            // Move path one ahead and add slash to the beginning
            memmove(path + 1, path, strlen(path) + 1);
            path[0] = '/';
        }
        // If path does not contain slash (meaning file name in current folder)
        // Example: file
        else
        {
            // Create copy of path
            char pathCopy[BUFF_SIZE];
            strcpy(pathCopy, path);
            // Change path to './' then add pathCopy to it
            strcpy(path, "./");
            strcat(path, pathCopy);
        }

        // Delete file or folder in path given
        if (unlink(path) != 0)
        {
            if (rmdir(path) != 0)
            {
                printf("kaliShell: delete: '%s': No such file or directory\n", path);
            }
        }
    }
    // Path starts and ends with quotes
    // Example: delete "file name" OR delete "mnt/d/file"
    else if (strncmp(args[1], "\"", 1) == 0 && lastArgument[strlen(lastArgument) - 1] == '\"')
    {
        // Add args[1] excluding first quote to path string
        strcpy(path, args[1] + 1);

        // Add more arguments to path string as long as previous argument did not end in quote
        for (int i = 2; args[i - 1][strlen(args[i - 1]) - 1] != '\"'; i++)
        {
            strcat(path, " ");     // Add space between arguments
            strcat(path, args[i]); // Add current argument
        }
        // Remove last quote ('"') from end of destination string
        path[strlen(path) - 1] = '\0';

        // If path contains slash (meaning a full file path)
        // Example: mnt/d/Ariel/file
        if (containsSlash(path))
        {
            // Move path one ahead and add slash to the beginning
            memmove(path + 1, path, strlen(path) + 1);
            path[0] = '/';
        }
        // If path does not contain slash (meaning file name in current folder)
        // Example: file
        else
        {
            // Create copy of path
            char pathCopy[BUFF_SIZE];
            strcpy(pathCopy, path);
            // Change path to './' then add pathCopy to it
            strcpy(path, "./");
            strcat(path, pathCopy);
        }

        printf("path: %s\n", path);
        // Delete file or folder in path given
        if (unlink(path) != 0)
        {
            if (rmdir(path) != 0)
            {
                printf("kaliShell: delete: '%s': No such file or directory\n", path);
            }
        }
    }
    else
    {
        puts("kaliShell: delete: Invalid path or file name format");
        return;
    }
}

// mypipe - Child processes function
void mypipe(char **args1, char **args2)
{
    // Create an array for file descriptors
    int fileDescs[2];

    // Create a pipe (two file descriptors are stored in fileDescs)
    // fileDesc[1] can read from fielDesc[0]
    if (pipe(fileDescs) != 0)
    {
        // Error handling if pipe failed
        puts("kaliShell: mypipe: Pipe failed");
        return;
    }

    // Fork the parent process into child process
    // Child processes don't use program's main functions, but system commands
    // Example: 'ls -l | grep file'
    // Will work, even though I don't have ls -l in my program
    if (fork() == 0)
    {
        // First child process
        // Close the read descriptor of the pipe (it is not needed for this part)
        close(fileDescs[0]);
        // Assign STDOUT to the write descriptor of the pipe
        dup2(fileDescs[1], STDOUT_FILENO);
        // Close unused file descriptors
        close(fileDescs[1]);

        // Execute args1 command
        if (execvp(args1[0], args1) == -1)
        {
            puts("kaliShell: mypipe: Executing first command failed");
            return;
        }
    }
    else
    {
        // Fork again to create the second child process
        if (fork() == 0)
        {
            // Second child process
            // Close the write descriptor of the pipe (it is not needed for this part)
            close(fileDescs[1]);
            // Assign STDIN to the read descriptor of the pipe
            dup2(fileDescs[0], STDIN_FILENO);
            // Close unused file descriptors
            close(fileDescs[0]);

            // Execute args2 command
            if (execvp(args2[0], args2) == -1)
            {
                puts("kaliShell: mypipe: Executing second command failed");
                return;
            }
        }
        else
        {
            // Close both descriptors of the pipe in the parent process
            close(fileDescs[0]);
            close(fileDescs[1]);
            // Wait for both child processes to finish
            wait(NULL);
            wait(NULL);
        }
    }
}

// move - Function to move a file to a folder
void move(char **args)
{
    // Check if args has arguments after "move"
    if (args[1] == NULL)
    {
        puts("kaliShell: move: No file provided");
        return;
    }
    if (args[2] == NULL)
    {
        puts("kaliShell: move: Destination not provided");
        return;
    }

    // Get last argument in args to later check if ends with quote ('"')
    char *lastArgument = NULL;
    for (int i = 1; args[i] != NULL; i++)
    {
        lastArgument = args[i];
    }

    // Allocate memory for file and destination folder (for file and folder names with spaces)
    char file[BUFF_SIZE];
    char destination[BUFF_SIZE];

    // Declare last file name index variable
    int lastFileIndex = 1;
    // Find last file name index if file starts with quote
    if (strncmp(args[1], "\"", 1) == 0 && args[1][strlen(args[1]) - 1] != '\"')
    {
        for (int i = 1; args[i][strlen(args[i]) - 1] != '\"' && args[i + 1] != NULL; i++)
        {
            lastFileIndex++;
        }

        // If last file name index reached last argument - invalid source name format
        if (args[lastFileIndex] == lastArgument)
        {
            puts("kaliShell: move: Invalid file name format");
            return;
        }
    }

    // Error handling if both file name and destination don't start with quotes ('"')
    if (strncmp(args[1], "\"", 1) != 0 && strncmp(args[2], "\"", 1) != 0)
    {
        // Any of them end with quote
        // Example inputs: 'move file" destination' or 'move file destination"'
        if (args[1][strlen(args[1]) - 1] == '\"' || args[2][strlen(args[2]) - 1] == '\"')
        {
            puts("kaliShell: move: Invalid file name format");
            return;
        }

        // More than three arguments inputed
        // Example input: move source destination hello
        if (args[3] != NULL)
        {
            puts("kaliShell: move: Too many arguments");
            return;
        }
    }

    // File name without quotes ('"') - File name without spaces
    // Does not start or end with quotes
    // Example input: move source destination
    if (strncmp(args[1], "\"", 1) != 0 && args[1][strlen(args[1]) - 1] != '\"')
    {
        // Copy file name provided in args[1] into file variable
        strcpy(file, args[1]);
    }
    // File name with quotes ('"') - File name with spaces
    // and is not a single argument file name (check with last source index)
    // Example input: move "file name" destination
    else if (strncmp(args[1], "\"", 1) == 0)
    {
        // Add first argument to file string excluding the first quote
        strcpy(file, args[1] + 1);

        // If current file string does not end with quote
        if (file[strlen(file) - 1] != '\"')
        {
            // Add more arguments to file string as long as previous argument did not end in quote
            for (int i = 2; args[i - 1][strlen(args[i - 1]) - 1] != '\"'; i++)
            {
                strcat(file, " ");     // Add space between arguments
                strcat(file, args[i]); // Add current argument
            }
        }

        // Error handling if file name does not end with quote
        if (file[strlen(file) - 1] != '\"')
        {
            puts("kaliShell: move: Invalid source file name format");
            return;
        }
        // Remove last quote ('"') from end of file string
        file[strlen(file) - 1] = '\0';

        printf("File: %s\n", file);

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
}

void logout()
{
    puts("Thank you for using Kali Shell! Goodbye :)");
    exit(EXIT_SUCCESS); // EXIT_SUCCESS = 0. EXIT_FAILURE = 1.
}