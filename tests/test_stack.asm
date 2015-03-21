  mov $1, %r1
  push %r1
  print %r1
stack:
  add $1, %r1
  push %r1
  print %r1
  cmp $5, %r1
  jmpe unstack
  jmp stack
unstack:
  cmp $1, %r1
  jmpe halt
  pop %r1
  print %r1
  jmp unstack
halt:
  hlt
