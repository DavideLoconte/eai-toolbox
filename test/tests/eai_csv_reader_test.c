//
// Created by DavideLoconte on 07-Nov-22.
//

#include <eai_csv_reader.h>
#include <time.h>
#include <ulib.h>

bool test_eai_csv_reader1(void)
{
    const char *eai_csv_reader_test_str =
        "test, \"test2\", \"a\"test 3\"b\"\n"
        "1, \"2\", \"3\n"
        "4,5\"\",6\"\"\"\n"
        "4,5,6\n"
        "7,8,\" 9\"\"abc\"";

    const char *eai_csv_reader_test_str_crlf =
        "test; \"test2\"; \"a\"test 3\"b\"\r\n"
        "1; \"2\"; \"3\r\n"
        "4;5\"\";6\"\"\"\r\n"
        "4;5;6\r\n"
        "7;8;\" 9\"\"abc\"";

    char expected[4][4][16] = { { "test", " test2", " atest 3b" },
                                { "1", " 2", " 3\n4,5\",6\"" },
                                { "4", "5", "6" },
                                { "7", "8", " 9\"abc" } };

    char expected_crlf[4][4][16] = { { "test", " test2", " atest 3b" },
                                     { "1", " 2", " 3\r\n4;5\";6\"" },
                                     { "4", "5", "6" },
                                     { "7", "8", " 9\"abc" } };

    ulib_uint i;

    int expected_length[] = { 3, 3, 3, 3 };

    UIStream csv_stream;
    ustream_ret ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str);
    utest_assert(ret == USTREAM_OK);

    // No CRLF
    EaiCsvReader reader = eai_csv_reader(',', false);
    i = 0;
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        utest_assert(record != NULL);
        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected[i][field.i],
                                                  strlen(expected[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }
        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);

    // No CRLF
    ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str_crlf);
    utest_assert(ret == USTREAM_OK);
    reader = eai_csv_reader(';', true);

    i = 0;
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        utest_assert(record != NULL);

        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected_crlf[i][field.i],
                                                  strlen(expected_crlf[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }

        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);
    return true;
}

bool test_eai_csv_reader2(void)
{
    const char *eai_csv_reader_test_str =
        "1\t2\t3\t4\t\n"
        "5\t6\t7\n"
        "8\t9\t\"\"\n"
        "10\t11\t\"\"\"\"";

    const char *eai_csv_reader_test_str_crlf =
        "1\t2\t3\t4\t\r\n"
        "5\t6\t7\r\n"
        "8\t9\t\"\"\r\n"
        "10\t11\t\"\"\"\"";

    char expected[4][5][16] = {
        { "1", "2", "3", "4", "" }, { "5", "6", "7" }, { "8", "9", "" }, { "10", "11", "\"" }
    };

    int expected_length[] = { 5, 3, 3, 3 };

    ulib_uint i;

    UIStream csv_stream;
    ustream_ret ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str);
    utest_assert(ret == USTREAM_OK);

    // No CRLF
    EaiCsvReader reader = eai_csv_reader('\t', false);
    i = 0;
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        utest_assert(record != NULL);
        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected[i][field.i],
                                                  strlen(expected[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }
        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);

    // No CRLF
    ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str_crlf);
    utest_assert(ret == USTREAM_OK);
    reader = eai_csv_reader('\t', true);

    i = 0;
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        utest_assert(record != NULL);
        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected[i][field.i],
                                                  strlen(expected[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }
        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);
    return true;
}

bool test_eai_csv_reader3(void)
{
    const char *eai_csv_reader_test_str =
        ",2,3,4,\n"
        "\"\"\"\",6,7\n"
        ",9,\n"
        ",11,\r\n";

    const char *eai_csv_reader_test_str_crlf =
        ",2,3,4,\r\n"
        "\"\"\"\",6,7\r\n"
        ",9,\r\n"
        ",11,\r\n";
    char expected[4][5][16] = {
        { "", "2", "3", "4", "" }, { "\"", "6", "7" }, { "", "9", "" }, { "", "11", "\r" }
    };

    char expected_crlf[4][5][16] = {
        { "", "2", "3", "4", "" }, { "\"", "6", "7" }, { "", "9", "" }, { "", "11", "" }
    };

    int expected_length[] = { 5, 3, 3, 3 };

    ulib_uint i;

    UIStream csv_stream;
    ustream_ret ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str);
    utest_assert(ret == USTREAM_OK);

    // No CRLF

    EaiCsvReader reader = eai_csv_reader(',', false);
    i = 0;
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        utest_assert(record != NULL);
        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected[i][field.i],
                                                  strlen(expected[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }
        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);

    // No CRLF
    ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str_crlf);
    utest_assert(ret == USTREAM_OK);
    reader = eai_csv_reader(',', true);

    i = 0;
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        utest_assert(record != NULL);
        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected_crlf[i][field.i],
                                                  strlen(expected_crlf[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }
        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);
    return true;
}

bool test_eai_csv_reader_multiple(void)
{
    srand((unsigned int) time(NULL));
    const char *eai_csv_reader_test_str_0 =
        ",2,3,4,\n"
        "\"\"\"\",6,7\n"
        ",9,\n"
        ",11,\r\n";

    char expected_0[4][5][16] = {
        { "", "2", "3", "4", "" }, { "\"", "6", "7" }, { "", "9", "" }, { "", "11", "\r" }
    };

    int expected_length_0[] = { 5, 3, 3, 3 };

    const char *eai_csv_reader_test_str_1 =
        "should,reallocate,and,move,buffers,\"Lorem ipsum dolor sit amet, consectetur adipiscing "
        "elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
        "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
        "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore "
        "eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa "
        "qui officia deserunt mollit anim id est laborum.\", \"test2\", \"a\"test 3\"b\"\n"
        "\"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "
        "incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud "
        "exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure "
        "dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit "
        "anim id est laborum.\",\"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis "
        "nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute "
        "irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
        "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia "
        "deserunt mollit anim id est laborum.\", \"3\n"
        "4,5\"\",6\"\"\"\n"
        "4,5,6\n"
        "7,8,\" 9\"\"abc\"";

    char expected_1[4][16][1024] = {
        { "should",
          "reallocate",
          "and",
          "move",
          "buffers",
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "
          "incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud "
          "exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure "
          "dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
          "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
          "mollit anim id est laborum.",
          " test2",
          " atest 3b" },
        { "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "
          "incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud "
          "exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure "
          "dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
          "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
          "mollit anim id est laborum.",
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "
          "incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud "
          "exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure "
          "dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
          "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
          "mollit anim id est laborum.",
          " 3\n4,5\",6\"" },
        { "4", "5", "6" },
        { "7", "8", " 9\"abc" }
    };

    int expected_length_1[] = { 8, 3, 3, 3 };

    EaiCsvReader reader = eai_csv_reader(',', false);

    UIStream csv_stream;
    uistream_from_string(&csv_stream, eai_csv_reader_test_str_0);
    ulib_uint i = 0;
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        utest_assert(record != NULL);
        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected_0[i][field.i],
                                                  strlen(expected_0[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }
        utest_assert_int(uvec_count(UString, record), ==, expected_length_0[i]);
        i++;
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    uistream_deinit(&csv_stream);

    i = 0;
    size_t j = 0;
    uistream_from_string(&csv_stream, eai_csv_reader_test_str_1);
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        // Fragment memory
        char *dummy_buffers[64];
        for(j = 0; j < 64; j++) {
            dummy_buffers[j] = (char *) malloc(sizeof(char) * rand() % 1024);
        }

        utest_assert(record != NULL);
        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected_1[i][field.i],
                                                  strlen(expected_1[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }
        utest_assert_int(uvec_count(UString, record), ==, expected_length_1[i]);
        i++;
        for(j = 0; j < 64; j++) {
            free(dummy_buffers[j]);
        }
    }
    utest_assert_int(i, ==, 4);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);
    return true;
}

bool test_eai_csv_reader_errors(void)
{
    // Stream error
    UIStream csv_stream;
    ustream_ret ret = uistream_from_string(&csv_stream, "\"");
    utest_assert(ret == USTREAM_OK);

    // No CRLF
    EaiCsvReader reader = eai_csv_reader(',', false);
    eai_csv_reader_foreach(&reader, &csv_stream, record) {
        return false;
    }
    EaiCsvReaderError state = eai_csv_reader_state(&reader);
    utest_assert(state == EAI_CSV_READER_PARSING_ERROR);

    uistream_deinit(&csv_stream);
    eai_csv_reader_deinit(&reader);
    return true;
}

bool test_eai_csv_reader_header(void)
{
    const char *eai_csv_reader_test_str =
        "test, \"test2\", \"a\"test 3\"b\"\n"
        "1, \"2\", \"3\n"
        "4,5\"\",6\"\"\"\n"
        "4,5,6\n"
        "7,8,\" 9\"\"abc\"";

    const char *eai_csv_reader_test_str_crlf =
        "test; \"test2\"; \"a\"test 3\"b\"\r\n"
        "1; \"2\"; \"3\r\n"
        "4;5\"\";6\"\"\"\r\n"
        "4;5;6\r\n"
        "7;8;\" 9\"\"abc\"";

    char expected_header[4][16] = { "test", " test2", " atest 3b" };

    char expected[3][4][16] = { { "1", " 2", " 3\n4,5\",6\"" },
                                { "4", "5", "6" },
                                { "7", "8", " 9\"abc" } };

    char expected_crlf[3][4][16] = { { "1", " 2", " 3\r\n4;5\";6\"" },
                                     { "4", "5", "6" },
                                     { "7", "8", " 9\"abc" } };

    ulib_uint i;

    int expected_length[] = { 3, 3, 3, 3 };

    UIStream csv_stream;
    ustream_ret ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str);
    utest_assert(ret == USTREAM_OK);

    // No CRLF
    EaiCsvReader reader = eai_csv_reader(',', false);
    i = 0;

    eai_csv_reader_foreach_record(&reader, &csv_stream, header, record)
    {
        utest_assert(record != NULL);
        utest_assert(header != NULL);

        uvec_foreach(UString, header, field) {
            UString expected_field = ustring_wrap(expected_header[field.i],
                                                  strlen(expected_header[field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }

        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected[i][field.i],
                                                  strlen(expected[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }

        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }

    utest_assert_int(i, ==, 3);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);

    // No CRLF
    ret = uistream_from_string(&csv_stream, eai_csv_reader_test_str_crlf);
    utest_assert(ret == USTREAM_OK);
    reader = eai_csv_reader(';', true);

    i = 0;
    eai_csv_reader_foreach_record(&reader, &csv_stream, header, record)
    {
        utest_assert(record != NULL);

        uvec_foreach(UString, header, field) {
            UString expected_field = ustring_wrap(expected_header[field.i],
                                                  strlen(expected_header[field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }

        uvec_foreach(UString, record, field) {
            UString expected_field = ustring_wrap(expected_crlf[i][field.i],
                                                  strlen(expected_crlf[i][field.i]));
            utest_assert_ustring(*field.item, ==, expected_field);
        }

        utest_assert_int(uvec_count(UString, record), ==, expected_length[i]);
        i++;
    }

    utest_assert_int(i, ==, 3);
    utest_assert(eai_csv_reader_state(&reader) == EAI_CSV_READER_OK);
    eai_csv_reader_deinit(&reader);
    uistream_deinit(&csv_stream);
    return true;
}
