/*------------------------------------------------------------------
 * wcsset_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
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
 *    Sets maximal dmax wide characters of dest to a wide character value,
 *    but not the final NULL character.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the
 *    array of dmax wide characters pointed to by dest are nulled.
 *
 * @remark EXTENSION TO
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *    * Implemented in the Windows secure API as _wcsset_s()
 *
 * @param[out]  dest    wide string that will be set.
 * @param[in]   dmax    restricted maximum length of dest
 * @param[in]   value   wide character value to write
 *
 * @retval  EOK         when successful
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_WSTR
 *
 * @see
 *    wcsnset_s(), wmemset_s(), strzero_s(), strnset_s(), strispassword_s()
 *
 */
EXPORT errno_t
wcsset_s (wchar_t *restrict dest, rsize_t dmax, wchar_t value)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("wcsset_s: dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("wcsset_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcsset_s: dmax/value exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    while (dmax && *dest) {
        *dest = value;
        dmax--;
        dest++;
    }
#ifdef SAFECLIB_STR_NULL_SLACK
    /* null slack to clear any data */
    if (!*dest)
        memset(dest, 0, dmax*sizeof(wchar_t));
#endif

    return (EOK);
}
