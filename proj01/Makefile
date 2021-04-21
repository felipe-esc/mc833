all: server.o client.o shared.o
	gcc -o client-proccess client/client.o shared.o
	gcc -o server-proccess server/server.o shared.o

server.o:
	cd server; \
	make clean; \
	make server.o

client.o:
	cd client; \
	make clean; \
	make client.o

shared.o: shared.c shared.h
	gcc shared.c -o shared.o -c

clean:
	rm client-proccess server-proccess client/client.o server/server.o *.o
