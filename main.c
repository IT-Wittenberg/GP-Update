#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
    #include <Lmcons.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
#endif

// Define Log Konfiguration
#define LOG_FILE "gpupdate.log"
#define MAX_LOG_SIZE 10485760 // 10MB
#define BACKUP_LOG_FILE "gpupdate_backup.log"

// Funktionsprototypen
void get_hostname(char *hostname, size_t size);
void get_username(char *username, size_t size);
void log_message(const char *message);

int main(void) {
    log_message("Starte Gruppenrichtlinienaktualisierung...");
    int gp_result = system("echo N | gpupdate /force");
    
    if (gp_result == 0) {
        log_message("Gruppenrichtlinien erfolgreich aktualisiert.");
        printf("Gruppenrichtlinien erfolgreich aktualisiert.\n");
    } else {
        char error_message[150];
        snprintf(error_message, sizeof(error_message), "Fehler beim Aktualisieren der Gruppenrichtlinien. Fehlercode: %d", gp_result);
        log_message(error_message);
        printf("%s\n", error_message);
    }
    
    return gp_result;
}

void get_hostname(char *hostname, size_t size) {
    #ifdef _WIN32
        DWORD hostname_len = size;
        GetComputerNameA(hostname, &hostname_len);
    #else
        gethostname(hostname, size);
    #endif
}

void get_username(char *username, size_t size) {
    #ifdef _WIN32
        DWORD username_len = size;
        GetUserNameA(username, &username_len);
    #else
        struct passwd *pw = getpwuid(getuid());
        if (pw) {
            strncpy(username, pw->pw_name, size);
            username[size - 1] = '\0';
        } else {
            strncpy(username, "unknown", size);
        }
    #endif
}

void log_message(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) return;
    
    fseek(log_file, 0, SEEK_END);
    long file_size = ftell(log_file);
    
    if (file_size >= MAX_LOG_SIZE) {
        fclose(log_file);
        remove(BACKUP_LOG_FILE);
        rename(LOG_FILE, BACKUP_LOG_FILE);
        log_file = fopen(LOG_FILE, "a");
        if (!log_file) return;
    }
    
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    char hostname[256];
    get_hostname(hostname, sizeof(hostname));
    
    char username[256];
    get_username(username, sizeof(username));
    
    fprintf(log_file, "%s - %s - %s - %s\n", time_str, hostname, username, message);
    fclose(log_file);
}