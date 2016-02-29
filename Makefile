# CMPT 434 - Winter 2016
# Assignment 2, Question 1
#
# Jordaen Graham - jhg257
#
# File: Makefile

CC := gcc
CCFLAGS := -Wall -Wextra -pedantic -pthread -g

all: clean Receiver Sender

clean:
	@rm *.o* &> /dev/null || true
	@rm *~ &> /dev/null || true
	@rm Receiver &> /dev/null || true
	@rm Sender &> /dev/null || true

RUN: Receiver
	./Receiver &
	./Sender &

KILL:
	@kill -9 `ps | grep Receiver | cut -d" " -f1` &> /dev/null ;\
	@kill -9 `ps | grep Sender | cut -d" " -f1` &> /dev/null ;\

run_receiver: Receiver
	./Receiver
#	./Receiver 5

Receiver: receiver.c
	$(CC) $(CCFLAGS) -o Receiver receiver.c

run_sender: Sender
	./Sender
#	./Sender 127.0.0.1 3950 20 10

Sender: sender.c
	$(CC) $(CCFLAGS) -o Sender sender.c


