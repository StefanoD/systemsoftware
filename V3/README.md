# V3

##### Was bewirkt das Kommando »make source«?
* herunterladen und erstellen der Toolchain und aller benötigten Pakete

##### In welchem Verzeichnis finden sich die heruntergeladenen Pakete?
buildroot/dl

##### In welchem Verzeichnis befinden sich die ausgepackten Quellcodes?
* buildroot/output

##### Welche Pakete benutzt buildroot für die Crosstool-Chain?
* ls buildroot/package/
##### Welche Pakete installiert buildroot für das Target?

##### In welches Verzeichnis legt buildroot die generierten Imagedateien ab?
* buildroot/output/images

##### In welchem Verzeichnis befindet sich die generierte Cross-Toolchain?
* buildroot/toolchain

##### Wo befinden sich die Dateien des Root-Filesystems? 
* buildroot/output/images

##### Starten Sie das erzeugte System im Emulator (qemu-system-arm). Vergessen Sie nicht die Optionen für die verwendete Hardware (Option -M versatilepb) und die Speichergröße (Option -m) mit anzugeben.
* qemu-system-arm -m 128 -M versatilepb -kernel output/images/zImage -initrd output/images/rootfs.cpio -serial stdio -append "console=ttyAMA0"

    
##### Messen Sie aus, wie lange Ihr System zum Booten benötigt.
* ca 5sec
    
##### Schauen Sie sich etwas im System um. 


###### Welche Prozesse laufen aktuell? 


###### Wieviel Hauptspeicher steht zur Verfügung? 


###### Gibt es eine Userverwaltung? 


###### Auf wieviel Festplatte kann zugegriffen werden? ... 

