      mov $20, %r1
      mov $2, %r2
      print %r1
loop:
      sub %r2, %r1
      print %r1
      cmp $0, %r1
      jmpl loop
      hlt
