# V3

##### Was bewirkt das Kommando »make source«?
* herunterladen aller angegeben Pakete für die Tool-Chain, Kernel, Busybox

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
* https://github.com/roroettg/systemsoftware/blob/master/V3/Makefile verwendet

##### Wo befinden sich die Dateien des Root-Filesystems? 
* buildroot/output/images

##### Starten Sie das erzeugte System im Emulator (qemu-system-arm). Vergessen Sie nicht die Optionen für die verwendete Hardware (Option -M versatilepb) und die Speichergröße (Option -m) mit anzugeben.
* qemu-system-arm -m 128 -M versatilepb -kernel output/images/zImage -initrd output/images/rootfs.cpio -serial stdio -append "console=ttyAMA0"

    
##### Messen Sie aus, wie lange Ihr System zum Booten benötigt.
* ca 5sec
    
##### Schauen Sie sich etwas im System um.
es ist nichts da


###### Welche Prozesse laufen aktuell?
init
-sh (shell)
kernel worker/ threads
klogd (Kernel log daemon)
syslogd (Systme log daemon


###### Wieviel Hauptspeicher steht zur Verfügung? 
128MB (qemu param)
System nutzt ca. 9MB

##### Gibt es eine Userverwaltung?
Ja, es gibt den User Root


###### Auf wieviel Festplatte kann zugegriffen werden?
0

##### Unterstützte Netzwerkkarten versatilepb
qemu-system-arm -M versatilepb -net nic,model=?
qemu: Supported NIC models: ne2k_pci,i82551,i82557b,i82559er,rtl8139,e1000,pcnet,virtio


#### Kernel Shrinking
Bootzeit: 1,5 Sekunde
Festplattenbelegung: 5 MB

#### Wie lautet der Name des Cross-Compilers für die in V3 verwendete ARM-Plattform?
arm-none-linux-gnueabi-gcc





