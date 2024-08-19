client: client.o client_main.o
	gcc -o client_main client.o client_main.o

client.o: client.c client.h
	gcc -c client.c

client_main.o: client_main.c client.h
	gcc -c client_main.c

clean:
	rm -f *.o client_main
