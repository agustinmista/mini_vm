loop:
	read %r1
	print %r1
	cmp $0, %r1
	jmpe halt
	jmp loop
halt:
  hlt
