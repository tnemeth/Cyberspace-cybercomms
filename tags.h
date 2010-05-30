/**
 *  \file    tags.h
 *  \brief   Tags definitions.
 *
 *           Project: Network communication system.
 *
 *           This file contains all the tags definitions for an example
 *           communications as defined in the conception document.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    mer 12 sep 2007
 */


#ifndef TAGS_H
#define TAGS_H

/**
 *  \defgroup tags Messages tags for all communications
 *
 *  \brief Tags definitions in communications messages for all related
 *         softwares.
 *
 *  \details
 *  Network communications between the different softwares are done
 *  using a common protocol. Each packet sent to the laptop or received from
 *  it follows a simple format:
 *  - <tt>2 bytes</tt> describing the packet's length in bytes (size)
 *  - <tt>1 byte</tt>  describing the packet's content (tag)
 *  - <tt>x bytes</tt> optionnal data of the packet (data)
 *
 *  The size is coded in little-endian order: the least significant byte
 *  is transmitted first, then the most significant byte.
 *
 *  The tag is a single byte that can be a command (with optional parameters
 *  in the data section) or a data description for the following data in the
 *  packet.
 *
 *  \latexonly
 *  Here is the description of the tags used in the system:\\
 *
 *  \begin{tabular}{|l|l|c|c|c|c|}
 *  \hline
 *  {\em Category}     & {\em Description} & {\em Tag} & {\em C->S}
 *                                                     & {\em S->C} & {\em Data} \\
 *  \hline
 *  \hline
 *  Acknowledge        & Acknowledge  & 0xFA &      & X    & \\
 *  \hline
 *  Not Acknowledge    & Not Acknowledge & 0xFA &      & X    & \\
 *  \hline
 *  Error message      & Error        & 0xFF &      & X    & X \\
 *  \hline
 *  \end{tabular}
 *  \endlatexonly
 *
 *  @{
 */

#define CMD_NOOP            0x00
#define CMD_GET_PARAM       0x01
#define CMD_SET_PARAM       0x02
#define CMD_ADD_OBJECT      0x03
#define CMD_DEL_OBJECT      0x04
#define CMD_LOAD_CONFIG     0x05
#define CMD_SAVE_CONFIG     0x06
#define CMD_DUMP_STATE      0x07
#define CMD_SET_SELECTION   0x08
#define CMD_DISCONNECT      0x0D

#define PACKET_MSG_ACK      0xFA  /*!< Acknowledge message from server.     */
#define PACKET_MSG_NACK     0xFB  /*!< Acknowledge message from server.     */
#define PACKET_MSG_ERROR    0xFF  /*!< Error message.                       */

/** @} */


#endif /* TAGS_H */

