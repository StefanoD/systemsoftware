#!/bin/sh
loadkmap < /etc/keymap
udhcpc -i eth0 -s /etc/udhcpc/sample.script
httpd -h /www/ &
exec /bin/sh
exit 0
