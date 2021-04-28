all: client

client: client.o shared.o
	# add IP option
	gcc -o client-proccess client.o shared.o \
	-lmongoc-1.0 -lbson-1.0

shared.o: ../shared.c ../shared.h
	gcc ../shared.c -o shared.o -c

client.o: client.c client.h
	gcc client.c -o client.o -c \
	-I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0

clean:
	rm -rf client *.o *~
