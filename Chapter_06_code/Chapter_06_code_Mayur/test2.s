	.text
	.file	"test.ll"
	.globl	test
	.align	16, 0x90
	.type	test,@function
test:                                   # @test
	.cfi_startproc
# BB#0:
	movl	%edx, %ecx
	movl	%esi, %edx
	movl	%edi, %eax
	leal	(%rax,%rdx), %eax
	cltd
	idivl	%ecx
	retq
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
	.cfi_endproc


	.section	".note.GNU-stack","",@progbits
