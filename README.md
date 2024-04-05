# Kali Shell project

### Project created by ***Einav Kohavi***

Welcome to Kali Shell!</br>
In here I will describe the purpose and functionalities of the project.</br>
Kali Shell is an application built using `C programming language` in `Visual Studio Code` environment.</br>

**The program includes error handling and appropriate messages to any error encountered.**

</br>

---

## Files

Kali shell's files include:
* `kaliShell.c` - The program's main file using all of Kali Shell's main functions. Welcome function is also implemented here.
* `kaliShell.h` - Main file's header file. Includes all needed libraries and the welcome function declaration.
* `kaliFunctions.c` - The program's functions file. Includes all of the program's main functions' implementation.
* `kaliFunctions.h` - Functions file's header file. Includes all needed libraries and declares used variables and functions.
* `assistFunctions.c` - This file implements different assist functions used in the program.
* `assistFunctions.h` - Assist functions file's header file. Includes all needed libraries and declares it's functions.
* `makefile` - Makefile responsible for building and running the application by creating object files and a main executable file.
* `Files for testing` - Different text files and folders to try out functions.

</br>

---

## Main functions

Kali Shell simulates a unix shell command prompt.</br>
It implements not all, but some of the original shell's commands, such as:
* `cd` - Folder navigation.
* `cp` - Copy files.
* `delete` - Delete files.
* `pipe` - Piping commands.
* `ls` - List folder's files and directories.
* `grep` - Filter lines containing given string. **Not used in main**
* `move` - Move files to a different folder.
* `echo` - Print given string.
* `echoappend` & `echowrite` - Append text or overwrite text for text files.
* `readfile` - Read and print contents of text files.
* `wordcount` - Count words or lines in given text files.
* `logout` - Finally, an exit command to terminate the application

</br>

---

## Functionalities, assist functions and explanations

**Every function includes error handling and returns error messages accordingly when errors are encountered**

</br>

### Welcome

The `welcome` function is implemented in the program's main file.</br>

The welcome function prints a message when first running the program.</br>
The message includes a dot art logo, a welcome message and a link to my github.</br>

This function is used in the `main` function.

</br>

### Main

The main function uses the `welcome` function once and then a while loop starts.</br>

Each iteration of the while loop a function called `getLocation` prints a line showing the current username, host name and the current folder path (just like cmd, bash and terminal programs).</br>

After that, a variable for pipe index `piping` is created to later find the index of a pipe symbol if found (to use a piping function).</br>

Function `getInputFromUser` is used to collect the user input into the `input` variable, it is then stripped of leading and trailing spaces into the `trimmedInput` variable using `trimWhitepaces` assist function, and after that the `arguments` variable is created to split the given input into different arguments split by spaces using the `splitArguments` function.</br>

The next part checks if any of the arguments is a pipe symbol and changes `piping` variable to be the index of it, if found.</br>

A `numberOfArguments` variable is created to count the number of arguments. If `arguments` array is empty, `input` and `arguments` variables are freed from the memory and the current iteration is skipped. No message is outputed and no function is being used.</br>

The main part of this function checks if the input starts with any of the known command callouts (cd, cp, ls etc.) or if pipe symbol is found in the arguments and uses required command functions as needed.</br>

If the first argument is not one of the programmed commands, a `Command not found` message is outputed with the input given.</br>

At the end of the iteration, `input` and `arguments` variables are freed before the next iteration.</br>

The while loop only ends if the user inputed the `exit` command (one argument only) or if the program is forcefully terminated.

</br>

### Get location

`getLocation` function is implemented in the functions file.</br>

It collects the current username, host name and location path of the user running the program and prints them in order and color of the original unix terminal.</br>

This function is used in the `main` function to print the start of the command line.

</br>

### Get input from user

`getInputFromUser` function is implemented in the functions file.</br>

It reads the input from the user character by character until nextLine is encountered (enter pressed).</br>

The first char read is allocated into the `str` string variable using `malloc`.</br>
A while loop then adds the rest of the characters ahead of `str` to create a collection of chars, while resizing the `str` variable accordingly, using `realloc`.</br>

`\0` is then added to the end of `str` to declare the end of the string.</br>
The function finally returns the `str` containing the entire input.</br>

This function is used in the `main` function to get an input from the user.

</br>

### Split arguments

`splitArguments` function is implemented in the functions file.</br>

This function gets the input given as string `str` and splits it by spaces to indicate different arguments.</br>

It first replaces all spaces with `\0` to indicate the end of each argument, then adds each of them in order into `subStr` variable.</br>

Each argument is added by using `subStr` into a new `arguments` array. The first argument is added and then a while loop adds the rest of them.</br>

Finally, `arguments` is returned.</br>

This function is used in the `main` function to split the input into different arguments for the commands.

</br>

### Echo

`echo` function is implemented in the functions file.</br>

This function gets an `arguments` array and prints all of them using a while loop.</br>

This function is used when the input starts with `echo`.

</br>

## List folder

`ls` function is implemented in the functions file.</br>

This function opens the current working folder and prints all files and folders inside it.</br>

The function first creates a `DIR` variable called `dir` and attempts to open the current working directory into it.</br>
It then reads it's contents into `entry` variable.</br>

A while loop starts, prints each entry (file or folder) inside it in it's own line.</br>

Finally, the function closes the directory in the `dir` variable.</br>

This function is used when the input starts with `ls` and no more arguments are found after it.</br>

**This function was implemented for testing purposes only**

</br>

### Grep

`grep` function is implemented in the functions file.</br>

This function gets a string, goes over all of the former command's return value (e.g: ls function returning folder contents line by line) and prints the lines including given string.</br>

**This function was implemented for testing purposes only and is not used in the main function**

</br>

### Change directory

`cd` function is implemented in the functions file.</br>

This function navigates to given path or folder name.</br>

It first collects the current working directory into `currentPath` variable.</br>
A new variable `newPath` is created for the new path to navigate to.</br>

If the next argument after `cd` is `.`, the command will remain in the same directory.</br>
If the next argument after `cd` is `..`, the command will navigate to the previous directory by removing the part after the last slash in the current working directory path.</br>

The function checks if the folder is given as a folder name or full folder path, quotes wrapping it or not, and if it contains spaces or not.</br>

If the folder name or path starts with quotes, it most likely means they contain spaces.</br>
The function then collects the entire string inside the quotes and removes the quotes from it.</br>

If a folder is given as a folder name, it will attempt to find the folder name in the current working directory and navigate to it by adding `./` to the beginning.</br>

If a folder is given as full path, it will attempt to find the path and navigate to it directly.</br>

This function is used when the input starts with `cd`.

</br>

### Copy files

`cp` function is implemented in the functions file.</br>

This function copies a given file to the given destination file.</br>

The function declares variables for source `src`, destination `des`, and `ch` for characters in the file.</br>

Just like `cd`, the function checks if the source file and destination file are given as a name or full path, quotes wrapping it or not, and if it contains spaces or not.</br>
It also tries to open the files as a full path and if it does not succeed, it tries to find and open them from the current working directory by adding `./` to the beginning.

After indicating whether path or name was given, it opens the source file for reading and the destination file for writing.</br>

Finally, goes over source file characters and writes them one by one into the destination file.</br>

This function is used when the input starts with `cp`.

</br>

### Delete files

`rm` function is implemented in the functions file.</br>

This function deletes a given file or folder.</br>

The function declares a variable `path` to add the the path of the file to delete into it.</br>

It then checks if the file or folder is given as full path or name, quotes wrapping it or not, and if it contains spaces or not.</br>

Adds appropriate beginning if not a path (`./` for current working directory, else `/` for full path).</br>

The function then attempts to delete the file or folder.</br>

This function is used when the input starts with `rm`.

</br>

### Pipe function

`mypipe` function is implemented in the functions file.</br>

This function is called if the input contains a pipe symbol, and creates 2 child processes out of the parent to run the two commands on one another.</br>

The function uses a file descriptors array `fileDescs` with a size of 2, attempts to pipe and fork the commands into 2 child processes and runs them.</br>

The first command runs, then the seconds commands reads the result returned from it and attempts to make changes on it accordingly.</br>

For example: The function `ls | grep hello` - `ls` runs to show the contents of the current working folder, then `grep` reads the result of `ls` and filters it to show lines containing the string `hello`.</br>

This function waits for all child processes to finish their job before ending.</br>

This function is used when the input contains `pipe symbols (|)`.

</br>

### Moving files

`move` function is implemented in the functions file.</br>

This function moves files given to given path.</br>

The function declares variables for source file `file` and destination path `destination`.</br>

It then checks if the files or folders are given as full path or name, quotes wrapping it or not, and if it contains spaces or not.</br>

It puts both file and destination into their variables.</br>
If file was given as a path, it gets the file name out of it by removing the path before the last slash (`/`), and then creates a new path for the file in the new `newDest` variable by adding `destination` to it followed by the `file` name.</br>

Finally, the function attempts to move the file to the new destination using the `rename` function.</br>

This function is used when the input starts with `mv`.

</br>

### Echo append

`echoappend` function is implemented in the functions file.</br>

This function adds inputed text to a given file.</br>

This function is called if the `echo` command is followed by an append symbol after it (`>>`).</br>

The function first checks the location of the append symbol to know where the text ends and file name or path starts.</br>
It then declares variables for the text `text` and file path `filePath`.</br>

The function then checks if text or path were given with spaces and adds them to `text` and `filePath` variables.</br>

A variable `file` is created to open the file in append mode.</br>

Finally, the function attempts to append the text into the file and closes it.</br>

This function is used when the input starts with `echo >>`.

</br>

### Echo write

`echowrite` function is implemented in the functions file.</br>

his function overwrites inputed text to a given file.</br>

This function is called if the `echo` command is followed by a write symbol after it (`>`).</br>

The function first checks the location of the write symbol to know where the text ends and file name or path starts.</br>
It then declares variables for the text `text` and file path `filePath`.</br>

The function then checks if text or path were given with spaces and adds them to `text` and `filePath` variables.</br>

A variable `file` is created to open the file in write mode.</br>

Finally, the function attempts to write the text into the file and closes it.</br>

This function is used when the input starts with `echo >`.

</br>

### Read files

`readfile` function is implemented in the functions file.</br>

This function gets a file name or path and shows it's contents.

The function first declares a variable for the file path `filePath`</br>
It then checks if file was given with spaces and adds it to `filePath` variable.</br>

The function creates the `file` variable and attempts to open the file in read mode.</br>

Finally, the function prints the contents of the file character by characters and closes it.</br>

This function is used when the input starts with `read`.

</br>

### Count words or lines

`wordcount` function is implemented in the functions file.</br>

This function counts words or lines in a given file.</br>

The function first creates a variable for file path `filePath` to add the file path into it.</br>

It then checks if which parameter is asked to be counted (`-w` for words or `-l` for lines).</br>
If none of them are provided, an error message is returned.</br>

The function checks if the file is given as file name or full path.</br>

A variable named `file` is created to open the file in read mode.</br>
The function then counts lines or words according to the option provided.</br>

Finally, the appropriate result is returned.</br>

This function is used when the input starts with `wc`.

</br>

### Exit the program

`logout` function is implemeneted in the functions file.</br>

This function allows the user to terminate the program safely.

The function is called when the user inputs `exit`.</br>

The user is presented with a `Thank you and goodbye` message and the program terminates.</br>

This function is used when the input starts with `exit` and no more arguments are found after it.

</br>

### Trim white spaces

`trimWhitespaces` function is implemented in the assist functions file.</br>

This function trims a given string (specifically the user's `input`) of white spaces before and after the command.</br>

The function first trims leading spaces, then trailing spaces accordingly and returns the string.</br>

This function is used in the `main` function to trim white spaces from it.

</br>

### Contains slash

`containsSlash` function is implemented in the assist functions file.</br>

This function checks if a given string contains a slash inside it.</br>

The function is used in some of the main functions to indicate if the given input file or folder is a path.</br>
Returns a boolean.

</br>

### Contains pipe

`containsPipe` function is implemented in the assist functions file.</br>

This function checks if a given string (specifically the user's `input`) contains a pipe symbol inside it.</br>

This function is used in the main function to check if the user's command contains a pipe to indicate if the `mypipe` function is required.
Returns a boolean.

</br>

### Contains append

`containsAppend` function is implemeneted in the assist functions file.</br>

This function checks if a given string (specifically the user's `input`) contains an append symbol inside it.</br>

This function is used in the main function to check if the user's echo command contains an append symbol to indicate which echo function is required.
Returns a boolean.

</br>

### Contains write

`containsWrite` function is implemeneted in the assist functions file.</br>

This function checks if a given string (specifically the user's `input`) contains a write symbol inside it.</br>

This function is used in the main function to check if the user's echo command contains a write symbol to indicate which echo function is required.
Returns a boolean.

</br>

---

# Thank you for viewing my project and giving feedback! c:

---
