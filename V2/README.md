nützliche links:
http://fedoraproject.org/wiki/Architectures/ARM/HowToQemu


linux 3.4.66



Wie müssen nun die ARCH und CROSS_COMPILE Variablen gesetzt werden? 
# CROSS_COMPILE=arm-linux-gnueabi ARCH=arm

Wie können Sie diese Variablen in Ihrer Shell automatisch setzen, sodass Sie die Variablen nicht ständig angeben müssen? 
# export VAR=Wert

Config erstellen
#make versatile_defconfig
# im menuconfig unter systemtype versatile express family auswählen
# Versatile Express Cortex-A9x4 tile 
# Device Tree support for Versatile Express platforms

Welche Gerätetreiber sind aktiviert? 
# MTD, RAMDisk Support, NetworkDevice Support, I2C Bus, SoundKarte Advanced, HID Devices (z.B. für USB Maus), USB, SDKartenSupport, IO MemmoryManagement Support,

Welche Konsolen sind konfiguriert? 
# FrameBuffer Console, Console Translations, Dummy Console, Serial_Amba_PL011_Console,

Deaktivieren Sie den 'Loadable Module Support' im Kernel und achten Sie auf die feste Einbindung der Treiber in den Kernel. 
# Ändert automatisch die einbindung von "Modul" zu "integriert"

Welche ARM Architekturen unterstützt QEMU? 
# qemu-system-arm -machine arm
## vexpress-a9          ARM Versatile Express for Cortex-A9
## vexpress-a15         ARM Versatile Express for Cortex-A15

(Cortex-A)

Starten Sie den Kernel mit initramfs im QEMU Emulator. Was sehen Sie?
# qemu-system-arm -M vexpress-a9 -kernel arch/arm/boot/zImage 

Damit Sie die Bootmeldungen des Kernels sehen können, muss der Kernel mit einem geeigneten console= Parameter gestartet werden:
Warum benötigt der Kernel diesen Parameter, wie lautet dieser Parameter? 
# -serial stdio -append "console=ttyAMA0"

Benutzen Sie nun eine serielle Schnittstelle für die Konsole
Welche Konsole ist im Linux Kernel konfiguriert?
# console=ttyAMA0

Wie aktivieren Sie die Konsole unter Linux beim Booten?
# -append "console=tty1"

Wie können Sie sich die Konsole unter QEMU ausgeben lassen?
#

Experimentieren Sie auch mit der Konsolenausgabe von QEMU auf eine pty Schnittstelle von homer. Mittels minicom oder screen können Sie auf pty Schnittstellen zugreifen. 

minicom
screen

Zum Rebooten des Kernels in Qemu benutzen Sie dessen Monitorfunktionialität. 
# qemu system_reset


erst im initfile init (initramfs) mit busybox verlinken
--install installiert busybox selbständig (in config einstellen)





