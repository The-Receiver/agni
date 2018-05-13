target ?= bin/agni.iso
kernel ?= bin/shakti.elf

ifeq ($(res),)
	res := RES_800_600
endif

c_source_files := $(wildcard src/*.c)
asm_source_files := $(wildcard asm/*.asm)

c_object_files := $(patsubst src/%.c, \
	obj/%.o, $(c_source_files))
asm_object_files := $(patsubst asm/%.asm, \
	obj/%.o, $(asm_source_files))

all: $(target)

clean:
	@rm -rf obj/*.o
	@rm -rf bin/*.elf

run: all
	@kvm -m 4G -debugcon stdio -cdrom $(target)
	
bochs: all
	@bochs -q

$(target): $(kernel)
	@cp -r $(kernel) iso/boot
	@grub-mkrescue iso -o $(target)
	
$(kernel): $(c_object_files) $(asm_object_files)
	@i386-elf-gcc -fPIC -nostdlib -Tlinker.ld -nostartfiles $(c_object_files) $(asm_object_files) -o $(kernel) -lgcc

obj/%.o: asm/%.asm
	@nasm -felf32 -D$(res) $^ -o $@

obj/%.o: src/%.c
	@i386-elf-gcc -fPIC -masm=intel -I./include -c -nostdlib -fno-builtin -O0 -ffreestanding $^ -o $@
