
# Author   KMS - Martin Dubois, ing.
# Product  FX3-KB
# File     cyfx_gcc_startup.S

.section .text
.code 32

# Jump function implementation.
.global jump
jump:
    bx  R0

.global CyU3PToolChainInit
CyU3PToolChainInit:

# Clear the BSS segment and then jump to main.
__main:
	mov	R0, #0
	ldr	R1, =_bss_start
	ldr	R2, =_bss_end
1:	cmp	R1, R2
	strlo	R0, [R1], #4
	blo	1b

	b	main

# Stacks are setup in the FX3 API library. We currently do not use a heap
# in this application. Therefore, no stack or heap initialization is required.
.global __user_initial_stackheap
__user_initial_stackheap:

.if  INTER == TRUE
    bx      lr                 
.else
    mov     pc, lr             
.endif

.end
