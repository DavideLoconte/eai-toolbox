//
// Created by DavideLoconte on 07-Nov-22.
//

#ifndef EAI_TOOLBOX_EAI_PROFILE_TEST_H
#define EAI_TOOLBOX_EAI_PROFILE_TEST_H

#include <ulib.h>

bool eai_profile_test_wall_clock(void);
bool eai_profile_test_sleep(void);
bool eai_profile_test_rss(void);

#define EAI_PROFILE_TESTS eai_profile_test_wall_clock, eai_profile_test_sleep, eai_profile_test_rss

#endif // EAI_TOOLBOX_EAI_PROFILE_TEST_H
