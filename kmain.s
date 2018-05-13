	.file	"kmain.c"
	.text
	.section	.rodata
.LC0:
	.string	"Agni version 0.0.1\n"
.LC1:
	.string	"Starting boot...\n"
	.align 8
.LC2:
	.string	"[boot] setting up identity paging...\n"
	.align 8
.LC3:
	.string	"[boot] activated identity paging\n"
	.align 8
.LC4:
	.string	"[boot] setting up interrupts...\n"
.LC5:
	.string	"[boot] activated interrupts\n"
.LC6:
	.string	"[boot] allocating memory...\n"
	.align 8
.LC7:
	.string	"[boot] memory allocated at address %x \n"
	.align 8
.LC8:
	.string	"[boot] copying data to the address...\n"
	.align 8
.LC9:
	.string	"eorkeporjpeojrpejrepreporjeporjoerjoe"
.LC10:
	.string	"[boot] memory contains \""
.LC11:
	.string	"\"\n"
	.align 8
.LC12:
	.string	"[boot] allocating memory failed\n"
.LC13:
	.string	"[boot] testing the pit...\n"
.LC14:
	.string	"[boot] working!\n"
.LC15:
	.string	"[boot] initializing acpi..."
	.text
	.globl	kmain
	.type	kmain, @function
kmain:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	video_init@PLT
	leaq	.LC0(%rip), %rdi
	call	kputs@PLT
	leaq	.LC1(%rip), %rdi
	call	kputs@PLT
	call	set_up_page_tables@PLT
	leaq	.LC2(%rip), %rdi
	call	kputs@PLT
	call	paging_init@PLT
	leaq	.LC3(%rip), %rdi
	call	kputs@PLT
	leaq	.LC4(%rip), %rdi
	call	kputs@PLT
	movl	$0, %eax
	call	idt_init@PLT
	leaq	.LC5(%rip), %rdi
	call	kputs@PLT
	call	pmm_init@PLT
	leaq	.LC6(%rip), %rdi
	call	kputs@PLT
	call	pmm_alloc_page@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L2
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	kprintf@PLT
	leaq	.LC8(%rip), %rdi
	call	kputs@PLT
	leaq	.LC9(%rip), %rdi
	call	kstrlen@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	leaq	.LC9(%rip), %rsi
	movq	%rax, %rdi
	call	kmemcpy@PLT
	leaq	.LC10(%rip), %rdi
	call	kputs@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	kputs@PLT
	leaq	.LC11(%rip), %rdi
	call	kputs@PLT
	jmp	.L3
.L2:
	leaq	.LC12(%rip), %rdi
	call	kputs@PLT
.L3:
	leaq	.LC13(%rip), %rdi
	call	kputs@PLT
	movl	$2, %edi
	call	kdelay@PLT
	leaq	.LC14(%rip), %rdi
	call	kputs@PLT
	leaq	.LC15(%rip), %rdi
	call	kputs@PLT
#APP
# 37 "src/kmain.c" 1
	hlt
# 0 "" 2
#NO_APP
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	kmain, .-kmain
	.ident	"GCC: (Debian 7.3.0-18) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
