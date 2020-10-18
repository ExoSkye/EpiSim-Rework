//
// Created by protobit on 17/10/2020.
//

#ifndef EPISIM_NETWORKALGO_H
#define EPISIM_NETWORKALGO_H

#include "../algorithm.h"


class networkAlgo : public algo {
public:
    std::string _ip;
    std::string _port = "25565";
    virtual void initNet(std::string ip, std::string port) = 0;
    virtual void initNet(std::string ip) = 0;
    virtual void initNet_REAL() = 0;
};


#endif //EPISIM_NETWORKALGO_H
