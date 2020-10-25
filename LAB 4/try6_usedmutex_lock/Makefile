all: server.o client.o service1.o service2.o service3.o

server.o: server.c
	gcc -pthread server.c -o server
client.o: client.c
	gcc -pthread client.c -o client
service1.o: service1.c
	gcc service1.c -o service1
service2.o: service2.c
	gcc service2.c -o service2
service3.o: service3.c
	gcc service3.c -o service3
clean:
	rm server client service1 service2 service3