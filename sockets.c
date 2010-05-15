/**
 *  \file    sockets.c
 *  \brief   Sockets related functions.
 *
 *           Project: project independant file.
 *
 *           This file contains all the low-level sockets manipulation
 *           functions. It allows for socket creation, server socket
 *           creation and installation, server connections, accepting
 *           incomming transmission and waiting for incomming data on
 *           a socket.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    ven 31 aoû 2007
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "errors.h"
#include "sockets.h"


#ifdef SunOS
/**
 * \brief IP address conversion.
 *
 *        This function convert the given IP address into a structure usable by
 *        the C library functions.
 *
 * @param padr          IP address
 * @param ia            returned structure built from the IP address
 * @return              the status of the converstion
 * @retval      1       success
 * @retval      0       failure
 */
static int inet_aton_local(char *padr, struct in_addr *ia)
{
        int lna, net;

        lna = inet_addr(padr); /* Extract the sub-net number */
        if (lna == -1)
        {
                return 0;
        }

        net = inet_network(padr); /* Extact the network number */
        if (net == -1)
        {
                return 0;
        }

        *ia = inet_makeaddr(net, lna);

        return 1;
}
#endif


/**
 *  \brief Open a socket.
 *
 *         This function opens a socket of the TCP stream type. A server
 *         (listening) socket must specify its port different from 0.
 *
 * @param port          TCP port (0 if not a server)
 * @param ip_address    IP address
 * @param ptr_address   returned structure built from the IP address
 * @return              the file descriptor of the opened socket or a negative
 *                      value in case of error.
 * @retval -ERR_CREATE_SOCKET           could not create the socket
 * @retval -ERR_CONFIGURE_SOCKET        could not configure the socket
 * @retval -ERR_UNKNOWN_ADDRESS         could not find host (machine/IP address)
 * @retval -ERR_BIND_SOCKET             could not bind the socket to the address
 */
int socket_open(int port, char *ip_address, struct sockaddr_in *ptr_address)
{
        int                 sock_fd;
        unsigned int        len_addr = sizeof(struct sockaddr_in);
        unsigned int        len_ling = sizeof(struct linger);
        struct sockaddr_in  address;
        struct linger       ling;

        /*
         *      Socket creation
         */
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd == -1)
        {
                return -ERR_CREATE_SOCKET;
        }

        /*
         *      Options setting
         */
        if (port != 0)
        {
                int opt = 1;
                if (setsockopt(sock_fd,
                               SOL_SOCKET,
                               SO_REUSEADDR,
                               &opt,
                               sizeof(int)) == -1)
                {
                        return -ERR_CONFIGURE_SOCKET;
                }
        }
        ling.l_onoff = 0;
        ling.l_linger = 0;
        if (setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &ling, len_ling) == -1)
        {
                return -ERR_CONFIGURE_SOCKET;
        }

        /*
         *      Address binding preparation
         */
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        if (ip_address)
        {
                struct in_addr ia;

#ifdef SunOS
                if (inet_aton_local(ip_address, &ia))
#else
                if (inet_aton(ip_address, &ia))
#endif
                {
                        address.sin_addr = ia;
                }
                else
                {
                        struct hostent *h;

                        h = gethostbyname(ip_address);
                        if (!h)
                        {
                                return -ERR_UNKNOWN_ADDRESS;
                        }
                        memcpy(&address.sin_addr.s_addr, h->h_addr, h->h_length);
                }
        }
        else
        {
                address.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        address.sin_port = htons(port);

        /*
         *      Binding of the socket to the address
         */
        if (bind(sock_fd, (struct sockaddr *)&address, len_addr) == -1)
        {
                close(sock_fd);
                return -ERR_BIND_SOCKET;
        }

        /*
         *      Get back address binfin information
         */
        if (ptr_address != NULL)
        {
                getsockname(sock_fd, (struct sockaddr*)ptr_address,
                            (socklen_t *)&len_addr);
        }

        return sock_fd;
}


/**
 *  \brief Connect to a server.
 *
 *         This function opens a socket of the stream type and tries to
 *         connect a server.
 *
 * @param machine       IP address or hostname of the server
 * @param port          TCP port to connect to on the server
 * @return              the file descriptor of the opened socket or a negative
 *                      value in case of error
 * @retval -ERR_BAD_PARAMETER           remote host no given in parameter
 * @retval -ERR_SERVER_INFO             could not find server information
 * @retval -ERR_CREATE_SOCKET           could not create the socket
 * @retval -ERR_CONFIGURE_SOCKET        could not configure the socket
 * @retval -ERR_UNKNOWN_ADDRESS         could not find address (host)
 * @retval -ERR_BIND_SOCKET             could not bind the socket
 * @retval -ERR_CONNECT_SERVER          could not connect to the given server
 */
int connect_server(const char *machine, int port)
{
        struct hostent     * server_ent;
        struct sockaddr_in   server_addr, client_addr;
        int                  sock_fd;

        /*
         *      Look for the host where the server is
         */
        if (! machine)
        {
                return -ERR_BAD_PARAMETER;
        }
        server_ent = gethostbyname(machine);
        if (server_ent == NULL)
        {
                return -ERR_SERVER_INFO;
        }

        /*
         *      Communication socket creation -- indifferent port
         */
        sock_fd = socket_open(0, NULL, (struct sockaddr_in *)&client_addr);
        if (sock_fd < 0)
        {
                return sock_fd;
        }

        /*
         *      Server address preparation
         */
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        memcpy(&server_addr.sin_addr.s_addr,
               server_ent->h_addr, server_ent->h_length);

        /*
         *      Connecting to the server
         */
        if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
                return -ERR_CONNECT_SERVER;
        }

        return sock_fd;
}


/**
 *  \brief Server installation function.
 *
 *         This function installs a server on a listening socket in order
 *         to wait for incomming clients connections. It returns the file
 *         descriptor of the opened listening socket.
 *
 * @param port          the TCP port on which to listen
 * @param ip_address    the IP address to bind to
 * @param ptr_address   returned information about the socket
 * @return              the file descriptor index of the socket, a negative
 *                      value in case of error
 * @retval -ERR_CREATE_SOCKET           could not create the socket
 * @retval -ERR_CONFIGURE_SOCKET        could not configure the socket
 * @retval -ERR_UNKNOWN_ADDRESS         could not find address (host)
 * @retval -ERR_BIND_SOCKET             could not bind the socket
 * @retval -ERR_SERVER_LISTEN           could not configure the listening
 */
int install_server(int port, char *ip_address, struct sockaddr_in *ptr_address)
{
        int sock_fd = socket_open(port, ip_address, ptr_address);

        if (sock_fd < 0)
        {
                return sock_fd;
        }

        /*
         *      Service opening declaration
         */
        if (listen(sock_fd, 10) == -1)
        {
                return -ERR_SERVER_LISTEN;
        }

        return sock_fd;
}


/**
 *  \brief Timeout waiting function.
 *
 *         This function waits for incomming communications on a file
 *         descriptor until a timeout occur.
 *
 * @param fd            the file descriptor on which to wait for data
 * @param timeout       timeout, in seconds
 * @return              the status of the wait
 * @retval SUCCESS              successful, some data are available on the socket
 * @retval -ERR_CONNECTION      connection lost or connection error
 * @retval -ERR_TIMEOUT         timeout elapsed
 */
int wait_timeout(int fd, int timeout)
{
        if (fd < 0)
        {
                return -ERR_CONNECTION;
        }

        if (timeout > 0)
        {
                int            result;
                fd_set         read_set;
                struct timeval tv;

                FD_ZERO(&read_set);
                FD_SET(fd, &read_set);
                tv.tv_sec = timeout;
                tv.tv_usec = 0;
                result = select(fd + 1, &read_set, NULL, NULL, &tv);
                if (result == 0)
                {
                        return -ERR_TIMEOUT;
                }
                else if (result < 0)
                {
                        return -ERR_CONNECTION;
                }
        }

        return SUCCESS;
}


/**
 *  \brief Incoming connection waiting function.
 *
 *         This function accepts incoming connections on the given listening
 *         socket. It waits until a connection is made and the returns the file
 *         descriptor of the created service socket.
 *
 * @param socket_server         listening socket for incomming connections
 * @param timeout               timeout until the operation is aborted (0
 *                              means that the wait is not bounded)
 * @return                      the file descriptor of the incomming connection
 *                              socket or a negative value is case of error
 * @retval -ERR_SERVICE         could not service connections
 * @retval -ERR_CONNECTION      connection lost
 * @retval -ERR_TIMEOUT         timeout elapsed
 */
int accept_connection(int socket_server, int timeout)
{
        int                delay;
        int                sock_fd;
        int                len_address = sizeof(struct sockaddr_in);
        struct sockaddr_in address;

        delay = wait_timeout(socket_server, timeout);
        if (delay < 0)
        {
                return delay;
        }

        /*
         *      Waiting for a connection
         */
        sock_fd = accept(socket_server,
                        (struct sockaddr *)&address,
                        (socklen_t *) &len_address);

        /*
         *      Got SIGPIPE
         */
        if ((sock_fd == -1) && (errno == EINTR))
        {
                return -ERR_SERVICE;
        }

        /*
         *      No connection (non blocking mode, waiting for a termination
         *      order)
         */
        if ((sock_fd == -1) && (errno == EAGAIN))
        {
                return -ERR_SERVICE;
        }

        /*
         *      Other socket error
         */
        if (sock_fd == -1)
        {
                return -ERR_CONNECTION;
        }

        return sock_fd;
}


/**
 *  \brief Socket remote hostname function.
 *
 *        This function gets the remote hostname of the given socket. A NULL
 *        value for a char * parameter makes this parameter information not
 *        returned.
 *
 * @param fd                    socket to analyse
 * @param name                  hostname of the remote machine
 * @param len                   maximum length for storing the hostname
 * @return                      the status of the operation
 * @retval SUCCESS              all information found
 * @retval -ERR_NOT_FOUND       at least one of the information is not found
 */
int socket_remote_host(int fd, char * name, int len)
{
        struct sockaddr_in info;
#ifdef OpenBSD
        int                infolen;
#else
        socklen_t          infolen;
#endif

        infolen = sizeof(info);
        if (getpeername(fd, (struct sockaddr *) &info, &infolen) != 0)
        {
                return -ERR_NOT_FOUND;
        }
        if (name)
        {
                struct hostent * machine = gethostbyaddr((char *)&info.sin_addr,
                                                         sizeof(info.sin_addr),
                                                         info.sin_family);
                if (machine == NULL)
                {
                        return -ERR_NOT_FOUND;
                }

                strncpy(name, machine->h_name, len);
        }
        return SUCCESS;
}


/**
 *  \brief Socket remote IP address function.
 *
 *        This function gets the remote IP address of the given socket. A NULL
 *        value for a char * parameter makes this parameter information not
 *        returned.
 *
 * @param fd                    socket to analyse
 * @param addr                  IP address of the remote machine
 * @param len                   maximum length for storing the IP address
 * @return                      the status of the operation
 * @retval SUCCESS              all information found
 * @retval -ERR_NOT_FOUND       at least one of the information is not found
 */
int socket_remote_ip(int fd, char * addr, int len)
{
        struct sockaddr_in info;
#ifdef OpenBSD
        int                infolen;
#else
        socklen_t          infolen;
#endif

        infolen = sizeof(info);
        if (getpeername(fd, (struct sockaddr *) &info, &infolen) != 0)
        {
                return -ERR_NOT_FOUND;
        }
        if (addr)
        {
                strncpy(addr, inet_ntoa(info.sin_addr), len);
        }
        return SUCCESS;
}


/**
 *  \brief Socket remote port information function.
 *
 *        This function returns the remote port ofthe given socket.
 *
 * @param fd                    socket to analyse
 * @return                      the remote port of the socket or -ERR_NOT_FOUND
 *                              in case of error
 */
int socket_remote_port(int fd)
{
        struct sockaddr_in info;
#ifdef OpenBSD
        int                infolen;
#else
        socklen_t          infolen;
#endif

        infolen = sizeof(info);
        if (getpeername(fd, (struct sockaddr *) &info, &infolen) != 0)
        {
                return -ERR_NOT_FOUND;
        }
        return ntohs(info.sin_port);
}


/**
 *  \brief Socket local port information function.
 *
 *        This function returns the local port of the given socket.
 *
 * @param fd                    socket to analyse
 * @return                      the local port of the socket or -ERR_NOT_FOUND
 *                              in case of error
 */
int socket_local_port(int fd)
{
        struct sockaddr_in info;
#ifdef OpenBSD
        int                infolen;
#else
        socklen_t          infolen;
#endif

        infolen = sizeof(info);
        if (getsockname(fd, (struct sockaddr *) &info, &infolen) != 0)
        {
                return -ERR_NOT_FOUND;
        }
        return ntohs(info.sin_port);
}
