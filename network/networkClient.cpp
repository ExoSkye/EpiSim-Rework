//
// Created by protobit on 17/10/2020.
//

#include "networkClient.h"


void
networkClient::run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
                   int immuneLength, int immuneLengthVar) {
/*
    count++;
    char pack[PACKETSIZE];
    packet DePack{};
    for (human& person : *humans) {
        sock->rcv(pack,PACKETSIZE);
        deserialize(pack,&DePack);
        if (DePack.type == 1) {
            deserialize(DePack.data, &person);
        }
    }

    algorithm->run(humans,infectChance,infectRadius,x,y,immuneChance,immuneLength,immuneLengthVar);

    for (human& person : *humans) {
        char data[BUFSIZE];

    }
    */
}

void networkClient::end() {
    sock->destroy();
}

void networkClient::initNet(std::string ip, std::string port) {
    _ip = ip;
    _port = port;
    initNet_REAL();
}

void networkClient::initNet(std::string ip) {
    _ip = ip;
    initNet_REAL();
}

void networkClient::initNet_REAL() {
    /*
    sock = new libsocket::inet_stream(_ip,_port,LIBSOCKET_UDP);
    try {
        *sock << "1";
        std::string ret;
        ret.resize(1);
        *sock >> ret;
        if (ret == "0") {
            std::cerr << "Server didn't respond properly";
            exit(2);
        }
        // sock is closed here automatically!
    } catch (const libsocket::socket_exception& exc) {
        std::cerr << exc.mesg;
    }*/
    char out[] = "aaaaaaaa";
    deflateSerialData(out,5,sizeof(out));
    std::cout << out << std::endl;
    inflateSerialData(out,5,sizeof(out));
    std::cout << out << std::endl;

}
