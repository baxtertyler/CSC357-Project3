#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

#define BLUE "\033[1;94m"
#define YELLOW "\033[1;93m"
#define RESET "\033[0m"

void printFileInformation(struct stat);
void printDirectoryInformation(DIR*);
void sigf(int);

int main() {
    char scannerInput[100], cwd[256], *tempDir;
    struct stat buffer;
    int status;
    DIR *directory;
    int x, i, rsp, f, timeCheck, timeCheck2, *sharedMem;
    pid_t forkPID, currentPID;
    time_t T;
    struct tm *tm;

    sharedMem = mmap(NULL, 100 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);

    currentPID = getpid();
    printf("parent PID: %d\n\n", currentPID);

    forkPID = fork();
    if(forkPID > 0) {
        for (f = 0; f <= 65; f++) { 
            signal(f, sigf);
        }
        /* parent*/
        while(1 < 2) {
            sleep(10);
            timeCheck = sharedMem[0];
            time(&T);
            tm = localtime(&T);
            timeCheck2 = tm->tm_sec;
            if(timeCheck2-timeCheck > 10 || (timeCheck > 50 && timeCheck2+60-timeCheck > 10)) {
                kill(0, 9);
                wait(0);
                break;
            }
        }
    }
    if (forkPID == 0) {
        for (f = 0; f <= 65; f++) { 
            signal(f, sigf);
        }
        while(1 < 2) {

            getcwd(cwd, 256);

            time(&T);
            tm = localtime(&T);
            sharedMem[0] = tm->tm_sec;

            printf(BLUE "stat prog ~%s" RESET "$ ", cwd);
            scanf("%s", scannerInput);

            if (scannerInput[0] == 'l' && scannerInput[1] == 'i' && scannerInput[2] == 's' && 
                scannerInput[3] == 't' && scannerInput[4] == '\0') {
                getcwd(cwd, 256);
                directory = opendir(cwd);
                printDirectoryInformation(directory);

            } else if (scannerInput[0] == 'q' && scannerInput[1] == '\0') {
                break;

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
    munmap(sharedMem, 100 * sizeof(int));
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

void sigf(int sig) {
    printf(" -> signal #%d failed\n", sig);
}
