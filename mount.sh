cd module/
make

sudo insmod mail_slot.ko


sudo mknod /dev/ms0 c 245 0
sudo chmod 666 /dev/ms0

sudo mknod /dev/ms1 c 245 1
sudo chmod 666 /dev/ms1

sudo mknod /dev/ms2 c 245 2
sudo chmod 666 /dev/ms2
