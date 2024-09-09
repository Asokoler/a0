#include <stdio.h>  // fprintf, stdout, stderr.
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.

int typeChecker(char *filename) {
        int retval = EXIT_FAILURE;
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen");
            int retval = EXIT_FAILURE;
            return retval;
        }
        else {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                fprintf(stdout, "%s", buffer);
                for (int i = 0; i < sizeof(buffer); i++) {
                //lav et loop er, som tjekker om den enkelte byte ligger i et bestemt interval
                    if(buffer[i]=="0x15"){ //det her skal være mere præcist
                        printf("This is a binary file");
                        break;
                    }
                };
            }
            printf("\n");
            retval = EXIT_SUCCESS;
        }
        fclose(file);
        return retval;
}

int main(int argc, char *argv[]) {
    int retval = EXIT_SUCCESS;
    if (argc == 2){
        retval = typeChecker(argv[1]);
    }    
    else {
        printf("Usage: %s <file>\n", argv[0]);
        retval = EXIT_FAILURE;
    }
    return retval;
}


