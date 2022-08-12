(gdb) run
Starting program: /home/users/level03/level03
***********************************
*		level03		**
***********************************
Password:1660315947

0x08048753 <+12>:	mov    ecx,edx
0x08048755 <+14>:	sub    ecx,eax # Subbing operation, let's breakpoint

Breakpoint 2, 0x08048755 in test ()
(gdb) info reg
[...]
ecx            0x1337d00d	322424845
[...]

# To enter decrypt with our provided passwd, we must pass the conditions:
# 322424845 - passwd > 1
# 322424845 - passwd < 22

# Decrypt is performing a XOR operation on a string with the result of:
	322424845 - passwd
# Then compares the result with "Congratulations!"

================================================================================

# We will "bruteforce" it with a python script that will try all values from 1 to 21

➜  override git:(master) ✗ cat exploit.py
key = "Q}|u`sfg~sf{}|a3"

def decrypt(nbr):
    print("Value: " + str(nbr), end=', ')
    for c in range(0, len(key)):
        res = nbr ^ ord(key[c])
        print(chr(res), end='')
    print()

for i in range(1, 21):
    decrypt(i)

➜  override git:(master) ✗ python3 exploit.py
Value: 1, P|}targfrgz|}`2
Value: 2, S~wbqde|qdy~c1
Value: 3, R~vcped}pex~b0
Value: 4, Uyxqdwbczwbyxe7
Value: 5, Txypevcb{vc~xyd6
Value: 6, W{zsfu`axu`}{zg5
Value: 7, Vz{rgta`yta|z{f4
Value: 8, Yut}h{nov{nsuti;
Value: 9, Xtu|izonwzortuh:
Value: 10, [wvjylmtylqwvk9
Value: 11, Zvw~kxmluxmpvwj8
Value: 12, ]qpyljkrjwqpm?
Value: 13, \pqxm~kjs~kvpql>
Value: 14, _sr{n}hip}husro=
Value: 15, ^rszo|ihq|itrsn<
Value: 16, Amlepcvwncvkmlq#
Value: 17, @lmdqbwvobwjlmp"
Value: 18, Congratulations!
Value: 19, Bnofs`utm`uhnor
Value: 20, Eihatgrsjgroihu'

# Nice, our str is "Congratulations!" when the provided value is 18

======================================================================================

# 322424845 - 18 = 322424827

level03@OverRide:~$ ./level03
***********************************
*		level03		**
***********************************
Password:322424827
$ whoami
level04
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
