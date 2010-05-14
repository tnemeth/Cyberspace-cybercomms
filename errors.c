/**
 *  \file    errors.c
 *  \brief   Error handling file.
 *
 *           Project: project independant file.
 *
 *           This file contains the errors management function that allow
 *           to display an error id into a human-readable value.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    2005
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

/**
 *  \defgroup errinternals Error management variables and macros
 *  @{
 */

/*! Error information storage. */
static char * error_string = NULL;


/*! This macro sets the \c error_string variable to the corresponding
 *  string constant.
 */
#define ADD_ERR(err, msg) case -err: error_string = #err; break;
/** @} */


/**
 *  \brief Function returning the error's name in a human readable string.
 *
 *  @param error_id      received error number
 *  @return              corresponding error name
 */
char * get_error_name(int error_id)
{
        switch (error_id)
        {
#include "errors_list.h"
                default: error_string = "ERR_UNKNOWN";
        }

        return error_string;
}

#undef ADD_ERR


/**
 *  \addtogroup errinternals
 *  @{
 */
/*! This macro sets the \c error_string variable to the corresponding
 *  error description string.
 */
#define ADD_ERR(err, msg) case -err: error_string = msg; break;
/** @} */


/**
 *  \brief Function returning the error's information in a human readable string.
 *
 *  @param error_id      received error number
 *  @return              corresponding error description string
 */
char * get_error_info(int error_id)
{
        switch (error_id)
        {
#include "errors_list.h"
                default: error_string = "Unknown error";
        }

        return error_string;
}

#undef ADD_ERR

