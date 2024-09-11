#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

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


int main(int argc, char *argv[]) {
    int retval = EXIT_FAILURE;
    if (argc == 2){
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fclose(file);
            return retval;
        }
        else {
            retval = fileFormatInspector(file);
            fclose(file);
        }
    }    
    else {
        return retval;
    }
    return retval;
}


