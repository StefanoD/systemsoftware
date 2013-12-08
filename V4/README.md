#V4

\#include < asm/unistd.h > !!!!!!!!!

#### Usefull link
http://www.linuxforu.com/tag/linux-device-drivers-series/page/2/

#### Beispiel Ausgabe Treiber Registrierung
- insmod treiber.ko
-> Register driver successful! Majornumber: 253
- lsmod 
-> Module                  Size  Used by    Tainted: G  
   treiber                  790  0
- rmmod treiber
-> Deregister driver successful!

Dmesg:  Register driver successful! Majornumber: 253
        Deregister driver successful!

#### Testen Sie den Treiber mit dem Systemprogramm cat. Ihr Treiber liefert ständig »hello world« zurück. Warum?
Testen Sie den Treiber mit dem Systemprogramm cat. Ihr Treiber liefert ständig »hello world« zurück. Warum?

