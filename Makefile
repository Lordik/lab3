all: server client

server:
	gcc -pthread server.c -o fileserver

client:
	gcc client.c -o client

clean:
	rm -f fileserver 
	rm -f client