cc ?= i386-tcc
ld ?= i386-elf-gcc
target ?= ../bin/shakti.elf

c_source_files := $(shell find src -name '*.c')
asm_source_files := $(shell find src/asm -name '*.asm')

c_object_files := $(c_source_files:.c=.o)
asm_object_files := $(asm_source_files:.asm=.o)

all: $(target) clean

clean:
	@rm -rf $(c_object_files) $(asm_object_files)

$(target): $(c_object_files) $(asm_object_files)
	@$(ld) -nostdlib -nostartfiles -Tlinker.ld -n $(c_object_files) $(asm_object_files) -o $(target) -lgcc

%.o: %.asm
	@nasm -Fdwarf -felf32 $(@:.o=.asm) -o $@

%.o: %.c
	@$(cc) -g -Wall -Wextra -O3 -I./include -c -nostdlib -fno-builtin -ffreestanding $(@:.o=.c) -o $@
