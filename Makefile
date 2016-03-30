# CMPT 434 - Winter 2016
# Assignment 2, Question 1
#
# Jordaen Graham - jhg257
#
# File: Makefile

CC := gcc
CCFLAGS := -std=gnu99 -Wall -Wextra -pedantic -pthread -g

all: clean RUN

clean:
	@rm *.o* &> /dev/null || true
	@rm *~ &> /dev/null || true
	@rm Server &> /dev/null || true

RUN: Server
	./Server

KILL:
	@kill -9 `ps | grep Server | cut -d" " -f1` &> /dev/null ; true
	@kill -9 `ps | grep CMPT434 | cut -d" " -f1` &> /dev/null ; true

Server: server.c
	$(CC) $(CCFLAGS) -o Server server.c barrier.c
