kernel ?= bin/shakti.elf
initrd ?= iso/boot/initrd.tar
target ?= bin/agni.iso

all: $(target)

remove_old_initrd:
	@rm -rf iso/boot/initrd.tar

run: all
	@qemu-system-i386 -monitor stdio bin/agni.iso #-d int -no-reboot -no-shutdown

$(target): $(kernel) $(initrd)
	@mv -f $(kernel) iso/boot
	@grub-mkrescue iso -o $(target)

$(kernel): remove_old_initrd
	@make -C shakti

$(initrd): 
	@./.build_initrd.sh
