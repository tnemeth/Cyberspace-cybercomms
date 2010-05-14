/**
 *  \file    errors_list.h
 *  \brief   Errors list file.
 *
 *           Project: project independant file.
 *
 *           This file contains the commands to create and use all the
 *           possible errors in a program. It is included in errors.h in
 *           order to create the data structure for all errors and in
 *           errors.c in order to be able to print the errors names.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    2005
 */


ADD_ERR(SUCCESS,                "No error")
ADD_ERR(ERR_UNKNOWN,            "Unknown error")
ADD_ERR(ERR_NO_MEMORY,          "No more memory")
ADD_ERR(ERR_NOT_FOUND,          "Element not found")
/* Communications */
ADD_ERR(ERR_CREATE_SOCKET,      "Cannot create socket")
ADD_ERR(ERR_CONFIGURE_SOCKET,   "Cannot configure socket")
ADD_ERR(ERR_UNKNOWN_ADDRESS,    "Unknown IP address / machine")
ADD_ERR(ERR_BIND_SOCKET,        "Cannot bind socket to a port")
ADD_ERR(ERR_SERVER_INFO,        "Cannot get server info")
ADD_ERR(ERR_CONNECT_SERVER,     "Cannot connect server")
ADD_ERR(ERR_SERVER_LISTEN,      "Cannot listen for incomming connections")
ADD_ERR(ERR_SERVICE,            "Cannot create service socket")
ADD_ERR(ERR_SERVICE_RUNNING,    "Service already running")
ADD_ERR(ERR_SERVICE_NOAUTH,     "Service not authorised")
ADD_ERR(ERR_NO_DATA,            "No data in socket")
ADD_ERR(ERR_CONNECTION_LOST,    "Connection lost")
ADD_ERR(ERR_CONNECTION,         "Unknown connection error")
ADD_ERR(ERR_TIMEOUT,            "Communication timeout")
/* Checks for local parameters and internal problems */
ADD_ERR(ERR_NO_FILE,            "File does not exist")
ADD_ERR(ERR_OPEN_DEVICE,        "Cannot open device")
ADD_ERR(ERR_CONFIG_DEVICE,      "Cannot configure device")
ADD_ERR(ERR_READ_DEVICE,        "Cannot read device data")
ADD_ERR(ERR_WRITE_DEVICE,       "Cannot write data to device")
ADD_ERR(ERR_EXEC_COMMAND,       "Cannot execute command")
/* Calculator's answers */
ADD_ERR(ERR_CANNOT_CONFIGURE,   "Cannot configure")
ADD_ERR(ERR_BAD_PROTOCOL,       "Bad protocol")
ADD_ERR(ERR_FIFO_FULL,          "FIFO full")
/* Parsing and evaluations errors */
ADD_ERR(ERR_DATA_INVALID,       "Data invalid")
ADD_ERR(ERR_UNKNOWN_OPTION,     "Unknown option")
ADD_ERR(ERR_BAD_PARAMETER,      "Bad parameter")
ADD_ERR(ERR_BAD_EXPRESSION,     "Bad expression")
ADD_ERR(ERR_PARSE,              "Parsing error")
ADD_ERR(ERR_DIV_BY_ZERO,        "Division by zero")
ADD_ERR(ERR_OUT_OF_RANGE,       "Value out of range")
ADD_ERR(ERR_MISSING_VALUE,      "Missing value")
ADD_ERR(ERR_INVALID_VALUE,      "Invalid value")
ADD_ERR(ERR_INVALID_EXPRESSION, "Invalid operand or value")

