CMPT 434 - Winter 2016
Assignment 3, Question 1

Jordaen Graham - jhg257

File: ReadMe.txt
========================================
Files included:

Makefile
    all -> clean and RUN
    clean -> cleans the workspace
    RUN -> builds and executes the simulation
    KILL -> cleans up any leftover proccesses running
    Server -> compiles the simulation program

server.c
    runs the simulation
    initializes tags for each animal as well as for the base station
    splits off new processes for the animals
    each animal creates a socket connection that can connect to each other animal and to the base station
        animals move then check if they can exchange data
        if an animal is in range of the base station it transmits all of it's packets
            it then tells each other animal that those packets have been delivered so they can
            be deleted from their packet lists and try to avoid sending duplicate packets

server.h
    contains global variables such as the Number of Tags, the size of the grid
    holds the structures that will be used

connections.c
connections.h
    intermediary for the server to create TCP connections


How to run:
To compile:
    make Server


To run:
    - make
    - make RUN
    - compile then
        ./Server <Steps> <Distance> <Range> <Packets> <Output>
            **all args are required**
