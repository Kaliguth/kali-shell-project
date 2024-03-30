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
        else if (strcmp(trimmedInput, "echo") == 0)
        {
            echo(arguments);
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
        // Input starts with delete
        else if (strcmp(trimmedInput, "delete") == 0)
        {
            delete (arguments);
        }
        // Handle piping
        else if (piping)
        {
            arguments[piping] = NULL;                  // Null terminate the first command's arguments at the pipe symbol
            mypipe(arguments, arguments + piping + 1); // Call the function to handle piping
            wait(NULL);                                // Wait for child processes to finish
        }
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

void welcome()
{
    puts("\n⠀⠀⠀⣠⡾⠟⠛⠦⣄⠀⠀⠀⠀⠀⠀⠀     ⠀⠀⣀⡴⠾⠛⠳⣦");
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