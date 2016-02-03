CMPT 434 - Winter 2016
Assignment 4, Questions 1,2

Jordaen Graham - jhg257

File: ReadMe.txt
========================================
Files included:

Makefile

tcp.c
    Connection methods to reduce duplicating code in each file

tcp_proxy.c
    Proxy server that redirects request to the main server

tcp_server.c
    Main server that contains the list if keys and values

server.h
  Header file for the server.
  

How to run:
To compile the Server and Proxy Server
  make Server Proxy

To run the server and proxy:
  ./Server
  ./Proxy serverhost serverport

To connect to the proxy:
  netcat host proxyport


Here's a dinosaur for your marking enjoyment:

                                 .       .
                                / `.   .' \
                        .---.  <    > <    >  .---.
                        |    \  \ - ~ ~ - /  /    |
                         ~-..-~             ~-..-~
                     \~~~\.'                    `./~~~/
           .-~~^-.    \__/                        \__/
         .'  O    \     /               /       \  \
        (_____,    `._.'               |         }  \/~~~/
         `----.          /       }     |        /    \__/
               `-.      |       /      |       /      `. ,~~|
                   ~-.__|      /_ - ~ ^|      /- _      `..-'   f: f:
                        |     /        |     /     ~-.     `-. _||_||_
                        |_____|        |_____|         ~ - . _ _ _ _ _>

                         
  
