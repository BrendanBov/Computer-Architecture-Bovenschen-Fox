#include <stdio.h>
#include <string.h>

#define BUFSIZE 128

int parse_output() {
    char *cmd = "find ./testing -type f -name *.memfile";    
    
    char buf[BUFSIZE] = {0};
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        // Do whatever you want here...
        //printf("OUTPUT: %s", buf);
        //system(strcat("./src/sim ",buf));
        char run[BUFSIZE] = "./src/sim ";
        strcat(run,buf);
        system(run);
        system("go");
    }

    if (pclose(fp)) {
        printf("Command not found or exited with error status\n");
        return -1;
    }

    return 0;
}

int main()
{
    parse_output();
    return 0;
}