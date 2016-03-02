CMPT 434 - Winter 2016
Assignment 2, Question 2

Jordaen Graham - jhg257

File: ReadMe.txt
========================================
Files included:

Makefile

sender.c
    Client to send packets to the receiver

receiver.c
    Receives packets and returns an ack if it is in the proper order and uncorrupted

server.h
    Header file for the server.


How to run:
To compile the UDP Server
  make Sender
  
To compile the Receiver
  make Receiver

To run the Sender and Receiver:
  ./Sender [arguments]
  ./Receiver [arguments]

