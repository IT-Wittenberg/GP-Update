#include <stdio.h>
#include <stdlib.h>


int verbindungstest(char TARGET_HOST_NAME[], char TARGET_HOST_IP[]) {
    char command[50];

    snprintf(command, sizeof(command), "ping -n 1 %s > nul 2>&1", TARGET_HOST_NAME);
    int conTestHost = system(command);

    snprintf(command, sizeof(command), "ping -n 1 %s > nul 2>&1", TARGET_HOST_IP);
    int conTestIP = system(command);

    if (conTestHost == 0 || conTestIP == 0) return 0;
    else return 50;

}

int main(int argc, char *argv[]) {
    if (argc != 3) return 100;
    if (verbindungstest(argv[1], argv[2]) != 0) {
        return 50;
    }

    if (system("echo N | gpupdate /force") != 0) {
        return 104;
    }

    return 0;
}