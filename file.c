#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

int fileFormatInspector(char *filename) {
        int retval = EXIT_FAILURE;
        FILE *file = fopen(filename, "r");
        //Not sure if needed
        if (file == NULL) {
            int retval = EXIT_FAILURE;
            return retval;
        }
        else {
            char buffer[BUFSIZ];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                fprintf(stdout, "%s", buffer);
                for (unsigned long i = 0; i < strlen(buffer); i++) {
                    if ((unsigned char)buffer[i] > 0x7F && (unsigned char)buffer[i] < 0xFF) {
                        printf("This is a UTF8-file\n");
                        break;
                    }
                    else if ((unsigned char)buffer[i] > 0x7F) {
                        printf("This is not an ascii file\n"); 
                        break;
                    };
                    
                };
            };
        printf("\n");
        retval = EXIT_SUCCESS;
        fclose(file);
        return retval;
        };
}

int main(int argc, char *argv[]) {
    int retval = EXIT_SUCCESS;
    if (argc == 2){
        retval = fileFormatInspector(argv[1]);
    }    
    else {
        retval = EXIT_FAILURE;
    }
    return retval;
}


