all: server client

server:
	gcc server.c -o fileserver

client:
	gcc client.c -o client

clean:
	rm -f fileserver 
	rm -f client