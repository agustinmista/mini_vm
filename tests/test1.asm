  read %r1
  cmp $0, %r1
  jmpl neg
  jmp pos
pos:
  mul $-1, %r1
  print %r1
  hlt
neg:
  print %r1
  hlt
