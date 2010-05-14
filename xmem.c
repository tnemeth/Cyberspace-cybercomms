/**
 *  \file    xmem.c
 *  \brief   Memory handling.
 *
 *           Project: project independant file.
 *
 *           This file contains all the functions related to memory
 *           allocation for programs that need secure allocation. These
 *           function are not always needed: they are just provided for
 *           convenience.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    lun 29 aoû 2005
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "xmem.h"


/**
 *  \brief Function allocating memory.
 *
 *         This function allocates memory in a sure way: if the memory
 *         cannot be allocated, the program that uses this function
 *         exits.
 *
 * @param size          size of memory to allocate
 * @return              a pointer to the allocated memory
 */
void * xmalloc(size_t size)
{
        void * ret = malloc(size);

        if (! ret)
        {
                perror("malloc() ");
                exit(-1);
        }

        return ret;
}


/**
 *  \brief Function duplicating a string.
 *
 *         This function duplicates a character string in a sure way: if
 *         the memory needed for duplication cannot be allocated, the
 *         program that uses this function exits.
 *
 * @param string        the string to duplicate
 * @return              a pointer to the newly duplicated string
 */
char * xstrdup(const char * string)
{
        char * ret = strdup(string);

        if (! ret)
        {
                perror("strdup() ");
                exit(-1);
        }

        return ret;
}
