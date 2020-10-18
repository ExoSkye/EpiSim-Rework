//
// Created by protobit on 18/10/2020.
//

#include "serializeHuman.h"

void serialize(human *msgPacket, char *data) {
    int *q = (int*)data;
    *q = msgPacket->x; q++;
    *q = msgPacket->y; q++;
    *q = (int)msgPacket->infect_info; q++;
    *q = msgPacket->real; q++;
    *q = msgPacket->time; q++;
    *q = msgPacket->peopleInfected; q++;
}

void deserialize(char *data, human *msgPacket) {
    int* q = (int*)data;
    msgPacket->x = *q; q++;
    msgPacket->y = *q; q++;
    msgPacket->infect_info = (infectInfo)*q; q++;
    msgPacket->real = *q; q++;
    msgPacket->time = *q; q++;
    msgPacket->peopleInfected = *q; q++;
}

void serialize(packet *msgPacket, char *data) {
    int *q = (int*)data;
    *q = msgPacket->type;   q++;
    *q = msgPacket->priority;   q++;
    *q = msgPacket->sender;   q++;
    char *p = (char*)q;
    int i = 0;
    while (i < BUFSIZE)
    {
        *p = msgPacket->data[i];
        p++;
        i++;
    }
}

void deserialize(char *data, packet *msgPacket) {
    int *q = (int*)data;
    msgPacket->type = *q;       q++;
    msgPacket->priority = *q;   q++;
    msgPacket->sender = *q;     q++;

    char *p = (char*)q;
    int i = 0;
    while (i < BUFSIZE)
    {
        msgPacket->data[i] = *p;
        p++;
        i++;
    }
}
