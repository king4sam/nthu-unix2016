TARFILE = myfilehole
SRCFILE = filehole
HEADFILE = file.holehead
MYHEADFILE = myfile.holehead
TAILFILE = file.holetail
MYTAILFILE = myfile.holetail
EXEC = lcp.out

all:clean lcp.c
	gcc lcp.c -o $(EXEC)

clean:
	rm -f $(TARFILE) $(EXEC)

run:all
	./$(EXEC) $(SRCFILE) $(TARFILE)
	./$(EXEC) $(HEADFILE) $(MYHEADFILE)
	./$(EXEC) $(TAILFILE) $(MYTAILFILE)
	ls -ls |grep $(HEADFILE)
	ls -ls |grep $(TAILFILE)
	ls -ls |grep $(SRCFILE)
