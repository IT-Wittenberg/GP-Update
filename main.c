#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void logAdd(char *text, char *host);

const int globalDelay = 2; // set delay between next trys 
const int maxTryOpenFile = 3; // open the file for writing try "maxTryOpenFile" times

int main(void) {
    // get client hostname
    system("hostname > t.txt");
    FILE *pFp = fopen("t.txt", "r");
    char hostname[255]; // hold 1 Line
    fgets(hostname, 255, pFp); // read line 1
    fclose(pFp);
    sleep(globalDelay);
    system("del t.txt");

    logAdd("Starte GP-Update", hostname);
    
    int gp_result = system("echo N | gpupdate /force");

    if (gp_result == 0) {
        logAdd("GP Update erfolgreich ausgefuehrt", hostname);
    } else {
        logAdd("Etwas ist schief gelaufen. UNBEKANNTER FEHLER", hostname);
        return gp_result;
    }
    
    return 0;
}

void logAdd(char *text, char *host) {
    // Logfile handling
    char *logfile = "\\\\stadtwbbaram\\dip$\\Apl\\Custom Programs\\Logs\\gp_update.log";
    // char *logfile = getenv("LOGPATH");

    for(int i = 0; i < maxTryOpenFile; i++) {
        FILE *pFp = fopen(logfile, "a");

        if (pFp == NULL) {
            // cant open file
            printf("Error opening the file %s\n", logfile);
            // wait x secconds until next try
            sleep(globalDelay);
            if (i == maxTryOpenFile-1) printf("Limit reached. No log entry created\n");
        } else {
            // set timestamp
            const time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            // create log
            fprintf(pFp, "%d-%02d-%02d %02d:%02d - { %s", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, host);
            fprintf(pFp, "                           %s }\n", text);
            fclose(pFp);
            i = maxTryOpenFile;
        }
    }
}

