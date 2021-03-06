/*------------------------------------------------------------------
 * test_strcpyfldout_s
 * File 'extstr/strcpyfldout_s.c'
 * Lines executed:100.00% of 49
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

int main()
{
    errno_t rc;
    uint32_t i;
    rsize_t len;
    rsize_t slen;

    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = strcpyfldout_s(NULL, LEN, str2, LEN);
    ERR(ESNULLP)

/*--------------------------------------------------*/

    len = 5;
    rc = strcpyfldout_s(str1, len, NULL, LEN);
    ERR(ESNULLP)
    for (i=0; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u   Error rc=%u \n",
                     __FUNCTION__, __LINE__,  rc );
            errs++;
        }
    }

/*--------------------------------------------------*/

    rc = strcpyfldout_s(str1, 0, str2, LEN);
    ERR(ESZEROL)
/*--------------------------------------------------*/

    rc = strcpyfldout_s(str1, (RSIZE_MAX_STR+1), str2, LEN);
    ERR(ESLEMAX)
/*--------------------------------------------------*/

    len = 5;
    slen = 0;
    rc = strcpyfldout_s(str1, len, str2, slen);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    len = 5;
    slen = 6;
    rc = strcpyfldout_s(str1, len, str2, slen);
    ERR(ESNOSPC)

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 1;
    slen = 1;
    rc = strcpyfldout_s(str1, len, str2, slen);
    ERR(EOK)
    /* str1 becomes null */
    for (i=0; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                   __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 2;
    slen = 2;
    rc = strcpyfldout_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<len-1; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    strcpy(str2, "01234567890123456789");

    len = 3;
    slen = 3;
    rc = strcpyfldout_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<len-1; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    /* same string in dest and src */
    len = LEN;
    rc = strcpyfldout_s(str1, len, str1, len);
    ERR(ESOVRLP)
    for (i=0; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple for best results");
    len = strlen(str1);

    /* overlap */
    rc = strcpyfldout_s(&str1[0], len, &str1[5], len);
    ERR(ESOVRLP)
    for (i=0; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple for best results ");
    len = 20;

    /* overlap */
    rc = strcpyfldout_s(&str1[10], len, &str1[0], len);
    ERR(ESOVRLP)
    for (i=10; i<len; i++) {
        if (str1[i] != '\0') {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "keep it simple");
    len = strlen(str1);

    strcpy(str2, "aaaaaaaaaaaaaa");
    slen = strlen(str2);

    rc = strcpyfldout_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<len-1; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    strcpy(str1, "always keep it simple");
    len = strlen(str1);

    strcpy(str2, "keep it simple");
    slen = strlen(str2);

    rc = strcpyfldout_s(str1, len, str2, slen);
    ERR(EOK)
    for (i=0; i<slen; i++) {
        if (str1[i] != str2[i]) {
            debug_printf("%s %u  diff s1[%d]=%d  s2[%d]=%d  rc=%u \n",
                     __FUNCTION__, __LINE__, i, str1[i], i, str2[i], rc);
            errs++;
        }
    }

/*--------------------------------------------------*/

    return (errs);
}
