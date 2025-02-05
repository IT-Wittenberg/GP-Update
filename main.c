#include <stdio.h>
#include <windows.h>

void RunGpupdateSilent();

int main() {
    RunGpupdateSilent();
    return 0;
}

void RunGpupdateSilent() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; // Versteckt das Terminalfenster

    ZeroMemory(&pi, sizeof(pi));

    // Befehl: Antwortet automatisch mit "N" auf die Abmeldeaufforderung
    char command[] = "cmd.exe /c echo N | gpupdate /force";

    // Prozess ohne sichtbares Fenster starten
    if (!CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        printf("Fehler beim Starten des Prozesses: %lu\n", GetLastError());
        return;
    }

    // Warten, bis der Prozess beendet ist
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Handles schließen
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

