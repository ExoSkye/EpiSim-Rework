//
// Created by protobit on 18/10/2020.
//

#ifndef EPISIM_SERIALIZEHUMAN_H
#define EPISIM_SERIALIZEHUMAN_H
#pragma once
#include "../human.h"

#define PACKETSIZE  sizeof(packet)
#define BUFSIZE     sizeof(human)

struct packet {
    int type;
    int priority;
    int sender;
    char data[BUFSIZE];
};

extern void serialize(human *msgPacket, char *data);
extern void deserialize(char *data, human *msgPacket);
extern void serialize(packet *msgPacket, char *data);
extern void deserialize(char *data, packet *msgPacket);
#endif //EPISIM_SERIALIZEHUMAN_H
