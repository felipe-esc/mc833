all: server client

server: server.o shared.o
	gcc -o server-proccess server/server.o shared.o server/db.o \
	-lmongoc-1.0 -lbson-1.0

server.o:
	cd server; \
	make clean; \
	make server.o; \
	make db.o

client: client.o shared.o
	gcc -o client-proccess client/client.o shared.o \
	-lmongoc-1.0 -lbson-1.0

client.o:
	cd client; \
	make clean; \
	make client.o

shared.o: shared.c shared.h
	gcc shared.c -o shared.o -c

clean:
	rm *-proccess *.o */*.o
