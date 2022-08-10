# Like in rainfall, we have to put the right value to login and pop a shell
# The main compares the value we enter with 0x149c (5276)

(gdb) disas main
   0x080484e7 <+83>:	cmp    $0x149c,%eax
   0x080484ec <+88>:	jne    0x804850d <main+121>
   0x080484ee <+90>:	movl   $0x8048639,(%esp)
   0x080484f5 <+97>:	call   0x8048390 <puts@plt>
   0x080484fa <+102>:	movl   $0x8048649,(%esp)
   0x08048501 <+109>:	call   0x80483a0 <system@plt>
   0x0804851e <+138>:	leave
   0x0804851f <+139>:	ret

# 0x149c = 5276

================================================================================

# We just have to enter 5276 to login and pop a shell

level00@OverRide:~$ ./level00
***************************
*       -Level00 -        *
***************************
Password:5276

$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
