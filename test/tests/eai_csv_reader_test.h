//
// Created by DavideLoconte on 07-Nov-22.
//

#ifndef EAI_TOOLBOX_EAI_CSV_READER_TEST_H
#define EAI_TOOLBOX_EAI_CSV_READER_TEST_H

bool test_eai_csv_reader1(void);
bool test_eai_csv_reader2(void);
bool test_eai_csv_reader3(void);
bool test_eai_csv_reader_multiple(void);
bool test_eai_csv_reader_errors(void);

#define EAI_CSV_READER_TESTS                                                                       \
    test_eai_csv_reader_multiple, test_eai_csv_reader2, test_eai_csv_reader1,                      \
        test_eai_csv_reader3, test_eai_csv_reader_errors

#endif // EAI_TOOLBOX_EAI_CSV_READER_TEST_H
