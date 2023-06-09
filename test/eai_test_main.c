//
// Created by DavideLoconte on 02-Nov-22.
//
#include <utest.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "tests/eai_csv_reader_test.h"
#include "tests/eai_evaluation_confusion_matrix_test.h"
#include "tests/eai_graph_test.h"
#include "tests/eai_k_fold_test.h"
#include "tests/eai_math_test.h"
#include "tests/eai_profile_test.h"

utest_main({
    utest_run("eai_csv_reader_test", EAI_CSV_READER_TESTS);
    utest_run("eai_math_test", EAI_MATH_TESTS);
    utest_run("eai_evaluation_confusion_matrix_test", EAI_CONFUSION_MATRIX_TESTS);
    utest_run("eai_graph_test", EAI_GRAPH_TESTS);
    utest_run("eai_k_fold_test", EAI_K_FOLD_TESTS);
    utest_run("eai_profile_test", EAI_PROFILE_TESTS);
})
