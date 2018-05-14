target ?= bin/agni.iso
kernel ?= bin/shakti.elf

ifeq ($(res),)
	res := RES_800_600
endif

c_source_files := $(shell find src -name '*.c')
asm_source_files := $(shell find asm -name '*.asm')

c_object_files := $(c_source_files:.c=.o)
asm_object_files := $(asm_source_files:.asm=.o)

all: $(target) clean

clean:
	@rm -rf $(c_object_files) $(asm_object_files)
	@rm -rf bin/*.elf

run: all
	@kvm -m 4G -debugcon stdio -cdrom $(target)
	
bochs: all
	@bochs -q

$(target): $(kernel)
	@cp -r $(kernel) iso/boot
	@grub-mkrescue iso -o $(target)
	
$(kernel): $(c_object_files) $(asm_object_files)
	@i386-elf-gcc -Wall -Wextra -Werror -fPIC -nostdlib -Tlinker.ld -nostartfiles $(c_object_files) $(asm_object_files) -o $(kernel) -lgcc

%.o: %.asm
	@nasm -felf32 -D$(res) $(@:.o=.asm) -o $@

%.o: %.c
	@i386-elf-gcc -fPIC -masm=intel -I./include -c -nostdlib -fno-builtin -O0 -ffreestanding $(@:.o=.c) -o $@
