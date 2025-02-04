# gpupdate.exe

gpupdate.exe ist eine Anwendung welche die Gruppenrichtlinien aktualisiert. 

# Warum eine eigene Anwendung
Die extra Anwendung für die Gruppenrichtlinien gibt es weil die Anwendung extra für unser Baramundi optimiert ist. 

# Beschreibung
Dieses Programm führt eine erzwungene Aktualisierung der Gruppenrichtlinien auf einem Windows-System durch (`gpupdate /force`) und protokolliert den gesamten Vorgang in einer Log-Datei. Es enthält eine Erfolgskontrolle und führt eine Log-Rotation durch, sobald die Log-Datei 10 MB erreicht.

# Funktionsweise

1. **Log-Datei schreiben:**
    - Das Programm protokolliert die Gruppenrichtlinienaktualisierung in `gpupdate.log`.
    - Jeder Eintrag enthält einen Zeitstempel, den Hostnamen und eine Statusmeldung.

2. **Log-Rotation:**
    - Wenn `gpupdate.log` größer als 10 MB wird, wird sie in `gpupdate_backup.log` umbenannt.
    - Eine neue `gpupdate.log` wird erstellt, um die weitere Protokollierung fortzusetzen.

3. **Erfolgskontrolle:**
    - Das Programm überprüft den Rückgabewert von `gpupdate /force`.
    - Erfolgreiche Aktualisierung: Log-Eintrag `Gruppenrichtlinien erfolgreich aktualisiert.`
    - Fehlgeschlagene Aktualisierung: Log-Eintrag `Fehler beim Aktualisieren der Gruppenrichtlinien. Fehlercode: <code>`

4. **Plattformübergreifende Unterstützung:**
    - Das Programm funktioniert sowohl auf Windows als auch auf Unix-basierten Systemen.
    - Es nutzt `#ifdef _WIN32`, um systemabhängigen Code zu implementieren.
    - Windows: `GetComputerNameA()` für den Hostnamen.
    - Linux/macOS: `gethostname()`.

# Verwendete Technologien

- **C-Programmierung**
- **Systemaufrufe (`system()`)**
- **Dateiverarbeitung (`fopen()`, `fclose()`, `fprintf()`)**
- **Speicherverwaltung (`snprintf()`)**
- **Zeitstempel (`time.h`, `strftime()`)**
- **Bedingte Kompilierung für Windows & Unix (`#ifdef _WIN32`)**

# Installation & Nutzung

## 1. Kompilieren des Codes:
- Windows (MinGW oder MSVC):    
```sh
	gcc -o gpupdate main.c
```
    
- Linux/macOS (obwohl `gpupdate` ein Windows-Befehl ist):
```sh
	gcc -o gpupdate main.c
```

## 2. Programm ausführen:
```sh
./gpupdate_check
```

# Log-Datei (`gpupdate.log`)
```yaml
2025-02-04 12:30:45 - HOST - USER - Starte Gruppenrichtlinienaktualisierung... 2025-02-04 12:30:50 - HOST - USER - Gruppenrichtlinien erfolgreich aktualisiert.
```

# Fehlermeldungen & Fehlercodes

Falls `gpupdate /force` fehlschlägt, wird der Fehlercode ins Log geschrieben, z. B.:
```yaml
2025-02-04 12:31:10 - MY-PC - Fehler beim Aktualisieren der Gruppenrichtlinien. Fehlercode: 1
```

Die Bedeutung der Fehlercodes kann durch `gpupdate /?` oder eine Online-Suche nach Windows-Fehlercodes überprüft werden.