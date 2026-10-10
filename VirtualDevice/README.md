sudo install -m 0644 69-virtual-macropad.rules /etc/udev/rules.d/69-virtual-macropad.rules
sudo modprobe uhid
sudo udevadm control --reload-rules
sudo udevadm trigger --action=add --subsystem-match=misc --sysname-match=uhid
sudo udevadm trigger --action=change --subsystem-match=hidraw --sysname-match=hidraw8

ls -l /dev/uhid
getfacl -p /dev/uhid
