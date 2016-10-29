CC = gcc
FLAG = -o mytest
MYDUP = hw02_104062514.c
TEST = mytest.c
TATEST = test_dup2.c
main: clean ${MYDUP} ${TEST}
	${CC} ${MYDUP} ${TEST} ${FLAG}

clean:
	rm -rf mytest

runtest: rundup2
	rm -rf files
	mkdir files
	./mytest
	@for number in 1 2 3 4 ; do \
		echo $$number;\
    sudo diff ans/$$number.txt files/$$number.txt ; \
    echo "\n";\
	done

rundup2:
	${CC} ${MYDUP} ${TATEST} -o dupans
	rm -rf ans
	mkdir ans
	./dupans