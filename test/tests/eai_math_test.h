//
// Created by DavideLoconte on 07-Nov-22.
//

#ifndef EAI_TOOLBOX_EAI_MATH_TEST_H
#define EAI_TOOLBOX_EAI_MATH_TEST_H

#include <ulib.h>

bool eai_math_test_int_log2(void);
bool eai_math_test_int_log10(void);
bool eai_math_test_int_pow2(void);
bool eai_math_test_int_next_pow2(void);
bool eai_math_test_float_sqrt(void);

#define EAI_MATH_TESTS                                                                             \
    eai_math_test_int_log2, eai_math_test_int_pow2, eai_math_test_int_log10,                       \
        eai_math_test_int_next_pow2, eai_math_test_float_sqrt

#endif // EAI_TOOLBOX_EAI_MATH_TEST_H
