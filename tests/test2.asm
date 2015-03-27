  read %r1
  mov $0, %r2
loop:
  cmp $0, %r1
  jmpe fin
  mov $1, %r3
  and %r1, %r3
  add %r3, %r2
  rsh $1, %r1
  jmp loop
fin:
  print %r2
  hlt
  
