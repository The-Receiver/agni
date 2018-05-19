rm -rf iso/boot/initrd.tar
cd initrd
tar -cf ../iso/boot/initrd.tar * --format=v7
cd ..
