#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./src/shell.h"

#define BUFSIZE 128

int testsFailed = 0;

int parse_output() {
    char* cmd = "find ./testing -type f -name *.memfile";

    char buf[BUFSIZE] = { 0 };
    FILE* fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        printf("%s\n", buf);
        char run[BUFSIZE] = "./src/sim ";
        strcat(run, buf);
        system(run);
        //system("g\n");
        //check for success or fail
        //int testReg = CURRENT_STATE.REGS[10];
        //if(testReg == 17)
        //    testsFailed++;
        //system("q\n");
        //write(STDIN_FILENO, "\ng\n", BUFSIZE);
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
    printf("\n\nTests Failed : %d\n\n", testsFailed);
    return 0;
}