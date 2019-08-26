#!/bin/bash

for i in `ls /dev/ | grep -i ttyacm`; do echo $i && udevadm info -a -p $(udevadm info -q path -n /dev/$i) | grep -i serial | grep -v usb; done | sed -e '/^tty*/N' -e 's/\n//' > /tmp/udevout && for i in `cat /etc/serials`; do grep $i /tmp/udevout; done | awk '{print "/dev/"$1}' > /tmp/panel_order
