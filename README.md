# This is the final exam on Operating Systems.

Objective is to implement basics FILE's operations by my own.
using structure of BFD.
For further documentation of how its work check code's comments.

For use:
in bash print
"make" to compile the program
and after that "make test" to run the tests.
Some test on my computer:

TESTS:
smallcopy(using a buffer of 8 bytes and only system function)
smallbcopy(using a buffer of 8 bytes and my implementation of bfd(FILE))
bcp(using a buffer of 1024 and my implementation of bfd(FILE))
```
tests/smallcopy :

Réussi

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 60.82    1.219463           3    335097           write
 39.18    0.785458           2    335129           read
------ ----------- ----------- --------- --------- ----------------
100.00    2.004921                670226           total

tests/smallbcopy :

Réussi

 57.99    0.001492           1      1146           write
 42.01    0.001081           0      1179           read
------ ----------- ----------- --------- --------- ----------------
100.00    0.002573                  2325           total



tests/bcp :

Réussi

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 63.13    0.001017           0      1146           write
 36.87    0.000594           0      1179           read
------ ----------- ----------- --------- --------- ----------------
100.00    0.001611                  2325           total
```
