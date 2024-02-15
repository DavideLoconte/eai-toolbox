//
// Created by DavideLoconte on 07-Nov-22.
//

#include <ulib.h>

#include <eai_profile.h>

bool eai_profile_test_wall_clock(void)
{
    utest_assert_float(eai_profile_get_wall_clock(), >, 0);
    return true;
}

bool eai_profile_test_rss(void)
{
    ulib_uint x = eai_profile_get_resident_memory();
    utest_assert_uint(x, >, 0);
    char *abc = malloc(sizeof(char) * 1024 * 1024); // Allocating 1024 KB
    for(ulib_uint i = 0; i < 1024 * 1024; i++) {
        abc[i] = i % 250;
    }
    ulib_uint x2 = eai_profile_get_resident_memory();
    utest_assert_uint(x2, >=, x);
    utest_assert_uint(x2, >=, 1024);                 // Process occupies at least 1 MB
    char *abc2 = malloc(sizeof(char) * 1024 * 1024); // Allocating 1024 KB
    for(ulib_uint i = 0; i < 1024 * 1024; i++) {
        abc[i] = i % 250;
        abc2[i] = abc[i];
    }
    ulib_uint x3 = eai_profile_get_resident_memory();
    utest_assert_uint(x3, >=, x);
    utest_assert_uint(x3, >=, x2);
    utest_assert_uint(x3, >=, 2048); //  Process occupies at least 2 MB
    free(abc2);
    free(abc);
    return true;
}

bool eai_profile_test_sleep(void)
{
    ulib_float epsilon = 0.2;
    ulib_uint seconds = 1;
    ulib_float start = eai_profile_get_wall_clock();
    eai_profile_sleep(seconds);
    ulib_float stop = eai_profile_get_wall_clock();

    utest_assert_float((stop - start), >, seconds - epsilon);
    utest_assert_float((stop - start), <, seconds + epsilon);

    return true;
}
