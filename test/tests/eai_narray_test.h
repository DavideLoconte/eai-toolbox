#ifndef EAI_TOOLBOX_EAI_NARRAY_TEST_H
#define EAI_TOOLBOX_EAI_NARRAY_TEST_H

#include <ulib.h>

void eai_narray_base_test(void);
void eai_narray_set_test(void);
void eai_narray_get_test(void);
void eai_narray_get_ref_test(void);
void eai_narray_shape_test(void);
void eai_narray_iter_test(void);

#define EAI_NARRAY_TESTS                                                                           \
    eai_narray_base_test, eai_narray_set_test, eai_narray_get_test, eai_narray_get_ref_test, eai_narray_shape_test, eai_narray_iter_test

#endif // EAI_TOOLBOX_EAI_PROFILE_TEST_H
