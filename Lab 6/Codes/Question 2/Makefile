all: server.o client.o

server.o: server.c
	gcc server.c -o server
client.o: client.c
	gcc client.c -o client
clean:
	rm server client