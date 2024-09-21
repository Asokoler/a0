#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access, F_OK, R_OK.


// Function to print error message if file cannot be opened or read.
int print_error(char *path, int errnum) {
    fprintf(stdout, "%s: cannot determine (%s)\n", path, strerror(errnum));
    return 0;
}
// Function to print the type of file.
int print_type(int type, const char* path) {
    // Array of strings to represent the file types.
    const char * const FILE_TYPE_STRINGS[] = {
        "empty",
        "ISO-8859 text",
        "ASCII text",
        "UTF-8 Unicode text",
        "data"
    };
    printf("%s: %s\n", path, FILE_TYPE_STRINGS[type]);
    return 0;
}

// Function to inspect if the file is empty.
int inspectEmpty(const char* path) {
    // Attempt to open the file in read mode.
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 0;
    }

    // Read the first character from the file and check if it is EOF.
    int ch = fgetc(file);
    if (ch != EOF) {
        fclose(file);
        return 0; 
    } 
    fclose(file);
    return 1;
}

// Function to inspect if the file is ASCII.
int inspectASCII(const char* path) {
    // Attempt to open the file in read mode.
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 0;
    }
    int ch = fgetc(file);
    //While the ch is not EOF check if the character is in the ASCII range.
    while (ch != EOF) {
        if ((ch >= 0x07 && ch <= 0x0D) || ch == 0x1B || (ch >= 0x20 && ch <= 0x7E)) {
            ch = fgetc(file);
        } 
        else {
            //As soon an non-ASCII character is found, close the file and return 0.
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

//Is meant to be run on files that are ASCII as to check if it is not only ASCII but also ISO-8859 text.
int inspectISOinASCII(const char* path) {
    // Attempt to open the file in read mode.
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 0;
    }
    int ch = fgetc(file);
    
    //While the ch is not EOF check if the character is in the ASCII range.
    while (ch != EOF) {
        if (ch >= 160 && ch <= 255) {
            ch = fgetc(file);
        } else {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

// Function to inspect if the file is UTF-8.
int inspectUTF(const char* path) {
    // Attempt to open the file in read mode.
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 0;
    }
    int ch = fgetc(file);
    
    int byteCounter = 1;
    // While the ch is not EOF check if the character is in the UTF-8 range.
    while (ch != EOF) {
        //Check if the character is in the UTF-8 range and set the byteCounter accordingly. The number of possible bytes in a UTF-8 character is determined by the first byte.
        if ((ch <= 127) && (ch > 0)) {
                byteCounter = 1;
        }
        else if ((ch <= 223) && (ch >= 192)) {
                byteCounter = 2;
        }
        else if ((ch <= 239) && (ch >= 224)) {
                byteCounter = 3;
        }
        else if ((ch >= 240) && (ch <= 247)) {
                byteCounter = 4;
        }
        //If the character is not in the UTF-8 range, close the file and return 0.
        else {
            fclose(file);
            return 0;
        }
        
        //Check if the following bytes are in the UTF-8 range.
        for (int i = 0; i < (byteCounter-1); i++) {
            //Move to the next character.
            ch = fgetc(file);
            //If the following bytes are not in the UTF-8 range, close the file and return 0.
            if ((ch <= 191) && (ch >= 128)) {
                continue;
            }
            else {
                fclose(file);
                return 0;
            }
        }
        //Move to the next character.
        ch = fgetc(file);
    };
    fclose(file);
    return 1;
}

// Main function to run the program.
int main(int argc, char *argv[]) {
    //Assume the program will fail.
    int retval = EXIT_FAILURE;
    if (argc == 1) {
        //Print error message if no arguments are provided.
        fprintf(stderr, "Usage: file path\n");
    } 
    else if (argc == 2) {
        //Check if the file exists
        if (access(argv[1], F_OK) != 0) {
            retval = print_error(argv[1], ENOENT);
        //Check if the file is readable.
        } 
        else if (access(argv[1], R_OK) != 0) {
            retval = print_error(argv[1], EACCES);
        }
        //Check if the file is readable and exists. 
        else if ((access(argv[1], R_OK) == 0) && (access(argv[1], F_OK) == 0)) {
            //Check if the file is empty, ASCII, ISO-8859 text, UTF-8 Unicode.
            int is_empty = inspectEmpty(argv[1]);
            int is_ascii = inspectASCII(argv[1]);
            int is_iso = inspectISOinASCII(argv[1]);
            int is_utf = inspectUTF(argv[1]);

            //Is empty is checked first
            if (is_empty) {
                print_type(0, argv[1]);
            }
            //If the file is not empty, check if it is ASCII.
            else if (is_ascii) {    
                if (is_iso) {
                    //If the file is ASCII, check if it is also ISO-8859 text.
                    print_type(1, argv[1]);
                } else {
                    print_type(2, argv[1]);
                }
            } 
            //If the file is not empty or pure ASCII or ISO, check if it is UTF-8 Unicode.
            else if (is_utf) {
                print_type(3, argv[1]);
            } 
            //If the file is not empty, ASCII, ISO or UTF-8, it must be data.
            else {
                print_type(4, argv[1]);
            }

            //If the file is read successfully, set the return value to success.
            retval = EXIT_SUCCESS;
        //A catch all error message if something goes wrong.
        } 
        else {
            fprintf(stderr, "Error: Something went wrong while reading the file. Not sure what...\n");
        }

    } 
    //If argv has more than 2 arguments (with one being the file name), print an error message.
    else {
        fprintf(stderr, "Usage: Too many arguments provided.\n");
    }

    return retval;
}
