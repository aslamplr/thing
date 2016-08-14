# CoAP client, server library for Arduino.
<a href="http://coap.technology/" target=_blank>CoAP</a> simple server, client library for Arduino.

## Source Code
This lightweight library source code are only 2 files. coap.cpp, coap.h.

## Example
Some sample sketches for Arduino included(/examples/).

 - coaptest.ino : simple request/response sample.
 - coapserver.ino : server endpoint url callback sample.

## How to use
Download this source code branch zip file and extract to the Arduino libraries directory or checkout repository. Here is checkout on MacOS X.

    cd $HOME/Documents/Arduino/libraries/
    git clone -b arduino https://github.com/hirotakaster/CoAP CoAP_library
    # restart Arduino IDE, you can find CoAP_library examples.

In this exmples need CoAP server libcoap or microcoap server for check the example program. There is setting the libcoap on Ubuntu Linux. But if there don't use CoAP server(request/reseponse), following setting don't be needed.

    git clone https://github.com/obgm/libcoap 
    cd libcoap/
    ./autogen.sh 
    ./configure --disable-examples 
    gcc -o coap-server ./examples/coap-server.c -I./include/coap/ -I. -L.libs -lcoap-1 -DWITH_POSIX
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.libs
    ./coap-server
    # next start Arduino and check the request/response.

## Particle Photon, Core compatible
This library is Particle Photon, Core compatible. That's version is <a href="https://github.com/hirotakaster/CoAP">here</a>.
