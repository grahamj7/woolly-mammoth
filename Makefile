# CMPT 434 - Winter 2016
# Assignment 1, Question 3
#
# Jordaen Graham - jhg257
#
# File: Makefile

CC := gcc
CCFLAGS := -Wall -Wextra -pedantic -pthread -g

all: clean UDP Proxy

clean:
	@rm *.o* &> /dev/null || true
	@rm *~ &> /dev/null || true
	@rm Proxy &> /dev/null || true
	@rm UDP &> /dev/null || true

RUN: UDP Proxy
	./UDP &
	./Proxy &

run_server: UDP
	./UDP

run_proxy: Proxy
	./Proxy

UDP: udp_server.c
	$(CC) $(CCFLAGS) -o UDP udp_server.c

Proxy: udp_proxy.c tcp.c
	$(CC) $(CCFLAGS) -o Proxy udp_proxy.c tcp.c


