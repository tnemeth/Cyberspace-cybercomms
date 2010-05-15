/**
 *  \file    cyberspace.h
 *  \brief   Cyberspace main header..
 *
 *           Project: cybercomms (Cyberspace communication library).
 *
 *           This file contains all that is needed for a client to connect
 *           to the cyberspace system server.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Sat, May 15 2010
 */


#ifndef CYBERSPACE_H
#define CYBERSPACE_H

#include "errors.h"
#include "sockets.h"
#include "packets.h"
#include "tags.h"
#include "xmem.h"

/**
 *  \brief cyty Cyberspace communication data types.
 *  @{
 */

/*! Types of client that can connect to the cyberspace system server. */
typedef enum {client_god, client_probe, client_ship} client_type;

/** @} */


/** @cond DUPLICATE_DOCUMENTATION */
int cyberspace_connect(const char * machine, int port, client_type user);
/** @endcond */

#endif /* CYBERSPACE_H */

