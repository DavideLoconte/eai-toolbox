//
// Created by DavideLoconte on 25-Nov-22.
//
#include <eai_confusion_matrix.h>

void test_eai_confusion_matrix_scores_accuracy_1(void)
{
    UVec(UString) symbols = uvec(UString);
    uvec_push(UString, &symbols, ustring_literal("Positive"));
    uvec_push(UString, &symbols, ustring_literal("Negative"));

    EaiConfusionMatrix confusion_matrix = eai_confusion_matrix();

    for(ulib_uint i = 0; i < 100; i++) {
        eai_confusion_matrix_add(&confusion_matrix, 1, 1);
        eai_confusion_matrix_add(&confusion_matrix, 0, 0);
    }

    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 1) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 0) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 1) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 0) == 0);

    for(ulib_uint i = 0; i < 10; i++) {
        eai_confusion_matrix_del(&confusion_matrix, 1, 1);
    }

    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 1) == 100 - 10);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 0) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 1) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 0) == 0);

    utest_assert(eai_confusion_matrix_get_recall(&confusion_matrix, 0) == 1.0);
    utest_assert(eai_confusion_matrix_get_precision(&confusion_matrix, 0) == 1.0);
    utest_assert(eai_confusion_matrix_get_fb_score(&confusion_matrix, 0, 1) == 1.0);

    utest_assert(eai_confusion_matrix_get_recall(&confusion_matrix, 1) == 1.0);
    utest_assert(eai_confusion_matrix_get_precision(&confusion_matrix, 1) == 1.0);
    utest_assert(eai_confusion_matrix_get_fb_score(&confusion_matrix, 1, 1) == 1.0);

    utest_assert(eai_confusion_matrix_get_mean_recall(&confusion_matrix) == 1.0);
    utest_assert(eai_confusion_matrix_get_mean_precision(&confusion_matrix) == 1.0);
    utest_assert(eai_confusion_matrix_get_mean_fb_score(&confusion_matrix, 1) == 1.0);
    utest_assert(eai_confusion_matrix_get_accuracy(&confusion_matrix) == 1.0);

    UString log1 = eai_confusion_matrix_to_string(&confusion_matrix, &symbols);
    UString log2 = eai_confusion_matrix_metrics_to_string(&confusion_matrix, &symbols);

    utest_assert_ustring(log1,
                         ==,
                         ustring_literal("|     a     |     b     |   <-- Classified as\n"
                                         "----------------------------------------------\n"
                                         ":    100    :     0     :   a = Positive\n"
                                         ":     0     :    90     :   b = Negative\n"));

    utest_assert_ustring(log2,
                         ==,
                         ustring_literal("|Precision|Recall   |F1-Score |Accuracy |\n"
                                         "-----------------------------------------\n"
                                         "|  1.000  |  1.000  |  1.000  |         |   Positive\n"
                                         "|  1.000  |  1.000  |  1.000  |         |   Negative\n"
                                         "-----------------------------------------\n"
                                         "|  1.000  |  1.000  |  1.000  |  1.000  |   Average\n"));

    ustring_deinit(&log1);
    ustring_deinit(&log2);

    uvec_deinit(UString, &symbols);
    eai_confusion_matrix_deinit(&confusion_matrix);
}

void test_eai_confusion_matrix_scores_accuracy_05(void)
{
    UVec(UString) symbols = uvec(UString);
    uvec_push(UString, &symbols, ustring_literal("High"));
    uvec_push(UString, &symbols, ustring_literal("Mid"));
    uvec_push(UString, &symbols, ustring_literal("Low"));

    EaiConfusionMatrix confusion_matrix = eai_confusion_matrix();

    for(ulib_uint i = 0; i < 100; i++) {
        eai_confusion_matrix_add(&confusion_matrix, 0, 0);
        eai_confusion_matrix_add(&confusion_matrix, 1, 1);
        eai_confusion_matrix_add(&confusion_matrix, 2, 2);
    }

    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 2, 2) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 1) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 0) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 0) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 2) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 2, 1) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 1) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 2) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 2) == 0);

    for(ulib_uint i = 0; i < 100; i++) {
        eai_confusion_matrix_add(&confusion_matrix, 1, 0);
        eai_confusion_matrix_add(&confusion_matrix, 2, 0);
        eai_confusion_matrix_add(&confusion_matrix, 2, 1);
    }

    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 2, 2) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 1) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 0) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 0) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 2, 0) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 2, 1) == 100);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 1) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 0, 2) == 0);
    utest_assert(eai_confusion_matrix_get(&confusion_matrix, 1, 2) == 0);

    utest_assert(eai_confusion_matrix_get_recall(&confusion_matrix, 0) == 100.0 / 100.0);
    utest_assert(eai_confusion_matrix_get_precision(&confusion_matrix, 0) == 100.0 / 300.0);
    utest_assert(eai_confusion_matrix_get_fb_score(&confusion_matrix, 0, 1) ==
                 2 * (1 / ((100.0 / 100.0) + (300.0 / 100.0))));

    utest_assert(eai_confusion_matrix_get_recall(&confusion_matrix, 1) == 100.0 / 200.0);
    utest_assert(eai_confusion_matrix_get_precision(&confusion_matrix, 1) == 100.0 / 200.0);
    utest_assert(eai_confusion_matrix_get_fb_score(&confusion_matrix, 1, 1) ==
                 2 * (1 / ((200.0 / 100.0) + (200.0 / 100.0))));

    utest_assert(eai_confusion_matrix_get_recall(&confusion_matrix, 2) == 100.0 / 300.0);
    utest_assert(eai_confusion_matrix_get_precision(&confusion_matrix, 2) == 100.0 / 100.0);
    utest_assert(eai_confusion_matrix_get_fb_score(&confusion_matrix, 2, 1) ==
                 2 * (1 / ((100.0 / 100.0) + (300.0 / 100.0))));

    utest_assert(eai_confusion_matrix_get_mean_recall(&confusion_matrix) ==
                 (100.0 / 300.0 + 100.0 / 200.0 + 100.0 / 100.0) / 3);
    utest_assert(eai_confusion_matrix_get_mean_precision(&confusion_matrix) ==
                 (100.0 / 300.0 + 100.0 / 200.0 + 100.0 / 100.0) / 3);
    utest_assert(eai_confusion_matrix_get_mean_fb_score(&confusion_matrix, 1) ==
                 ((2 * (1 / ((100.0 / 100.0) + (300.0 / 100.0)))) +
                  2 * (1 / ((200.0 / 100.0) + (200.0 / 100.0))) +
                  2 * (1 / ((100.0 / 100.0) + (300.0 / 100.0)))) /
                     3);
    utest_assert(eai_confusion_matrix_get_accuracy(&confusion_matrix) == 0.5);

    UString log1 = eai_confusion_matrix_to_string(&confusion_matrix, &symbols);
    UString log2 = eai_confusion_matrix_metrics_to_string(&confusion_matrix, &symbols);

    utest_assert_ustring(log1,
                         ==,
                         ustring_literal("|     a     |     b     |     c     |   <-- Classified "
                                         "as\n"
                                         "---------------------------------------------------------"
                                         "-\n"
                                         ":    100    :     0     :     0     :   a = High\n"
                                         ":    100    :    100    :     0     :   b = Mid\n"
                                         ":    100    :    100    :    100    :   c = Low\n"));

    utest_assert_ustring(log2,
                         ==,
                         ustring_literal("|Precision|Recall   |F1-Score |Accuracy |\n"
                                         "-----------------------------------------\n"
                                         "|  0.333  |  1.000  |  0.500  |         |   High\n"
                                         "|  0.500  |  0.500  |  0.500  |         |   Mid\n"
                                         "|  1.000  |  0.333  |  0.500  |         |   Low\n"
                                         "-----------------------------------------\n"
                                         "|  0.611  |  0.611  |  0.500  |  0.500  |   Average\n"));

    ustring_deinit(&log1);
    ustring_deinit(&log2);

    uvec_deinit(UString, &symbols);
    eai_confusion_matrix_deinit(&confusion_matrix);
}

void test_eai_confusion_matrix_scores_errors(void)
{
    EaiConfusionMatrix confusion_matrix = eai_confusion_matrix();
    for(ulib_uint i = 0; i < 100; i++) {
        eai_confusion_matrix_add(&confusion_matrix, 1, 0);
        eai_confusion_matrix_add(&confusion_matrix, 2, 0);
        eai_confusion_matrix_add(&confusion_matrix, 2, 1);
    }
    ulib_uint i = eai_confusion_matrix_get(&confusion_matrix, 3, 3);
    utest_assert_uint((ulib_uint) i, ==, (ulib_uint) -1);
    utest_assert(confusion_matrix.flags & EAI_CONFUSION_MATRIX_NOT_FOUND_ERR);
    eai_confusion_matrix_deinit(&confusion_matrix);
}
