	.text
	.file	"poc.c"
	.globl	mem_write               # -- Begin function mem_write
	.p2align	4, 0x90
	.type	mem_write,@function
mem_write:                              # @mem_write
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-8, %rsp
	subq	$32, %rsp
	pushq	%rbx
	pushq	%rax
	leaq	24(%rsp), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB0_6
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB0_6
	popq	%rax
	popq	%rbx
	movq	%rdi, 24(%rsp)
	pushq	%rbx
	pushq	%rax
	leaq	16(%rsp), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB0_6
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB0_6
	popq	%rax
	popq	%rbx
	movq	%rsi, 16(%rsp)
	pushq	%rbx
	pushq	%rax
	leaq	8(%rsp), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB0_6
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB0_6
	popq	%rax
	popq	%rbx
	movq	$0, 8(%rsp)
	jmp	.LBB0_2
.LBB0_1:                                # %ins
                                        #   in Loop: Header=BB0_2 Depth=1
	movq	16(%rsp), %rax
	movq	24(%rsp), %rcx
	pushq	%rbx
	pushq	%rax
	leaq	(%rcx), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB0_6
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB0_6
	popq	%rax
	popq	%rbx
	movq	%rax, (%rcx)
	pushq	%rbx
	pushq	%rax
	leaq	8(%rsp), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB0_6
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB0_6
	popq	%rax
	popq	%rbx
	movq	$1, 8(%rsp)
.LBB0_2:                                # %exp
                                        # =>This Inner Loop Header: Depth=1
	cmpq	$0, 8(%rsp)
	jne	.LBB0_4
# %bb.3:                                # %if.then
                                        #   in Loop: Header=BB0_2 Depth=1
	jmp	.LBB0_1
.LBB0_4:                                # %if.end
	jmp	.LBB0_5
.LBB0_5:                                # %out
	xorl	%eax, %eax
	movq	%rbp, %rsp
	popq	%rbp
	retq
.LBB0_6:                                # Label of block must be emitted
	popq	%rax
	popq	%rbx
	movl	$4294967295, %edi       # imm = 0xFFFFFFFF
	callq	exit
.Lfunc_end0:
	.size	mem_write, .Lfunc_end0-mem_write
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	andq	$-16, %rsp
	subq	$32, %rsp
	pushq	%rbx
	pushq	%rax
	leaq	28(%rsp), %rax
	movabsq	$18446744073709551615, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB1_1
	movabsq	$1, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	jb	.LBB1_1
	popq	%rax
	popq	%rbx
	movl	$0, 28(%rsp)
	leaq	.L.str(%rip), %rdi
	callq	puts@PLT
	leaq	.L.str.1(%rip), %rdi
	pushq	%rbx
	pushq	%rax
	leaq	8(%rsp), %rax
	movabsq	$1, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB1_1
	movabsq	$18446744073709551615, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB1_1
	popq	%rax
	popq	%rbx
	movl	%eax, 8(%rsp)           # 4-byte Spill
	callq	puts@PLT
	movl	$8, %edi
	pushq	%rbx
	pushq	%rax
	leaq	4(%rsp), %rax
	movabsq	$1, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB1_1
	movabsq	$18446744073709551615, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB1_1
	popq	%rax
	popq	%rbx
	movl	%eax, 4(%rsp)           # 4-byte Spill
	callq	malloc@PLT
	pushq	%rbx
	pushq	%rax
	leaq	16(%rsp), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB1_1
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB1_1
	popq	%rax
	popq	%rbx
	movq	%rax, 16(%rsp)
	movq	16(%rsp), %rdi
	movl	$3735928559, %esi       # imm = 0xDEADBEEF
	callq	mem_write@PLT
	pushq	%rbx
	pushq	%rax
	leaq	12(%rsp), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB1_1
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB1_1
	popq	%rax
	popq	%rbx
	movl	%eax, 12(%rsp)
	leaq	.L.str.2(%rip), %rdi
	callq	puts@PLT
	xorl	%ecx, %ecx
	pushq	%rbx
	pushq	%rax
	leaq	(%rsp), %rax
	movabsq	$4611686018427387903, %rbx # imm = 0x3FFFFFFFFFFFFFFF
	cmpq	%rbx, %rax
	ja	.LBB1_1
	movabsq	$5764607523034234879, %rbx # imm = 0x4FFFFFFFFFFFFFFF
	cmpq	%rax, %rbx
	jb	.LBB1_1
	popq	%rax
	popq	%rbx
	movl	%eax, (%rsp)            # 4-byte Spill
	movl	%ecx, %eax
	movq	%rbp, %rsp
	popq	%rbp
	retq
.LBB1_1:                                # Label of block must be emitted
	popq	%rax
	popq	%rbx
	movl	$4294967295, %edi       # imm = 0xFFFFFFFF
	callq	exit
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        # -- End function
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"hello in\n"
	.size	.L.str, 10

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"hello middle\n"
	.size	.L.str.1, 14

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"hello out\n"
	.size	.L.str.2, 11


	.ident	"clang version 9.0.0 (https://github.com/StanPlatinum/proofGen.git 6044f8a5547a07bdda36539f140b3f888888bd3b)"
	.section	".note.GNU-stack","",@progbits
