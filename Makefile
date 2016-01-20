CC=gcc
CCOPTS=-g

TARGETS=server client_tcp client_udp

all: $(TARGETS)

server: server.c
	$(CC) $(CCOPTS) -o $@ $^ -lpthread

client_tcp: client_tcp.c
	$(CC) $(CCOPTS) -o $@ $^

client_udp: client_udp.c
	$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
