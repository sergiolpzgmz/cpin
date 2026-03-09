#include <stdio.h>
#include <string.h>


// ./cpin arg1 arg2 arg3 . . . -> ./cpin == arg0
// argc -> how many args are there (only ./cpin would be argc = 1)

// tools entry point 
int main(int argc, char** argv){
    // error handeling 
    if (argc < 2) {
        printf("Usage:\n");
        printf("  cpin add <file:line> \"<note>\"\n");
        printf("  cpin list <file>\n");
        printf("  cpin remove <file:line>\n");
        return 1;
    }


    //command parsing logic
    char* cmd = argv[1];

    if (!strcmp(cmd, "add")) {
        if (argc <= 3) {
            printf("Usage:\n");
            printf("  cpin add <file:line> \"<note>\"\n");
            return 1;
        }
        // Adding logic 
        char* token = strtok(argv[2], ":");
        char* file = token;
        token = strtok(NULL, ":");
        char* line = token;
        if (line==NULL) {
            printf("Error: use <file:line> format, e.g. src/parser.c:42");
            return 1;
        }
        printf("File: %s Line: %s", file, line);


        return 0;
    } else if (!strcmp(cmd, "list")) {
        printf("listing");
        return 0;
    } else if (!strcmp(cmd, "remove")) {
        printf("removing");
        return 0;
    } else {
        printf("Unknowen command!");
        printf("Usage:\n");
        printf("  cpin add <file:line> \"<note>\"\n");
        printf("  cpin list <file>\n");
        printf("  cpin remove <file:line>\n");
        return 1;
    }
}
