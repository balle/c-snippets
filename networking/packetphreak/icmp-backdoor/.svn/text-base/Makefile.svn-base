CC=gcc
LIBS=-lssl

all : client server

client : client.o crypt.o
	$(CC) $(LIBS) -o client client.o crypt.o

server : server.o crypt.o
	$(CC) $(LIBS) -o server server.o crypt.o

clean : 
	rm *.o client server 


