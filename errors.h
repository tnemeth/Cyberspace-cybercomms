/**
 *  \file    errors.h
 *  \brief   Error handling file.
 *
 *           Project: project independant file.
 *
 *           This is the header file for errors.c. It contains the structure
 *           defining all the errors in a C-language way.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    2005
 */


#ifndef ERRORS_H
#define ERRORS_H

/**
 *  \addtogroup errinternals
 *  @{
 */

/*! This macro checks the return value of a function and exits the current
 *  calling function if an error is encountered.
 *
 *  @param function     a function call
 */
#define check(function)                 \
        {                               \
                int status = function;  \
                if (status != SUCCESS)  \
                {                       \
                        return status;  \
                }                       \
        }

/*! This macro simply take the parameter and put it in the errors enum. */
#define ADD_ERR(err, msg) err,

/** @}*/


/**
 *  \defgroup errenum Enum for errors
 *
 *            Due to the nature of the enum construction, all values may not
 *            be visible in the resulting document.
 *  @{
 */
/*! This enum lists all the available error codes. */
enum {
#include "errors_list.h"
};
/** @} */

#undef ADD_ERR


/** @cond DUPLICATE_DOCUMENTATION */
char * get_error_name(int error_id);
char * get_error_info(int error_id);
/** @endcond */


#endif /* ERRORS_H */

