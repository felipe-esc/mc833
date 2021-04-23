all: client

client: client.o shared.o
	# add IP option
	gcc -o client-proccess client.o shared.o

shared.o: ../shared.c ../shared.h
	gcc ../shared.c -o shared.o -c

client.o: client.c client.h
	gcc client.c -o client.o -c

clean:
	rm -rf client *.o *~
