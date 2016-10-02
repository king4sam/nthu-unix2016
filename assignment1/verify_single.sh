#!bin/bash

rm -r myans wcans lwc

gcc lwc.c -o lwc

wc testcase/mycase.txt >> wcans
wc -lw testcase/test1.txt >> wcans
wc testcase/test2.txt -lw >> wcans
wc -lc testcase/test3.txt >> wcans
wc -l testcase/test4.txt -c >> wcans
wc -wc testcase/test5.txt >> wcans
wc -l testcase/test1.txt >> wcans
wc -w testcase/test2.txt >> wcans
wc -c testcase/test3.txt >> wcans
wc -la testcase/test4.txt >> wcans
wc -lw testcase/myasdasd.txt >> wcans
wc -s testcase/test5.txt >> wcans

./lwc -lwc testcase/mycase.txt >> myans
./lwc -lw testcase/test1.txt >> myans
./lwc testcase/test2.txt -lw>> myans
./lwc -lc testcase/test3.txt >> myans
./lwc -l testcase/test4.txt -c >> myans
./lwc -wc testcase/test5.txt >> myans
./lwc -l testcase/test1.txt >> myans
./lwc -w testcase/test2.txt >> myans
./lwc -c testcase/test3.txt >> myans
./lwc -la testcase/test4.txt >> myans
./lwc -lw testcase/myasdasd.txt >> myans
./lwc -s testcase/test4.txt >> myans

diff myans wcans

