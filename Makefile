
CFLAGS = -Wall

client: chatclient.c chatclient.h
	gcc -o chatclient -g  chatclient.c $(CFLAGS)

all: client

clean:
	rm -f *.o chatclient
