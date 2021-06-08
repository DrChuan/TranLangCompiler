	.file	"userLibFunc.c"
	.text
	.globl	__stack_chk_fail
	.type	__stack_chk_fail, @function
__stack_chk_fail:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	__stack_chk_fail, .-__stack_chk_fail
	.globl	printInt
	.type	printInt, @function
printInt:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movl	%edi, -116(%rbp)
	movl	%esi, -120(%rbp)
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$0, -80(%rbp)
	movq	$0, -72(%rbp)
	movq	$0, -64(%rbp)
	movq	$0, -56(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -24(%rbp)
	movl	$63, -104(%rbp)
	movl	-116(%rbp), %eax
	shrl	$31, %eax
	movzbl	%al, %eax
	movl	%eax, -92(%rbp)
	movl	-116(%rbp), %eax
	sarl	$31, %eax
	xorl	%eax, -116(%rbp)
	subl	%eax, -116(%rbp)
	jmp	.L3
.L4:
	movl	-116(%rbp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -84(%rbp)
	movl	-84(%rbp), %eax
	leal	48(%rax), %ecx
	movl	-104(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -104(%rbp)
	movl	%ecx, %edx
	cltq
	movb	%dl, -80(%rbp,%rax)
	movl	-116(%rbp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -116(%rbp)
.L3:
	cmpl	$0, -116(%rbp)
	jg	.L4
	cmpl	$0, -92(%rbp)
	je	.L5
	movl	-104(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -104(%rbp)
	cltq
	movb	$45, -80(%rbp,%rax)
.L5:
	movl	$63, %eax
	subl	-104(%rbp), %eax
	movl	%eax, -88(%rbp)
	movl	$0, -100(%rbp)
	jmp	.L6
.L7:
	movl	-100(%rbp), %eax
	cltq
	movb	$32, -80(%rbp,%rax)
	addl	$1, -100(%rbp)
.L6:
	movl	-120(%rbp), %eax
	subl	-88(%rbp), %eax
	cmpl	%eax, -100(%rbp)
	jl	.L7
	movl	$0, -96(%rbp)
	jmp	.L8
.L9:
	addl	$1, -104(%rbp)
	movl	-100(%rbp), %edx
	movl	-96(%rbp), %eax
	leal	(%rdx,%rax), %ecx
	movl	-104(%rbp), %eax
	cltq
	movzbl	-80(%rbp,%rax), %edx
	movslq	%ecx, %rax
	movb	%dl, -80(%rbp,%rax)
	addl	$1, -96(%rbp)
.L8:
	movl	-96(%rbp), %eax
	cmpl	-88(%rbp), %eax
	jl	.L9
	movl	-100(%rbp), %edx
	movl	-96(%rbp), %eax
	addl	%edx, %eax
	cltq
	movb	$0, -80(%rbp,%rax)
	movl	-100(%rbp), %edx
	movl	-96(%rbp), %eax
	leal	(%rdx,%rax), %r8d
	leaq	-80(%rbp), %rcx
#APP
# 39 "userLibFunc.c" 1
	mov $1, %rax
	mov $1, %rdi
	mov %rcx, %rsi
	movl %r8d, %edx
	syscall
	
# 0 "" 2
#NO_APP
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L10
	call	__stack_chk_fail@PLT
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	printInt, .-printInt
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
