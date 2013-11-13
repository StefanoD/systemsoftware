Systemsoftware
==============

Prüfungsrelevant:

unterschiede initramfs ramfs tempfs


# RAM-Dateisysteme

- Virtueller und temporärer Datenträger im RAM
- Wird benutzt wenn adere physische Speichermeiden zu langsam oder noch nicht verfügbar sind
- Beim anlegen wird ein Teil des Arbeitspeichers statisch oder dynamisch "abgezweigt" (reserviert)

### RAM-DISK: (veraltet)
- Feste Größe
- Wird mit einem Dateisystem formatiert, eingehangen und wie eine Festplatte angesprochen
- Wirft Fehlermeldung bei vollem Speicher
- Kein Swap

### ramfs
- Erweiterung von RAM-DISK
- Einfaches Filesystem
- Benutzt Linux Caching Mechanismen um Speicher dynamisch zu erweitern
- Wenn der RAM voll ist kann das System abstürzen
 
### tmpfs
- Erweiter ramfs und Limitierung und Swapping um das Vollschreiben des gesamten Speichers zu verhindern

### initrd (init RAM-DISK)
- Temporäres Dateisystem beim booten
- Nutzt eine RAM-DISK in die das Root-FS Image kopiert wird
- Kernel mounted RAM-DISK als Root (Kernel param initrd=)
- Kernel startet danach das Skript /linuxrc

### initramfs (init ramfs)
- Weiterentwicklung von initrd
- Erstellt keine RAM-Disk 
- Das initramfs ist ein komprimiertes Archiv, das für den Systemstart benötigte Dateien enthält
- Es kann vom Linux-Kernel beim Booten als Stammverzeichnis eingehängt werden

 




