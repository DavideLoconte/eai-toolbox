#ifndef EAI_TOOLBOX_EAI_NARRAY_TEST_H
#define EAI_TOOLBOX_EAI_NARRAY_TEST_H

#include <stdbool.h>

bool eai_narray_base_test(void);
bool eai_narray_set_test(void);
bool eai_narray_get_test(void);
bool eai_narray_get_ref_test(void);

#define EAI_NARRAY_TESTS                                                                           \
    eai_narray_base_test, eai_narray_set_test, eai_narray_get_test, eai_narray_get_ref_test

#endif // EAI_TOOLBOX_EAI_PROFILE_TEST_H
