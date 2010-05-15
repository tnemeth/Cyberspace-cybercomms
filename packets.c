/**
 *  \file    packets.c
 *  \brief   Packets handling.
 *
 *           Project: project independant file.
 *
 *           This file contains all the functions related to data
 *           communications regarding to the implemented protocol.
 *           The exported functions include emitting data packets,
 *           reading incomming data packets, sending and receiving
 *           messages, as well as packets inspection.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    lun 03 oct 2005
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef __CYGWIN32__
#  define MSG_WAITALL 0
#endif

#include "packets.h"
#include "errors.h"


/**
 *  \brief Function reading bytes on a socket.
 *
 *         This function loops until all waited for bytes are arrived or the
 *         socket is closed.
 *
 * @param socket_fd     the socket's file descriptor
 * @param data          array where to store retreived data
 * @param size          size of data to retreive
 * @return              the total number of bytes retreived
 */
static int read_bytes(int socket_fd, unsigned char * data, int size)
{
        int total = 0;

        while (total < size)
        {
                /*
                int nb_read = read(socket_fd, &data[total], size - total);
                */
                int nb_read = recv(socket_fd, &data[total], size - total,
                                MSG_WAITALL);
                if (nb_read <= 0)
                {
                        total = -1; /* Fin de fichier : socket fermée */
                        break;
                }
                total += nb_read;
        }

        return total;
}


/**
 *  \brief Function reading a full packet on a socket.
 *
 *         This function waits for a full packet on a socket and returns the
 *         real number of bytes received. It returns 0 in case of closed
 *         connection or read error. Data are stored in a sufficiently-sized
 *         pre-allocated buffer.
 *         The returned packet is a full packet (L + TAG + DATA)
 *
 * @param socket_fd     the socket's file descriptor
 * @param data          the array where to store the received data
 * @param size          size of the data array
 * @return              the number of read bytes
 */
int packet_read(int socket_fd, unsigned char * data, int size)
{
        int           received;
        int           packet_size; /* taille convertie en entier */
        int           read_size;   /* taille à réellement lire   */

        /*
         *      On récupère d'abord la taille des données.
         */
        received = read_bytes(socket_fd, data, PACKET_LEN_SIZE);
        if (received < PACKET_LEN_SIZE)
        {
                return 0;
        }

        /*
         *      Calcul de la taille à recevoir puis de celle la plus adaptée
         *      pour mettre dans la zone fournie.
         */
        packet_size = ((data[1] << 8) | data[0]);
        read_size = (packet_size > (size - PACKET_LEN_SIZE)
                        ? (size - PACKET_LEN_SIZE)
                        : packet_size);

        /*
         *      Lecture des données du paquet.
         */
        received = read_bytes(socket_fd, &data[PACKET_LEN_SIZE], read_size);
        if (packet_size > size)
        {
                /*
                 *      Prendre en compte le cas où les données reçues sont
                 *      plus grandes que la taille du buffer passé en
                 *      paramètre : elles sont lues et non prises en compte
                 *      afin de ne pas gêner les communications protocolaires :
                 *      ne devrait jamais arriver car la taille des buffer est
                 *      maximisée.
                 */
                int           i;
                unsigned char trash;
                for (i = size ; i < packet_size ; i++)
                {
                        read(socket_fd, &trash, 1);
                }
        }

        return (received > 0) ? received + PACKET_LEN_SIZE: 0;
}


/**
 *  \brief Function creating a packet.
 *
 *         This function creates a packet, adding the associated TAG and
 *         calculating the packet's size. It returns the created packet
 *         in the sufficiently-sized given memory area.
 *
 * @param type          packet type (TAG)
 * @param data          data to include in the packet
 * @param size          size of the data to include
 * @param packet        resulting packet
 */
void packet_create(int type, unsigned char * data, int size, unsigned char * packet)
{
        unsigned char tag = (type & 0xFF);
        unsigned int  packet_size = size + PACKET_TAG_SIZE;

        /*
         *      La taille du paquet en little-endian
         */
        packet[0] = (packet_size & 0xFF);
        packet[1] = ((packet_size >> 8) & 0xFF);

        /*
         *      Le TAG
         */
        packet[PACKET_LEN_SIZE] = tag;

        /*
         *      Les données
         */
        if (size > 0)
        {
                memcpy(&packet[PACKET_HEADER_SIZE], data, size);
        }
}


/**
 *  \brief Packet emitting function.
 *
 *         This function emits a previously created packet. It returns the
 *         size of the emitted packet or 0 in case of connection loss or in
 *         case of error.
 *
 * @param socket_fd     the socket's file descriptor
 * @param data          a pointer to the packet's full data
 * @return              the emitted size or 0 in case of error
 */
int packet_send(int socket_fd, unsigned char * data)
{

        int packet_size = packet_data_len(data) + PACKET_LEN_SIZE;
        int written = 0;
        int connected = 1;

        while (connected && (written < packet_size))
        {
                /*
                int nb_write = write(socket_fd,
                                     &data[written],
                                     packet_size - written);
                */
                int nb_write = send(socket_fd,
                                    &data[written],
                                    packet_size - written, 0);
                if (nb_write <= 0)
                {
                        connected = 0;
                        //perror("Packet sending error");
                }
                else
                {
                        written += nb_write;
                }
        }

        return written;
}


/**
 *  \brief Message packet creation function.
 *
 *         This function creates a packet of message type. The type
 *         specifies the TAG (kind) of message. The packet is created
 *         and then stored in a pre-allocated buffer of size:
 *         PACKET_HEADER_SIZE + PACKET_MSG_SIZE.
 *
 * @param type          type (TAG) of message packet
 * @param message       optional value of the message information
 * @param packet        resulting packet
 */
void message_create(int type, int message, unsigned char * packet)
{
        unsigned char message_data[PACKET_MSG_SIZE];
        int           size = (message != 0) ? PACKET_MSG_SIZE : 0;

        if (message != 0)
        {
                if (message < 0)
                {
                        /*
                         *      Il ne faut utiliser que des valeurs non signées
                         */
                        message = -message;
                }
                message_data[0] = (message & 0xFF);
                message_data[1] = ((message >> 8) & 0xFF);
        }
        packet_create(type, message_data, size, packet);
}


/**
 *  \brief Message sending function.
 *
 *         This function creates a message from the given information and
 *         sends it immediately.
 *
 * @param socket_fd     socket's file descriptor
 * @param type          type (TAG) of the message
 * @param message       optional additionnal information
 * @return              the emitted size or 0 in case of error
 */
int message_send(int socket_fd, int type, int message)
{
        unsigned char message_packet[PACKET_HEADER_SIZE + PACKET_MSG_SIZE];

        message_create(type, message, message_packet);

        return packet_send(socket_fd, message_packet);
}


/**
 *  \brief Packet data size information function.
 *
 *         This function decode the 2 firsts bytes of a packet and returns
 *         the data size of the packet.
 *
 * @param packet        the packet's full data to examine
 * @return              the size of the packet's data
 */
int packet_data_len(const unsigned char * packet)
{
        unsigned char b1, b2;

        b1 = packet[0];
        b2 = packet[1];

        return ((b2 << 8) | b1);
}


/**
 *  \brief Packet TAG information function.
 *
 *         This functions extracts the TAG field of a packet and returns
 *         it.
 *
 * @param packet        the packet's full data to examine
 * @return              the TAG field of the packet
 */
int packet_type(const unsigned char * packet)
{
        return packet[PACKET_LEN_SIZE];
}


/**
 *  \brief Function returning the error message in text of an error
 *         message.
 *
 * @param packet        the packet's full data to examine
 * @return              the error message string of the packet
 */
char * packet_error_message(const unsigned char * packet)
{
        int error = packet[PACKET_HEADER_SIZE];
        error |= (packet[PACKET_HEADER_SIZE + 1] << 8);

        return get_error_name(- error);
}

