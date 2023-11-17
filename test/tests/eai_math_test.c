//
// Created by DavideLoconte on 07-Nov-22.
//

#include "eai_math_test.h"
#include "eai_int_math.h"

bool eai_math_test_int_log2(void)
{
    utest_assert(eai_int_log2(2) == 1);
    utest_assert(eai_int_log2(3) == 1);
    utest_assert(eai_int_log2(5) == 2);
    utest_assert(eai_int_log2(8) == 3);
    utest_assert(eai_int_log2(65535) == 15);
    utest_assert(eai_int_log2(65536) == 16);
    return true;
}

bool eai_math_test_int_log10(void)
{
    utest_assert(eai_int_log10(1) == 0);
    utest_assert(eai_int_log10(10) == 1);
    utest_assert(eai_int_log10(200) == 2);
    utest_assert(eai_int_log10(960) == 2);
    utest_assert(eai_int_log10(1000) == 3);
    utest_assert(eai_int_log10(10000) == 4);
    utest_assert(eai_int_log10(5000) == 3);
    utest_assert(eai_int_log10(20000) == 4);
    return true;
}

bool eai_math_test_int_pow2(void)
{
    utest_assert_int(eai_int_pow2(2), ==, 4);
    utest_assert_int(eai_int_pow2(3), ==, 8);
    utest_assert_int(eai_int_pow2(15), ==, 32768);
    utest_assert_int(eai_int_pow2(16), ==, 65536);
    return true;
}

bool eai_math_test_int_next_pow2(void)
{
    utest_assert_int(eai_int_next_pow2(1), ==, 1);
    utest_assert_int(eai_int_next_pow2(3), ==, 4);
    utest_assert_int(eai_int_next_pow2(31), ==, 32);
    utest_assert_int(eai_int_next_pow2(42), ==, 64);
    utest_assert_int(eai_int_next_pow2(69), ==, 128);
    utest_assert_int(eai_int_next_pow2(65355), ==, 65536);
    return true;
}
