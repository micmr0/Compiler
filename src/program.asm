.data
	a: .word 0
	linia: .asciiz  "\n"
.text
.globl main
main:
	li $t0, 0
	sw $t0, a
	li $t0, 2
	neg $t0, $t0
	sw $t0, a
	li $v0, 1
	lw $t0, a
	add $a0, $t0, $zero
	syscall
