gcc standard_arbiter.c -pthread
./a.out > standard_arbiter
gcc standard_rh.c -pthread
./a.out > standard_rh
gcc random_arbiter.c -pthread
./a.out > random_arbiter
gcc random_rh.c -pthread
./a.out > random_rh