TARFILE = myfile.hole
SRCFILE = file.hole
HEADFILE = file.holehead
MYHEADFILE = myfile.holehead
EXEC = lcp

all:clean lcp.c
	gcc lcp.c -o $(EXEC)

clean:
	rm -f $(TARFILE) $(EXEC)

run:all
	./$(EXEC) $(SRCFILE) $(TARFILE)
	./$(EXEC) $(HEADFILE) $(MYHEADFILE)
	ls -al |grep hole
