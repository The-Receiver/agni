rm -rf iso/boot/initrd.tar
cd initrd
tar -cf ../iso/boot/initrd.tar * --format=ustar
cd ..
