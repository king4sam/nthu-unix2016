LIBCRYPT = -lcrypt
GCC = gcc
ONAME = decrypt

all: pwd_cracker.c
	$(GCC) pwd_cracker.c $(LIBCRYPT) -o $(ONAME)
clean:
	rm $(ONAME)
run: $(ONAME)
	time ./$(ONAME) njohn.txt