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
        char *input = getInputFromUser();
        char *trimmedInput = trimWhitespaces(input);
        char **arguments = splitArguments(trimmedInput);

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

        if (strcmp(trimmedInput, "exit") == 0 && arguments[1] == NULL)
        {
            logout();
            free(input);
            free(arguments);
        }
        else if (strcmp(trimmedInput, "ls") == 0)
        {
            ls();
        }
        else if (strcmp(trimmedInput, "cd") == 0)
        {
            cd(arguments);
        } else if (strcmp(trimmedInput, "cp") == 0) {
            cp(arguments);
        }
        else
        {
            // Command not found message:
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
            printf("%s: Command not found\n", allArguments);
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