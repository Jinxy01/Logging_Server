CC = cc -Wall

OBJS = error.o mainfifoserv.o mainfifocli.o mainfifoadmin.o client.o server.o mesg.o admin.o


all: server client admin

server: $(OBJS)
	cc -Wall -o server mainfifoserv.o server.o error.o mesg.o

client: $(OBJS)
	cc -Wall -o client mainfifocli.o client.o error.o mesg.o

admin: $(OBJS)
	cc -Wall -o admin mainfifoadmin.o admin.o error.o mesg.o -pthread

	rm -f *.o	

S: server	
	./server	

C: client
	./client

A: admin
	./admin

clean:
	rm -f client admin server *.o 
