/**
 *  \file    cybercomms.c
 *  \brief   .
 *
 *           Project: cybercomms (Cyberspace communication library).
 *
 *           File description here...
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Sat, May 15 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "cyberspace.h"


int cyberspace_connect(const char * machine, int port, client_type user)
{
        int           sock = connect_server(machine, port);
        unsigned char reply[10] = {0};

        if (sock < 0)
        {
                return sock;
        }
        
        message_send(sock, PACKET_MSG_ACK, user);
        if (packet_read(sock, reply, 10) < 0)
        {
                close(sock);
                return -ERR_CONNECT_SERVER;
        }

        return sock;
}

