/*------------------------------------------------------------------
 * strishex_s.c
 *
 * October 2008, Bo Berry
 *
 * Copyright (c) 2008-2011, 2013 by Cisco Systems, Inc
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#include "safeclib_private.h"




/**
 * @brief
 *    This function checks that the entire string contains
 *    hex characters.  The scanning stops at the first null
 *    or after dmax characters.
 *
 * @remark EXTENSION TO
 *    ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param  dest  pointer to string
 * @param  dmax  maximum length of string
 *
 * @pre  dest shall not be a null pointer.
 * @pre  dmax shall not equal zero.
 * @pre  dmax shall not be greater than RSIZE_MAX_STR.
 *
 * @return  true   when string is hex
 * @return  false  when string is not hex or an error occurred
 *
 * @see
 *    strisalphanumeric_s(), strisascii_s(), strisdigit_s(),
 *    strislowercase_s(), strismixedcase_s(),
 *    strisuppercase_s()
 *
 */
bool
strishex_s (const char *dest, rsize_t dmax)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strishex_s: dest is null",
                   NULL, ESNULLP);
        return (false);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strishex_s: dmax is 0",
                   NULL, ESZEROL);
        return (false);
    }

    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("strishex_s: dmax exceeds max",
                   NULL, ESLEMAX);
        return (false);
    }

    if (unlikely(*dest == '\0')) {
        return (false);
    }

    while (*dest && dmax) {

        if (((*dest >= '0') && (*dest <= '9')) ||
            ((*dest >= 'a') && (*dest <= 'f')) ||
            ((*dest >= 'A') && (*dest <= 'F'))) {
            dest++;
            dmax--;

        } else {
            return (false);
        }
    }

    return (true);
}
