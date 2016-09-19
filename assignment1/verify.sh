#!bin/bash

rm -r myans wcans lwc

gcc lwc.c -o lwc

wc testcase/mycase.txt >> wcans
wc -lw testcase/* >> wcans
wc testcase/* -lw >> wcans
wc -lc testcase/* >> wcans
wc -l testcase/* -c >> wcans
wc -wc testcase/* >> wcans
wc -l testcase/* >> wcans
wc -w testcase/* >> wcans
wc -c testcase/* >> wcans
wc -la testcase/* >> wcans
wc -lw testcase/myasdasd.txt >> wcans
wc -s testcase/* >> wcans

./lwc -lwc testcase/mycase.txt >> myans
./lwc -lw testcase/* >> myans
./lwc testcase/* -lw>> myans
./lwc -lc testcase/* >> myans
./lwc -l testcase/* -c >> myans
./lwc -wc testcase/* >> myans
./lwc -l testcase/* >> myans
./lwc -w testcase/* >> myans
./lwc -c testcase/* >> myans
./lwc -la testcase/* >> myans
./lwc -lw testcase/myasdasd.txt >> myans
./lwc -s testcase/* >> myans

diff myans wcans

