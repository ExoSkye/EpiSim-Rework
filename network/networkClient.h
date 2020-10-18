//
// Created by protobit on 17/10/2020.
//

#ifndef EPISIM_NETWORKCLIENT_H
#define EPISIM_NETWORKCLIENT_H

#include "networkAlgo.h"

#include "../libs/libSocket/headers/exception.hpp"
#include "../libs/libSocket/headers/inetclientstream.hpp"

class networkClient : public networkAlgo {
private:
    int count = 0;
    algo* algorithm;
    libsocket::inet_stream* sock;
public:
    void run(std::vector<human> *humans, int infectChance, int infectRadius, int x, int y, double immuneChance,
             int immuneLength, int immuneLengthVar) override;

    void end() override;

    void initNet(std::string ip) override;

    void initNet_REAL() override;

    void initNet(std::string ip, std::string port) override;
};


#endif //EPISIM_NETWORKCLIENT_H
