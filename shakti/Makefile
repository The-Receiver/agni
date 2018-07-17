as ?= nasm
cc ?= i386-elf-gcc
ld ?= i386-elf-gcc
target ?= ../bin/shakti.elf

c_source_files := $(shell find src -name '*.c')
asm_source_files := $(shell find src/asm -name '*.asm')

c_object_files := $(patsubst %.c, %.o, $(c_source_files))
asm_object_files := $(patsubst %.asm, %.o, $(asm_source_files))

all: $(target) clean

clean:
	@rm -rf $(c_object_files) $(asm_object_files)

$(target): $(c_object_files) $(asm_object_files)
	@$(ld) -nostdlib -nostartfiles -Tlinker.ld -n $(c_object_files) $(asm_object_files) -o $(target) -lgcc

%.o: %.asm
	@$(as) -felf $^ -o $@

%.o: %.c
	@$(cc) -ansi -Wall -O6 -I./include -c -nostdlib -fno-builtin -ffreestanding $^ -o $@
