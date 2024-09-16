#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access, F_OK, R_OK.



int print_error(char *path, int errnum) {
    fprintf(stdout, "%s: cannot determine (%s)\n",
    path, strerror(errnum));
    return 0;
}

int print_type(int type, const char* path) {
    const char * const FILE_TYPE_STRINGS[] = {
        "empty",
        "ASCII text",
        "UTF-8 Unicode text",
        "data"
    };
    printf("./%s\n%sfile: %s\n", path, FILE_TYPE_STRINGS[type], FILE_TYPE_STRINGS[type]);
    return 0;
}

int inspectEmpty(const char* path){
    FILE *file = fopen(path, "r");
    int ch = fgetc(file);
    if (ch == EOF) {
        return 1; 
        } 
    fclose(file);
    return 0;
}

int instpectASCII(const char* path){
    FILE *file = fopen(path, "r");
    //{0x07, 0x08, . . . 0x0D} ∪ {0x1B} ∪ {0x20, 0x21, . . . , 0x7E}
    int ch = fgetc(file);
    while (ch != EOF) {
        if ((ch >= 0x07 && ch <= 0x0D) || ch == 0x1B || (ch >= 0x20 && ch <= 0x7E)) {
            ch = fgetc(file);
        }
        else {
            return 0;
        }
    };
    return 1;
}

int inspectISO(const char* path){
    FILE *file = fopen(path, "r");
    //128–159
    int ch = fgetc(file);
    while (ch != EOF) {
        if (ch >= 128 && ch <= 159) {
            ch = fgetc(file);
        }
        else {
            return 0;
        }
    };   
    return 1;
} 

int inspectUTF8(const char* path){
    return 3;
}

int main(int argc, char *argv[]) {
    int retval = EXIT_FAILURE;
    if (argc == 1){
        fprintf(stderr, "Usage: file path\n");
    }

    else if (argc == 2){
        if (access(argv[1], F_OK) != 0) {
            retval = print_error(argv[1],ENOENT);
        }

        else if (access(argv[1], R_OK) != 0) {
            retval = print_error(argv[1],EACCES);
        }

        else if ((access(argv[1], R_OK) == 0) && (access(argv[1], F_OK) == 0)) {
            int is_empty = inspectEmpty(argv[1]);
            int is_ascii = instpectASCII(argv[1]);
            int is_iso = inspectISO(argv[1]);
            int is_utf8 = inspectUTF8(argv[1]);
            
            if (is_empty){
                print_type(0, argv[1]);
            }
            else if (is_ascii){
                if (is_iso){
                    print_type(2, argv[1]);
                }
                else {
                    print_type(1, argv[1]);
                }
            }
            else if (is_utf8){
                print_type(3, argv[1]);
            }
            else {
                print_type(0, argv[1]);
            }
        }
        else {
            fprintf(stderr, "Error: Something went wrong while reading the file. Not sure what... \n");
        }

    }
    else{
        fprintf(stderr, "Usage: Too many arguments provided.\n");
    }
    
    return retval;
}
