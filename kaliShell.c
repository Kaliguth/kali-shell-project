// Main app file
#include "kaliShell.h"
#include "kaliFunctions.h"

// IMPORTANT!
// All commands are case sensitive and are only activated when lower cased
int main()
{
    welcome();

    while (1)
    {
        getLocation();
        char *input = getInputFromUser();
        char **arguments = splitArguments(input);
        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0)
        {
            logout(input);
        }
        else
        {
            printf("%s: command not found\n", input);
        }
        // char **arguments = splitArguments(input);

        free(arguments);
        free(input);
    }

    return 1;
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