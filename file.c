#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access, F_OK, R_OK.

//Note: Not checking: 
//if (file == NULL) {return 0;}
//Not sure if needed

int print_error(char *path, int errnum) {
    fprintf(stdout, "%s: cannot determine (%s)\n", path, strerror(errnum));
    return 0;
}

int print_type(int type, const char* path) {
    const char * const FILE_TYPE_STRINGS[] = {
        "empty",
        "ASCII text",
        "UTF-8 Unicode text",
        "data"
    };
    printf("%s: %s\n", path, FILE_TYPE_STRINGS[type]);
    return 0;
}

int inspectEmpty(const char* path) {
    FILE *file = fopen(path, "r");
    int ch = fgetc(file);
    if (ch != EOF) {
        fclose(file);
        return 0; 
    } 
    fclose(file);
    return 1;
}

int inspectASCII(const char* path) {
    FILE *file = fopen(path, "r");
    int ch = fgetc(file);
    while (ch != EOF) {
        if ((ch >= 0x07 && ch <= 0x0D) || ch == 0x1B || (ch >= 0x20 && ch <= 0x7E)) {
            ch = fgetc(file);
        } else {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int inspectISOinASCII(const char* path) {
    FILE *file = fopen(path, "r");
    int ch = fgetc(file);
    while (ch != EOF) {
        if (ch >= 128 && ch <= 159) {
            ch = fgetc(file);
        } else {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int inspectUTF(const char* path) {
    FILE *file = fopen(path, "r");
    int ch = fgetc(file);
    int byteCounter = 1;
    while (ch != EOF) {
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
            for (int i = 0; i < (byteCounter-1); i++) {
                ch = fgetc(file);
                if ((ch <= 191) && (ch >= 128)) {
                    return 1;
                }
                else {
                    return 0;
                    break;
                }
        fclose(file);
        return 1;
    }
}

int main(int argc, char *argv[]) {
    int retval = EXIT_FAILURE;
    if (argc == 1) {
        fprintf(stderr, "Usage: file path\n");
    } else if (argc == 2) {
        if (access(argv[1], F_OK) != 0) {
            retval = print_error(argv[1], ENOENT);
        } else if (access(argv[1], R_OK) != 0) {
            retval = print_error(argv[1], EACCES);
        } else if ((access(argv[1], R_OK) == 0) && (access(argv[1], F_OK) == 0)) {
            int is_empty = inspectEmpty(argv[1]);
            int is_ascii = inspectASCII(argv[1]);
            int is_iso = inspectISOinASCII(argv[1]);

            if (is_empty) {
                print_type(0, argv[1]);
                retval = EXIT_SUCCESS;
            } else if (is_ascii) {
                if (is_iso) {
                    print_type(2, argv[1]);
                } else {
                    print_type(1, argv[1]);
                }
                retval = EXIT_SUCCESS;
            } else {
                print_type(3, argv[1]);
                retval = EXIT_SUCCESS;
            }
        } else {
            fprintf(stderr, "Error: Something went wrong while reading the file. Not sure what...\n");
        }
    } else {
        fprintf(stderr, "Usage: Too many arguments provided.\n");
    }
    return retval;
}
