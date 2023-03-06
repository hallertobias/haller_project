# haller_project

## Beschreibung
Simulation einer Digest-baiserte Authentifizierung auf einer Website. Dabei werden ein Client und ein Server simuliert, auf der Client-Seite werden die Anmeldedaten per Konsole oder über eine Config-Datei eingelesen. Nach dem Authentifizierungsprozess beendet sich der Client und der Server ist für eine neue Anfrage bereit.

## Verwendung
Zuerst muss der Server gestartet werden. 

Usage: server

Dannach kann ein Client gestartet werden, der sich authentifizieren will.

Usage: client [OPTIONS]

| Option                   | Typ                           | Bedingungen             | Beschreibung                                 |
| ------------------------ | ----------------------------- | ----------------------- | -------------------------------------------- |
| -h,--help                | FLAG                          |                         | Ausgabe der Hilfe                            |
| -p, --port               | FLAG                          |                         | Eingabe des Server Ports (Standard: 1113)    |
| -c, --credentials        | FLAG                          |                         | Eingabe der Anmeldeinformationen             |

## Issues
Hier sind die [Issues](/../../issues)

## Libaries
- [CLI11](https://github.com/CLIUtils/CLI11)
- [spdlog](https://github.com/gabime/spdlog)
- [md5-Hashing](https://github.com/stbrumme/hash-library)
