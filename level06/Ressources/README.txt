# Auth must ret 0 so we can pop a shell

(gdb) disas auth
[...]
   0x08048866 <+286>:	cmp    eax,DWORD PTR [ebp-0x10] # CMP serial with ebp-0x10
   0x08048869 <+289>:	je     0x8048872 <auth+298> # Must take this jump v
   0x0804886b <+291>:	mov    eax,0x1                                    |
   0x08048870 <+296>:	jmp    0x8048877 <auth+303>                       |
   0x08048872 <+298>:	mov    eax,0x0  <---------------------------------
   0x08048877 <+303>:	leave
   0x08048878 <+304>:	ret
[...]

# Well, this should be easy... Let's put a breakpoint and check which value
	is needed

(gdb) b *0x08048866
Breakpoint 1 at 0x8048866
(gdb) r
Starting program: /home/users/level06/level06
***********************************
*		level06		  *
***********************************
-> Enter Login: AAAAAA
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: salut
.---------------------------.
| !! TAMPERING DETECTED !!  |
'---------------------------'
[Inferior 1 (process 2273) exited with code 01]

# Well, anti-debug technique used with PTRACE :(

================================================================================

# Im gonna patch the binary so the check will be good when debuging

080487ba 83 f8 ff        CMP        EAX,-0x1 # Checks the return of PTRACE
# Instead of comparing with -1, i'll for the program to compare with 0 so we won't take the jump

# Just change those bytes in an hex editor
               ff
               -- # Put 0x00 instead of 0xff
080487ba 83 f8 00        CMP        EAX,0x0
================================================================================

(gdb) b *0x08048866
Breakpoint 1 at 0x8048866
(gdb) r
Starting program: /home/users/level06/level06
***********************************
*		level06		  *
***********************************
-> Enter Login: AAAAAA
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 9999999

Breakpoint 1, 0x08048866 in auth ()
=> 0x08048866 <auth+286>:	3b 45 f0	cmp    eax,DWORD PTR [ebp-0x10]

# Nice, we patched the binary ! We can now access the value
(gdb) x/wd $ebp-0x10
0xffffcd88:	6229050

# It's easy, for user AAAAAA the binary is comparing our serial with 6229050
# So this should be the right serial number

level06@OverRide:~$ ./level06
***********************************
*		level06		  *
***********************************
-> Enter Login: AAAAAA
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6229050
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
