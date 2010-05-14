/**
 *  \file    packets.h
 *  \brief   Packets handling.
 *
 *           Project: project independant file.
 *
 *           This is the packets.c header file and it contains all the macros
 *           and functions declarations related to packets transmissions.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    lun 03 oct 2005
 */


#ifndef PACKETS_H
#define PACKETS_H

/**
 *  \defgroup packets Packets information constants
 *  @{
 */
/*! Size in bytes of the LEN field (length of the data within a packet) */
#define PACKET_LEN_SIZE   2

/*! Size in bytes of the TAG field */
#define PACKET_TAG_SIZE   1

/*! Size in bytes of a message information */
#define PACKET_MSG_SIZE   2

/*! Size of the header of a packet */
#define PACKET_HEADER_SIZE (PACKET_LEN_SIZE + PACKET_TAG_SIZE)

/*! Maximum data size in a packet (64 kB without the TAG) */
#define MAX_DATA_SIZE     65534

/*! Maximum full size of a packet */
#define MAX_PACKET_SIZE   (PACKET_HEADER_SIZE + MAX_DATA_SIZE)
/** @} */


/** @cond DUPLICATE_DOCUMENTATION */
int packet_read(int socket_fd, unsigned char * data, int size);

void packet_create(int type, unsigned char * data, int size, unsigned char * packet);

int packet_send(int socket_fd, unsigned char * data);

void message_create(int type, int message, unsigned char * packet);

int message_send(int socket_fd, int type, int message);

int packet_data_len(const unsigned char * packet);

int packet_type(const unsigned char * packet);

char * packet_error_message(const unsigned char * packet);
/** @endcond */

#endif /* PACKETS_H */

