/**
 *  \file    cybercomms.c
 *  \brief   Cyberspace communications.
 *
 *           Project: cybercomms (Cyberspace communication library).
 *
 *           This file contains the wrapping functions for cyberspace
 *           communications.
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


/**
 *  \brief Cyberspace connection function.
 *
 *         This function connects a client to the cyberspace system server.
 *
 * @param machine               machine hosting the cyberspace system server
 * @param port                  port of the listening server
 * @param user                  type of client/user
 * @return                      the communication socket or an error status
 * @retval -ERR_CONNECT_SERVER          unable to connect to the server
 * @retval -ERR_BAD_PARAMETER           remote host no given in parameter
 * @retval -ERR_SERVER_INFO             could not find server information
 * @retval -ERR_CREATE_SOCKET           could not create the socket
 * @retval -ERR_CONFIGURE_SOCKET        could not configure the socket
 * @retval -ERR_UNKNOWN_ADDRESS         could not find address (host)
 * @retval -ERR_BIND_SOCKET             could not bind the socket
 */
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

