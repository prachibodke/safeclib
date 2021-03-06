/*------------------------------------------------------------------
 * test_memcpy_s
 * File 'mem/memcpy_s.c'
 * Lines executed:100.00% of 30
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_mem_lib.h"

#define LEN   ( 1024 )

static uint8_t  mem1[LEN+2];
static uint8_t  mem2[LEN+2];

int test_memcpy_s (void)
{
    errno_t rc;
    uint32_t i;
    rsize_t len;
    int errs = 0;

/*--------------------------------------------------*/

    rc = memcpy_s(NULL, LEN, mem2, LEN);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, 0, mem2, LEN);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, RSIZE_MAX_MEM+1, mem2, LEN);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, LEN, NULL, LEN);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    rc = memcpy_s(mem1, 10, mem2, 0);
#ifdef HAVE_C11
    ERR(EOK);
#else
    ERR(ESZEROL)
    CHECK_SLACK(mem1, 10);
#endif
  
/*--------------------------------------------------*/

    rc = memcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM+1);
    ERR(ESLEMAX);
    CHECK_SLACK(mem1, LEN);

/*--------------------------------------------------*/

    for (i=0; i<LEN+1; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, len);
    ERR(EOK)
    else {

    for (i=0; i<len; i++) {
        if (mem1[i] != mem2[i]) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

    if (mem1[i] != 33) {
        debug_printf("%d - %d m1=%d  m2=%d  \n",
               __LINE__, (int)len, mem1[len], mem2[len]);
        errs++;
    }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN+1; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, (len+1) );
    if (rc == EOK) {
        debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc);
        errs++;

    } else {

    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

    if (mem1[i] != 33) {
        debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, (int)len, mem1[len], mem2[len]);
        errs++;
    }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN+2; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN/2;
    rc = memcpy_s(mem1, len, mem2, LEN);
    ERR(ESNOSPC)
    else {

    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

    if (mem1[len] != 33) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, (int)len, mem1[len], mem2[len]);
        errs++;
    }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN+2; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, 0);
#ifdef HAVE_C11
    ERR(EOK)
#else
    ERR(ESZEROL)
#endif

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = LEN;
    rc = memcpy_s(mem1, len, mem2, RSIZE_MAX_MEM+1);
    ERR(ESLEMAX)
    else {

    /* verify mem1 was zeroed */
    for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
    }

    if (mem1[len] == 0) {
        debug_printf("%d - %d m1=%d  m2=%d  \n",
             __LINE__, i, mem1[i], mem2[i]);
        errs++;
    }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }

    /* same ptr - no move */
    rc = memcpy_s(mem1, LEN, mem1, LEN);
    ERR(EOK)

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }
    for (i=0; i<LEN; i++) { mem2[i] = 65; }

    /* overlap */
    len = 100;
    rc = memcpy_s(&mem1[0], len, &mem1[10], len);
    ERR(ESOVRLP)
    else {

      for (i=0; i<len; i++) {
        if (mem1[i] != 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
            errs++;
        }
      }

      if (mem1[len] == 0) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        errs++;
      }
    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 55; }
    for (i=0; i<LEN; i++) { mem2[i] = 65; }

    /* overlap */
    len = 100;
    rc = memcpy_s(&mem1[10], len, &mem1[0], len);
    ERR(ESOVRLP)
    else {

      for (i=10; i<len+10; i++) {
        if (mem1[i] != 0) {
          debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
      }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 35; }
    for (i=0; i<LEN; i++) { mem2[i] = 55; }

    len = 5;
    rc = memcpy_s(mem1, len, mem2, len);
    ERR(EOK)
    else {

      for (i=0; i<len; i++) {
        if (mem1[i] != 55) {
            debug_printf("%d - %d m1=%d\n",
                 __LINE__, i, mem1[i]);
        }
      }

    }

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 35; }
    for (i=0; i<LEN; i++) { mem2[i] = 55; }

    rc = memcpy_s(mem1, LEN, mem2, LEN/2);
    ERR(EOK)
    else {

      for (i=0; i<LEN/2; i++) {
        if (mem1[i] != 55) {
            debug_printf("%d - %d m1=%d  m2=%d  \n",
                 __LINE__, i, mem1[i], mem2[i]);
        }
      }
    }

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_memcpy_s());
}
#endif
