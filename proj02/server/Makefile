all: server

server: server.o shared.o db.o
	gcc -o server-proccess server.o shared.o db.o \
	-lmongoc-1.0 -lbson-1.0

shared.o: ../shared.c ../shared.h
	gcc ../shared.c -o shared.o -c 

server.o: server.c server.h
	gcc server.c -o server.o -c \
	-I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 

db.o: db.c db.h
	gcc db.c -o db.o -c \
	-I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 

clean:
	rm -rf server *.o
