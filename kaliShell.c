// Main app file
#include "kaliShell.h"
#include "kaliFunctions.h"
#include "assistFunctions.h"

// IMPORTANT!
// All commands are case sensitive and are only activated when lower cased
int main()
{
    welcome();

    while (1)
    {
        getLocation();
        // Pipe index variable
        int piping = 0;
        // Coolect user input into variable
        char *input = getInputFromUser();
        // Trim white spaces from input into a new variable
        char *trimmedInput = trimWhitespaces(input);
        // Split input into arguments array
        char **arguments = splitArguments(trimmedInput);

        // Check if there is pipe in input
        if (containsPipe(arguments))
        {
            // If it does, get the index of pipe inside arguments array
            int i = 0;
            while (arguments[i] != NULL)
            {
                if (strcmp(arguments[i], "|") == 0)
                {
                    piping = i;
                    break;
                }
                i++;
            }
        }

        // Count the number of arguments in input
        int numberOfArguments = 0;
        while (arguments[numberOfArguments] != NULL)
        {
            numberOfArguments++;
        }

        // If input is empty (no arguments), free all and continue to next iteration
        if (numberOfArguments == 0)
        {
            free(input);
            free(arguments);
            continue;
        }

        // Input is exit and is the only argument
        if (strcmp(trimmedInput, "exit") == 0 && arguments[1] == NULL)
        {
            logout();
            free(input);
            free(arguments);
        }
        // Input is ls and is the only argument
        else if (strcmp(trimmedInput, "ls") == 0 && arguments[1] == NULL)
        {
            ls();
        }
        // Input starts with echo
        else if (strcmp(trimmedInput, "echo") == 0)
        {
            // Regular echo - Print string
            if (!containsAppend(arguments) && !containsWrite(arguments))
            {
                echo(arguments);
            }
            // Echo append
            else if (containsAppend(arguments) && !containsWrite(arguments))
            {
                echoappend(arguments);
            }
            // Echo write
            else if (containsWrite(arguments) && !containsAppend(arguments))
            {
                echowrite(arguments);
            }
        }
        // Input starts with read
        else if (strcmp(trimmedInput, "read") == 0)
        {
            readfile(arguments);
        }
        // Input starts with wc
        else if (strcmp(trimmedInput, "wc") == 0)
        {
            wordcount(arguments);
        }
        // Input starts with cd
        else if (strcmp(trimmedInput, "cd") == 0)
        {
            cd(arguments);
        }
        // Input starts with cp
        else if (strcmp(trimmedInput, "cp") == 0)
        {
            cp(arguments);
        }
        // Input starts with rm
        else if (strcmp(trimmedInput, "rm") == 0)
        {
            rm(arguments);
        }
        // Input starts with mv
        else if (strcmp(trimmedInput, "mv") == 0)
        {
            move(arguments);
        }
        // Handle piping
        else if (piping)
        {
            arguments[piping] = NULL;                  // Remove pipe from first command
            mypipe(arguments, arguments + piping + 1); // Call mypipe function with first and second command
            wait(NULL);                                // Wait for child processes to finish
        }
        // Command not found block
        else
        {
            // Print command not found message
            // Create a string of all arguments
            char allArguments[100];
            // Copy first argument into it
            strcpy(allArguments, arguments[0]);

            // Add rest of arguments into allArguments string
            for (int i = 1; arguments[i] != NULL; i++)
            {
                strcat(allArguments, " ");          // Seperate by space
                strcat(allArguments, arguments[i]); // Add current argument to allArguments string
            }

            // Print all arguments for command not found message
            printf("kaliShell: %s: Command not found\n", allArguments);
        }

        free(arguments);
        free(input);
    }

    return 0;
}

// Welcome function
void welcome()
{
    puts("\n   ⣠⡾⠟⠛⠦⣄⠀⠀⠀⠀⠀⠀⠀     ⠀⠀⣀⡴⠾⠛⠳⣦");
    puts("⠀⠀⣰⡟⠀⢀⡀⠀⠈⠻⣶⣶⣾⣿⣟⣯⣽⠛⠿⣟⣻⣷⠚⠉⠀⡀⢀⠀⠘⡆");
    puts("⠀⠀⢹⡅⠀⣾⣧⢄⣴⡾⠙⢋⣥⡠⢿⣏⡿⠷⠀⢶⣌⡉⠀⠀⢳⣿⣿⡆⣸⡇");
    puts("⠀⠀⠘⣷⣦⣿⠇⠈⢁⣠⣴⣿⣵⡟⠚⢛⡏⠂⠰⣶⣦⡙⠳⢤⡀⠉⢿⣿⡟");
    puts("⠀⠀⠀⠻⣤⠴⢦⡴⣻⡇⠈⣡⠞⠁⡾⢛⡇⠘⢷⡈⢷⣄⠹⣧⡙⢦⡴⣾⠁");
    puts("⠀⠀⠀⡼⠁⣴⠏⡼⢻⣆⣴⡃⠀⣼⢁⡾⡿⠳⢤⣿⠆⣈⣧⡈⢻⡄⢻⡘⢦");
    puts("⠀⠀⣸⠁⣼⠏⣸⡇⣸⣏⣘⣉⣧⣉⠈⠁⠀⠀⠀⣠⣶⣽⣿⢽⡄⣿⠸⡇⠀⢧");
    puts("⠀⢰⠇⠀⣿⠀⢻⠁⣏⢯⣻⢿⣿⡙⡇⠀⠀⠀⠀⣿⣿⣾⣿⠞⠁⢸⠀⣷⠀⠘⣆⡀");
    puts("⠀⢸⠀⠀⣿⡄⢸⣀⠻⣄⣈⡓⢹⡟⠁⠀⠀⠀⠀⢹⡇⠀⠀⠀⠀⢸⠀⣿⡀⠀⣿⣷⣄⠀        _   __      _ _ ");
    puts("⠀⣼⠀⠀⣿⡇⠘⣟⠓⠠⠯⣁⣼⡇⠀⠀⠀⠀⠀⠘⠷⢦⡳⣦⢤⡾⢀⣿⡃⠀⣾⣿⡟⠀       | | / /     | (_)");
    puts("⠀⣿⠀⠀⠿⣿⡀⢹⡆⠴⣄⣨⠏⠀⠀⠀⠀⠀⠀⠀⡄⠈⠻⣿⠀⠃⢠⣿⠃⠀⣿⠉⠁⠀       | |/ /  __ _| |_ ");
    puts("⢸⢿⣆⠀⠀⢹⣷⣌⢻⣄⣙⣯⡀⢠⡀⠀⠀⠀⠀⢀⡇⠀⣰⣿⠗⣲⣿⣏⠀⠀⠋⠀⢰⡆       |    \\ / _` | | |");
    puts("⠀⠈⢿⣦⡀⠀⠿⢿⣿⣾⣯⣙⣷⡦⡟⢿⣏⣹⡿⠋⡤⣾⣿⣿⣾⠿⠛⠁⠀⡟⠀⠀⣼⠃       | |\\  \\ (_| | | |");
    puts("⠀⠀⠉⣿⣿⡄⠀⢸⢉⠉⡿⣯⡭⣽⡿⠀⢹⡟⠀⠀⣾⣿⣻⡿⠋⠀⡀⠀⣼⠃⠀⣸⣿⠀       \\_| \\_/\\__,_|_|_|");
    puts("⠀⠀⠀⢹⣿⡄⠀⠀⢸⡼⠁⠘⠷⣯⡥⠶⠛⠛⠲⠤⣤⡿⢯⡀⠳⡀⢉⣿⡇⠀⣠⣿⠏⠀     Welcome to Kali Shell!");
    puts("⠀⠀⠀⠘⣿⣿⣶⡶⠾⣧⣼⣧⣾⡁⠀⠀⠀⠀⠀⠀⠈⣷⠠⢓⣠⣿⡿⠋⢁⣴⣿⠋");
    puts("⠀⠀⠀⣄⢻⡍⠛⠿⣶⣤⣀⠈⠉⠓⠲⣤⣤⣄⣀⣴⠟⠛⠿⠛⠛⠛⢀⣴⣿⣿⠏");
    puts("⠀⠀⠀⠙⣷⡿⣄⠀⠀⠉⠛⢿⣶⣄⡀⠉⠉⠙⢿⡀⠀⠀⠀⠀⠀⣴⠞⣿⣿⡟");
    puts("⠀⠀⠀⠀⠈⢿⣌⠓⢦⣀⠀⠀⠉⠛⢿⣶⣄⠀⠀⠙⢦⡀⠀⡤⠞⠁⣴⣿⡿⠁");
    puts("⠀⠀⠀⠀⠀⠀⠙⢧⣀⠈⠙⠲⢤⣄⠀⠈⠻⢷⣦⠀⠀⠉⠓⠀⠀⢠⣿⠟⠁");
    puts("⠀⠀⠀⠀⠀⠀⠀⠈⠙⢷⣤⣀⠀⠈⠁⠀⠀⠀⢻⣿⣦⡀⠀⠀⣰⡿⠃");
    puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣷⣦⡀⠀⠀⠀⠹⣌⠛⠀⠀⡿⠃\n");
    puts("My Github: https://github.com/Kaliguth\n\nEnjoy!\n");
}