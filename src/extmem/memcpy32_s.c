/*------------------------------------------------------------------
 * memcpy32_s
 *
 * October 2008, Bo Berry
 * October 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 Cisco Systems
 * Copyright (c) 2017 Reini Urban
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
#include "safe_mem_constraint.h"
#include "mem_primitives_lib.h"
#include "safe_mem_lib.h"


/**
 * @brief 
 *    This function copies at most smax uint32_ts from src to dest, up to
 *    dmax.
 *
 * @remark EXTENSION TO
 *    ISO/IEC JTC1 SC22 WG14 N1172, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[out] dest   pointer to the memory that will be replaced by src.
 * @param[in]  dmax   maximum length of the resulting dest, in bytes
 * @param[in]  src    pointer to the memory that will be copied to dest
 * @param[in]  smax   maximum number bytes of src that can be copied
 *
 * @pre   Neither dest nor src shall be a null pointer.
 * @pre   dmax shall not be 0.
 * @pre   dmax shall not be greater than RSIZE_MAX_MEM.
 * @pre   smax shall not be greater than dmax.
 * @pre   Copying shall not take place between regions that overlap.
 *    
 * @return  If there is a runtime-constraint violation, the memcpy_s function
 *          stores zeros in the ﬁrst dmax bytes of the region pointed to
 *          by dest if dest is not a null pointer and smax is valid.
 * @retval  EOK         when operation is successful
 * @retval  ESNULLP     when dst/src is NULL POINTER
 * @retval  ESZEROL     when dmax = ZERO. Before C11 also with smax = ZERO
 * @retval  ESLEMAX     when dmax/smax > RSIZE_MAX_MEM
 * @retval  ESNOSPC     when dmax < smax
 * @retval  ESOVRLP     when src memory overlaps dst
 *
 * @see 
 *    memcpy_s(), memcpy16_s(), memmove_s(), memmove16_s(), memmove32_s()
 *
 */
EXPORT errno_t
memcpy32_s (uint32_t *dest, rsize_t dmax, const uint32_t *src, rsize_t smax)
{
    if (unlikely(dest == NULL)) {
        invoke_safe_mem_constraint_handler("memcpy32_s: dest is NULL",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_mem_constraint_handler("memcpy32_s: dmax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
    }

    if (unlikely(dmax > RSIZE_MAX_MEM32 || smax > RSIZE_MAX_MEM32)) {
        if (dmax < RSIZE_MAX_MEM32) {
            mem_prim_set32(dest, dmax, 0);
        }
        invoke_safe_mem_constraint_handler("memcpy32_s: dmax/smax exceeds max",
                   NULL, ESLEMAX);
        return (RCNEGATE(ESLEMAX));
    }

    if (unlikely(smax == 0)) {
        /* Since C11 smax=0 is allowed */
#ifdef HAVE_C11
        return EOK;
#else
        mem_prim_set32(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy32_s: smax is 0",
                   NULL, ESZEROL);
        return (RCNEGATE(ESZEROL));
#endif
    }

    if (unlikely(smax > dmax)) {
        mem_prim_set32(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy32_s: smax exceeds dmax",
                   NULL, ESNOSPC);
        return (RCNEGATE(ESNOSPC));
    }

    if (unlikely(src == NULL)) {
        mem_prim_set32(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy32_s: src is NULL",
                   NULL, ESNULLP);
        return (RCNEGATE(ESNULLP));
    }

    /*
     * overlap is undefined behavior, do not allow
     */
    if (unlikely( ((dest > src) && (dest < (src+smax))) ||
                  ((src > dest) && (src < (dest+dmax))) )) {
        mem_prim_set32(dest, dmax, 0);
        invoke_safe_mem_constraint_handler("memcpy32_s: overlap undefined",
                   NULL, ESOVRLP);
        return (RCNEGATE(ESOVRLP));
    }

    /*
     * now perform the copy
     */
    mem_prim_move32(dest, src, smax);

    return (RCNEGATE(EOK));
}
EXPORT_SYMBOL(memcpy32_s)
