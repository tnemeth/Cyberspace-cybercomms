/**
 *  \file    xmem.h
 *  \brief   Memory handling.
 *
 *           Project: project independant file.
 *
 *           This is the xmem.c header file and it contains all the macros
 *           and functions declarations related to packets transmissions.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    lun 29 aoû 2005
 */


#ifndef XMEM_H
#define XMEM_H

#include <string.h>
#include <strings.h>

/**
 *  \defgroup memhdl Memory handling macros
 *  @{
 */

/*! This macro checks wether a string is empty or not. */
#define IS_EMPTY(string) (! string || (strcmp(string, "") == 0))

/*! This macro sets a string to a specified value. */
#define SET_STRING(string, value) \
{ \
        if (string) \
        { \
                free(string); \
        } \
        string = xstrdup(value); \
}

/*! This macros frees an allocated memory pointer and sets it to NULL. */
#define FREE(data) \
{ \
        if (data) \
        { \
                free(data); \
        } \
        data = NULL; \
}

/** @} */


/** @cond DUPLICATE_DOCUMENTATION */
void * xmalloc(size_t size);
char * xstrdup(const char *string);
/** @endcond */


#endif

