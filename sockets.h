/**
 *  \file    sockets.h
 *  \brief   Sockets related functions.
 *
 *           Project: project independant file.
 *
 *           This is the header file of sockets.c and contains all the functions
 *           declarations for sockets manipulations.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    mar 27 mar 2007
 */


#ifndef SOCKETS_H
#define SOCKETS_H

#include <netinet/in.h>

/** @cond DUPLICATE_DOCUMENTATION */
int socket_open(int port, char *ip_address, struct sockaddr_in *ptr_address);
int connect_server(const char *machine, int port);
int install_server(int port, char *ip_address, struct sockaddr_in *ptr_address);
int wait_timeout(int fd, int timeout);
int accept_connection(int socket_server, int timeout);
int socket_remote_host(int fd, char * name, int len);
int socket_remote_ip(int fd, char * addr, int len);
int socket_remote_port(int fd);
int socket_local_port(int fd);
/** @endcond */


#endif /* SOCKETS_H */

