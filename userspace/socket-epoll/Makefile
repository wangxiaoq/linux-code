client server: client.o server.o
	cc -o client client.o
	cc -o server server.o -lpthread

client.o: util.h

server.o: server.h util.h

.PHONY : clean
clean:
	if [ -e client.o ]; then rm client.o; fi
	if [ -e server.o ]; then rm server.o; fi
	if [ -e client ]; then rm client; fi
	if [ -e server ]; then rm server; fi
