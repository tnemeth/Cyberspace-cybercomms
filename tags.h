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
 *  Disconnect message & Shutdown     & 0xFF &      & X    & \\
 *  \hline
 *  Acknowledge        & Acknowledge  & 0xFA &      & X    & \\
 *  \hline
 *  Error message      & Error        & 0xEF &      & X    & X \\
 *  \hline
 *  \end{tabular}
 *  \endlatexonly
 *
 *  @{
 */

#define PACKET_SHUTDOWN_CMD 0xFF  /*!< Shutdown order from MAID                */

#define PACKET_MSG_ACK      0xFA  /*!< Acknowledge message from MAID.          */

#define PACKET_MSG_ERROR    0xEF  /*!< Error message.                          */

/** @} */


#endif /* TAGS_H */

