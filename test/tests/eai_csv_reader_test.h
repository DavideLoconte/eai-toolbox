//
// Created by DavideLoconte on 07-Nov-22.
//

#ifndef EAI_TOOLBOX_EAI_CSV_READER_TEST_H
#define EAI_TOOLBOX_EAI_CSV_READER_TEST_H

#include <ulib.h>

void test_eai_csv_reader1(void);
void test_eai_csv_reader2(void);
void test_eai_csv_reader3(void);
void test_eai_csv_reader_multiple(void);
void test_eai_csv_reader_errors(void);
void test_eai_csv_reader_header(void);

#define EAI_CSV_READER_TESTS                                                                       \
    test_eai_csv_reader_multiple, test_eai_csv_reader2, test_eai_csv_reader1,                      \
        test_eai_csv_reader3, test_eai_csv_reader_errors, test_eai_csv_reader_header

#endif // EAI_TOOLBOX_EAI_CSV_READER_TEST_H
