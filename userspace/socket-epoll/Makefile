client server: client.o server.o
	cc -o client client.o
	cc -o server server.o -lpthread

client.o: util.h

server.o: server.h util.h

.PHONY : clean
clean:
	if [ ls *.o ] > /dev/null 2>&1; then rm *.o; fi
	if [ -e client ]; then rm client; fi
	if [ -e server ]; then rm server; fi
