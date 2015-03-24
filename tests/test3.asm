  push $35
  push $23
  push $-5
  push $12
  mov $4, %r3
  mov $0, %r1
loop:
  pop %r2
  add %r2, %r1
  sub $1, %r3
  cmp $0, %r3
  jmpe show
  jmp loop
show:
  print %r1
  hlt
