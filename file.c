#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access, F_OK, R_OK.

int fileFormatInspector(FILE *file_p){
    int retval = EXIT_FAILURE;
    char buffer[BUFSIZ];
    while (fgets(buffer, sizeof(buffer), file_p) != NULL) {
        fprintf(stdout, "%s", buffer);
        for (unsigned long i = 0; i < strlen(buffer); i++) {
            if ((unsigned char)buffer[i] > 0x7F && (unsigned char)buffer[i] < 0xFF) {
                printf("This is a UTF8-*file\n");
                break;
                }
            else if ((unsigned char)buffer[i] > 0x7F) {
                printf("This is not an ascii *file\n"); 
                break;
                };
            };            
    };
        printf("\n");
        retval = EXIT_SUCCESS;
        return retval;
}

int print_error(char *path, int errnum) {
    fprintf(stdout, "%s: cannot determine (%s)\n",
    path, strerror(errnum));
    return EXIT_SUCCESS;
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
            FILE *file = fopen(argv[1], "r");
            retval = fileFormatInspector(file);
            fclose(file);
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
