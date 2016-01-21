# CMPT 434 - Winter 2016
# Assignment 1, Question 1
#
# Jordaen Graham - jhg257
#
# File: Makefile

CC := gcc
CCFLAGS := -Wall -Wextra -pedantic -pthread -g

all: clean Server Proxy

clean:
	@rm *.o* &> /dev/null || true
	@rm *~ &> /dev/null || true
	@rm Proxy &> /dev/null || true
	@rm Server &> /dev/null || true

run_server: Server
	./Server

run_proxy: Proxy
	./Proxy localhost 30490

RUN: Server Proxy
	./Server &
	./Proxy localhost 30490 &

Server: tcp_server.c tcp.c
	$(CC) $(CCFLAGS) -o Server tcp_server.c tcp.c

Proxy: tcp_proxy.c tcp.c
	$(CC) $(CCFLAGS) -o Proxy tcp_proxy.c tcp.c
