target ?= bin/agni.iso
kernel ?= bin/shakti.elf

c_source_files := $(shell find src -name '*.c')
asm_source_files := $(shell find src/asm -name '*.asm')

c_object_files := $(c_source_files:.c=.o)
asm_object_files := $(asm_source_files:.asm=.o)

all: $(target) clean

clean:
	@rm -rf $(c_object_files) $(asm_object_files)
	@rm -rf bin/*.elf

run: all
	@kvm -debugcon stdio -m 4G -cdrom $(target)
	
bochs: all
	@bochs -q

$(target): $(kernel)
	@cp -r $(kernel) iso/boot
	@grub-mkrescue iso -o $(target)
	
$(kernel): $(c_object_files) $(asm_object_files)
	@i386-elf-gcc -m32 -g -Wall -Wextra -Werror -nostdlib -Tlinker.ld -nostartfiles $(c_object_files) $(asm_object_files) -o $(kernel) -lgcc

%.o: %.asm
	@nasm -Fdwarf -felf32 $(@:.o=.asm) -o $@

%.o: %.c
	@i386-elf-gcc -g -Wall -Werror -masm=intel -I./include -c -nostdlib -fno-builtin -O0 -ffreestanding $(@:.o=.c) -o $@
