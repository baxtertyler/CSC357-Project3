#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BLUE "\033[1;94m"
#define YELLOW "\033[1;93m"
#define RESET "\033[0m"

void printFileInformation(struct stat);
void printDirectoryInformation(DIR*);

int main() {
    char scannerInput[100], cwd[256], *tempDir;
    struct stat buffer;
    int status;
    DIR *directory;
    int x, rsp;
    pid_t forkPID, currentPID;

    currentPID = getpid();
    printf("parent PID: %d\n\n", currentPID);

    forkPID = fork();
    while(1 < 2) {
        if(forkPID > 0) {
            /* parent*/
            wait(0);
            sleep(10);
            forkPID = fork();
        }
        if (forkPID == 0) {
            while(1 < 2) {
                getcwd(cwd, 256);
                printf(BLUE "stat prog ~%s" RESET "$ ", cwd);
                scanf("%s", scannerInput);

                if (scannerInput[0] == 'l' && scannerInput[1] == 'i' && scannerInput[2] == 's' && 
                    scannerInput[3] == 't' && scannerInput[4] == '\0') {
                    getcwd(cwd, 256);
                    directory = opendir(cwd);
                    printDirectoryInformation(directory);

                } else if (scannerInput[0] == 'q' && scannerInput[1] == '\0') {
                    printf("Exiting...");
                    return 0;

                } else if (scannerInput[0] == '.' && scannerInput[1] == '.' && scannerInput[2] == '\0') {
                    chdir("..");

                } else if (scannerInput[0] == '/') {
                    x = 0;
                    tempDir = malloc(256 * sizeof(char));
                    while(scannerInput[x+1] != '\0') {
                        tempDir[x] = scannerInput[x+1];
                        x++;
                    }
                    rsp = chdir(tempDir);
                    if(rsp == -1) {
                        printf("No such directory exists. Try again\n");
                    }
                    free(tempDir);

                } else {
                    status = stat(scannerInput, &buffer);
                    if (status != -1) {
                        printFileInformation(buffer);
                    } else {
                        printf("No such file exists. Try again\n");
                    }
                }
            }
        }
    }
    printf("Exiting...");
    return 0;
}


    

void printFileInformation(struct stat buffer) {
    printf("Input file information\n");
    printf(" ID of device with file:   %lu\n", buffer.st_dev);
    printf(" Inode number:             %lu\n", buffer.st_ino);
    printf(" File type and mode:       %u\n", buffer.st_mode);
    printf(" Link Count:               %u\n", buffer.st_nlink);
    printf(" User ID:                  %u\n", buffer.st_uid);
    printf(" Group ID:                 %u\n", buffer.st_gid);
    printf(" Inode Device:             %lu\n", buffer.st_dev);
    printf(" File size:                %lu\n", buffer.st_size);
    printf(" Prefered I/O block size:  %d\n", buffer.st_blksize);
    printf(" Blocks allocated to file: %lu\n", buffer.st_blocks);
}

void printDirectoryInformation(DIR* directory) {
    int i;
    struct dirent *entry;

    while ((entry = readdir(directory)) != NULL) {
        i++;
        printf("FILE %d: %s\n", i, entry->d_name);
    }
}