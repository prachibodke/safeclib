/*------------------------------------------------------------------
 * test_strlastdiff_s
 * File 'extstr/strlastdiff_s.c'
 * Lines executed:100.00% of 29
 *
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#define LEN   ( 128 )
#define SHORT_LEN  ( 5 )

int main()
{
    errno_t rc;
    rsize_t ind;

    char   str1[LEN];
    char   str2[LEN];
    int errs = 0;

/*--------------------------------------------------*/

    rc = strlastdiff_s(NULL, LEN, str2, &ind);
    ERR(ESNULLP)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strlastdiff_s(str1, LEN, NULL, &ind);
    ERR(ESNULLP)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strlastdiff_s(str1, LEN, str2, NULL);
    ERR(ESNULLP)
/*--------------------------------------------------*/

    rc = strlastdiff_s(str1, 0, str2, &ind);
    ERR(ESZEROL)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    rc = strlastdiff_s(str1, RSIZE_MAX_STR+1, str2, &ind);
    ERR(ESLEMAX)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = strlastdiff_s(str1, LEN, str2, &ind);
    ERR(ESNODIFF)
    if (ind != 0) {
        debug_printf("%s %u  Error  ind=%d rc=%d \n",
                     __FUNCTION__, __LINE__, (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "Keep iT simple");
    strcpy (str2, "keep it simple");

    rc = strlastdiff_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 6) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "kEep it simple");
    strcpy (str2, "keep it simple");

    rc = strlastdiff_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 1) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it siMple");
    strcpy (str2, "keEp it Simple");

    rc = strlastdiff_s(str1, LEN, str2, &ind);
    ERR(EOK)
    if (ind != 10) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");

    rc = strlastdiff_s(str1, LEN, str1, &ind);
    ERR(ESNODIFF)
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep it simple");

    rc = strlastdiff_s(str1, 1, str2, &ind);
    ERR(ESNODIFF)
    if (ind != 0) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    strcpy (str1, "keep it simple");
    strcpy (str2, "keep it simplE");

    rc = strlastdiff_s(str1, 25, str2, &ind);
    ERR(EOK)
    if (ind != 13) {
        debug_printf("%s %u  Error ind=%d  rc=%d \n",
                     __FUNCTION__, __LINE__,  (int)ind, rc);
        errs++;
    }
/*--------------------------------------------------*/

    return (errs);
}
