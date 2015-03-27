main:
	push $3
	push $5
	call f
	pop %r2
	print %r2
	hlt	
f:
	pop %r3
	pop %r2
	pop %r1
	add %r2, %r1
	push %r1
	push %r3
	ret
