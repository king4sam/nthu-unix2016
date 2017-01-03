GCC = gcc
ONAME = 9a
OCLIENT = client
OSERVER = server
PTHREAD = -lpthread

all: 9a.c 9b_client.c
	$(GCC) 9a.c -o $(ONAME)
	$(GCC) 9b_client.c -o $(OCLIENT)
	$(GCC) 9b_server.c $(PTHREAD) -o $(OSERVER)
clean:
	rm $(ONAME) $(OCLIENT) $(OSERVER)