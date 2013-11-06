#V2

linux 3.4.66

nützliche links:

  [HowToQemu](http://fedoraproject.org/wiki/Architectures/ARM/HowToQemu)
  
  [Qemu Netzwerk](http://qemu-buch.de/de/index.php/QEMU-KVM-Buch/_Netzwerkoptionen)

### Vorbereitung

Wie müssen nun die ARCH und CROSS_COMPILE Variablen gesetzt werden?
* CROSS_COMPILE=arm-linux-gnueabi ARCH=arm

##### Wie können Sie diese Variablen in Ihrer Shell automatisch setzen, sodass Sie die Variablen nicht ständig angeben müssen? 
* export VAR=Wert

##### Config erstellen
* make versatile_defconfig
* im menuconfig unter systemtype versatile express family auswählen
 * Versatile Express Cortex-A9x4 tile 
 * Device Tree support for Versatile Express platforms

##### Welche Gerätetreiber sind aktiviert? 
* MTD, RAMDisk Support, NetworkDevice Support, I2C Bus, SoundKarte Advanced, HID Devices (z.B. für USB Maus), USB, SDKartenSupport, IO MemmoryManagement Support,

##### Welche Konsolen sind konfiguriert? 
* FrameBuffer Console, Console Translations, Dummy Console, Serial_Amba_PL011_Console,

##### Deaktivieren Sie den 'Loadable Module Support' im Kernel und achten Sie auf die feste Einbindung der Treiber in den Kernel. 
* Ändert automatisch die einbindung von "Modul" zu "integriert"

##### Welche ARM Architekturen unterstützt QEMU? 
* qemu-system-arm -machine arm
 * vexpress-a9          ARM Versatile Express for Cortex-A9
 * vexpress-a15         ARM Versatile Express for Cortex-A15

(Cortex-A)

------------------------------------------------------------------
### Kernel ausführen

Starten Sie den Kernel mit initramfs im QEMU Emulator. Was sehen Sie?
* qemu-system-arm -M vexpress-a9 -kernel arch/arm/boot/zImage 

Damit Sie die Bootmeldungen des Kernels sehen können, muss der Kernel mit einem geeigneten console= Parameter gestartet werden. Warum benötigt der Kernel diesen Parameter, wie lautet dieser Parameter? 
* -serial stdio -append "console=ttyAMA0"

Benutzen Sie nun eine serielle Schnittstelle für die Konsole. Welche Konsole ist im Linux Kernel konfiguriert?
* console=ttyAMA0

Wie aktivieren Sie die Konsole unter Linux beim Booten?
* -append "console=tty1"

Wie können Sie sich die Konsole unter QEMU ausgeben lassen?net 

#####Experimentieren Sie auch mit der Konsolenausgabe von QEMU auf eine pty Schnittstelle von homer. Mittels minicom oder screen können Sie auf pty Schnittstellen zugreifen. 
* minicom
* screen

#####Zum Rebooten des Kernels in Qemu benutzen Sie dessen Monitorfunktionialität. 
* qemu system_reset

##### Keymap:
* Busybox hat eine spezielle binary  loadkeys ist hier nutzlos!
* Host: sudo dumpkmap > keymap
* Target: loadkmap < keymap

##### Welche Prozesse laufen? Wieviel Speicher (RAM) belegt das derzeitige System? 
* Ram Total 123Mb, frei 117Bb, => ~6mb
* ->

------------------------------------------------------------------
### Netzwerkkonfiguration:
##### Welche Netzwerkkarten sind in der Linux Kernel Konfiguration aktiviert?
* alle Möglichen (jetzt nichtmehr)

##### Welche Netzwerkkarten unterstützt QEMU? Welche brauchen Sie?
* qemu-system-arm -net nic,model=? -> smc91c111
* qemu-system-arm -M vexpress-a9 -net nic,model=? -> lan9118

##### Der Socket auf den ihr QEMU-Netzwerkinterface zugreifen muss lautet /tmp/vde2-tap0.ctl. 
Achten Sie auch darauf, beim Aufruf in QEMU eine 'eigene' Mac Adresse für Ihre Netzwerkkarte zu verwenden, sonst bekommen Sie keine eigene IP Adresse vom DHCP Server zugewiesen.
* qemu-system-arm -M vexpress-a9 -kernel zImage -net nic,model=lan9118,macaddr=00:00:00:00:00:16 -net vde,sock=/tmp/vde2-tap0.ctl

##### IP-Adresse via dhcp:
* Samplescripte von busybox nehmen und auf target kopieren/pfade (für binaries) ändern
* udhcpc -i eth0 -s etc/udhcpc/sample.script

##### Warum funktioniert ein ping auf die eigene zugewiesene IP-Adresse evtl. nicht? 
* nicht im gleichen Netz wie homer
    
######(Hinweis: Welches Interface gibt es neben eth0 noch?
* loopback



##### DNS Auflösung aktivieren

Die mitgelieferte statische Busybox-Binärdatei enthält nicht alle nötigen Bibliotheken zum Auflösen von DNS Namen. 
Dazu muss ein Teil der Toolchain-Libs in das initramfs kopiert werden: 

    mkdir lib/ 
    cp /usr/arm-linux-gnueabi/lib/libnss_{dns,files}* /usr/arm-linux-gnueabi/lib/{libresolv,ld-linux}* /usr/arm-linux-gnueabi/lib/libc.so.6 lib/ 

Aktivieren Sie den telnetd auf dem Target und loggen Sie sich vom Host ein.
* auf target: telnetd -l/bin/sh
* auf host: telnet 192.168.29.X

Automatisieren Sie alle Konfigurationsschritte geeignet in einem Skript. 
* ->

Mit dem nun funktionierenden Netzwerk auf unserem Target können wir den Web Server aktivieren, um Informationen über das eingebettete System vom Host aus abzufragen. 
Dazu legen Sie das Verzeichnis /www an, in welchem Sie eine Start-Datei index.html anlegen. 
Starten Sie auf dem Target den busybox httpd über den Befehl: usr/sbin/httpd -h /www/ &
Greifen Sie nun auf dem Host über einen Web-Browser auf das Target zu. 

##### Legen Sie das Verzeichnis /www/cgi-bin an und lassen Sie über zusätzliche Links auf Ihrer Startseite die folgenden dort von Ihnen gespeicherten cgi-scripts ablaufen:
Ausgabe Name und Version des Betriebssystems auf dem Target
* ->

Auslesen der CPU Info auf dem Target
* ->

Ausgabe der "uptime" des Target-Systems
* ->

Ausgabe der Netzwerk Konfiguration
* ->

Löschen der temporären Dateien unter /tmp auf dem Target.
* ->

... (Weitere Informationen, die Sie über das /proc und /sys Filesystem abfragen können) 
* ->

##### Was unterstützt der HTTP Server von busybox?

      ->

##### Welche grafischen Features lassen sich dazu im Browser darstellen (Javascript, Stylesheets usw.)
###### Bauen Sie grafisch ansprechende Ausgaben, wenn der Browser auf Ihr 'Device' zugreift. Die beiden Grafiken sollen Ihnen für eigene Ideen als Input dienen. 

------------------------------------------------------------------
### Shrinking Kernel and RootFS
Deaktivieren Sie nun im Kernel und RootFS (busybox) Treiber und Funktionen, die Sie nicht benötigen. Wenn Sie beim Aufbau des RootFS von einer minimalen busybox Konfiguration ausgegangen sind, müssen Sie am RootFS nicht viel ändern. Je nach ausgewählte Linux Kernel Konfiguration kann dieser durch schrittweise Deaktivierung der unnötigen Treiber klein geschrumpft werden, was sich - vor allem auf einem realen Device - auch in einer schnelleren Bootzeit bemerkbar macht. 

* Deaktivierte Treiber:
 * ->

* Größe davor/danach:

-----------------------------------------------------------------

erst im initfile init (initramfs) mit busybox verlinken
--install installiert busybox selbständig (in config einstellen)





