#V1 (Linux/i386 3.10.15)


## Kernel Source vorbereiten

##### Mit welchem Kommando kann man das Kernel Verzeichnis "säubern" ? Was ist der Unterschied zwischen den verschiedenen Kommandos zum Säubern des Kernel Verzeichnisses? 
*  make clean : config files werden beibehalten 
*  make mrproper : löscht radikal alles 
*  make distclean : löscht alle Dateien von ./configure und make
    
##### Wie muss das Konfigurationsprogramm korrekt aufgerufen werden, damit wir für die i386 Architektur das Target konfigurieren können?
* ARCH=i386 make


## Kernel Code konfigurieren

##### Wie kann man nach Begriffen 'suchen'?
* über h: hilfe  und der Suchfunktion

##### Wie sind die Subsysteme des Kernel gruppiert? 
* nach typ und funktion

##### Welche Parameter sind für den Kernel zwingend nötig?
* CPU Architektur ARCH, Anzahl Kerne, ...

## Kernel Compilieren

##### Beobachten Sie, aus welchen Verzeichnissen die Dateien kompiliert wurden und welche Outputs erzeugt werden.
* Kernel Verzeichnis

##### Wie gross ist der Startup-Code, wie gross ist das Kernel-Image? Welchen CRC Code erstellt der Kernel-Build Prozess?
* Größe Image:  
* Größe Startup Code:   
* Image und zImage

##### Überprüfen Sie auch, ob ccache benutzt wird. 
* ja

##### Untersuchen Sie das Verzeichnis, in welchem der Kompilierungsprozess die verschiedenen Boot-Dateien angelegt hat. Von welchem Typ sind die einzelnen Dateien
* binary

###### Welchen Inhalt haben die einzelnen Dateien (Tipp: Programm file).
* code

###### Welche Images stehen nach einem erfolgreichen Durchlauf zur Verfügung?
* Image und zImage (komprimiert)

###### Vergleichen Sie den Inhalt des Verzeichnisses mit den in der Vorlesung besprochenen Output Dateien. 
* ->

## Kernel Booten
* qemu-system-i368 -kernel zImage  bis zum nicht vorhandenen init script
* qemu-system-i386 -kernel zImage -qmp tcp:localhost:4444,server,nowait

## Eigenes Iinit Programm
* benötigt /dev/Console
* Kernel hängt sich danach auf weil kein weiterer Code zum ausführen da ist

## Root FS erstellen
* Starten von busybox mit  qemu:   qemu-system-i386 busybox
* dmseg schreibt den kernelbuffer als konsolenausgabe
* liefert Fehlermeldungen und Hardwareinformationen
 
##### Mounten Sie das /proc und /sys Filesystem.Wieviele Prozesse laufen? 
* ->

##### Mounten Sie /tmp als TMP-FS.Wenn Sie Dateien im /tmp Verzeichnis des Targets anlegen, wo sind diese Dateien nach einem Reboot? Warum? 
* Weg, da nur temporär in den Arbeitsspeicher geschrieben, nach neustart ist dieser leer

##### Wieviel Speicher steht Ihnen im laufenden System zu Verfügung?
* gemäß quota

##### Wieviel Speicher muss der Emulator dem Linux System mindestens zu Verfügung stellen, so dass der Linux Kernel korrekt bootet? 
* ->

##### Experimentieren Sie mit mdev. Wie können Sie beim Booten das /dev Verzeichnis durch mdev füllen? 
* mdev füllt alle Verzeichnisse mit den entsprechenden devices (wenn vorher configuriert).  Vorher /proc und /sys mounten
