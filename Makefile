# CMPT 434 - Winter 2016
# Assignment 3, Question 1
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
	./Server 1 15 400 10 0

KILL:
	@kill -9 `ps | grep \./Server | cut -d" " -f1` &> /dev/null ; true
	@kill -9 `ps | grep CMPT434 | cut -d" " -f1` &> /dev/null ; true

Server: server.c
	$(CC) $(CCFLAGS) -lm -o Server server.c connections.c &> /dev/null
