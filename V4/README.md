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

Als Charackter Device registriert:
# cat /proc/devices 
Character devices:
  1 mem
  2 pty
  3 ttyp
  4 /dev/vc/0
  4 tty
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
  7 vcs
 10 misc
 13 input
 29 fb
 90 mtd
128 ptm
136 pts
204 ttyAMA
253 TestDriver
254 bsg
