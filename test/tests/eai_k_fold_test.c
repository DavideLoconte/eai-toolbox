//
// Created by DavideLoconte on 07-Nov-22.
//

#include <eai_k_fold.h>
#include <ulib.h>

const char *kfold_test =
    "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n"
    "10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n"
    "20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n"
    "30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n"
    "40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n"
    "50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n";

const char *kfold_test_empty = "";

bool eai_k_fold_test_1(void)
{
    UIStream csv;
    uistream_from_string(&csv, kfold_test);
    EaiCsvReader reader = eai_csv_reader(',', false);
    EaiKFold fold       = eai_k_fold(5);

    ulib_uint train_count[60] = { 0 };
    ulib_uint test_count[60] = { 0 };
    ulib_uint tests[60] = { 0 };

    for(ulib_uint j = 0; j < 60; j++) {
        tests[j] = 0;
        train_count[j] = 0;
        test_count[j]  = 0;
    }

    for(ulib_uint i = 0; i < 5; i++) {
        eai_k_fold_csv_foreach_train(&fold, &reader, &csv, i, record)
        {
            uvec_foreach(UString, record, field) {
                ulib_uint train_s = strtoul(ustring_data(*field.item), NULL, 10);
                tests[train_s]    = 1;
                train_count[train_s] += 1;
            }
        }

        uistream_reset(&csv);

        eai_k_fold_csv_foreach_test(&fold, &reader, &csv, i, record)
        {
            uvec_foreach(UString, record, field) {
                ulib_uint test_s = strtoul(ustring_data(*field.item), NULL, 10);
                utest_assert(tests[test_s] == 0);
                tests[test_s] = 1;
                test_count[test_s] += 1;
            }
        }

        for(ulib_uint j = 0; j < 60; j++) {
            utest_assert(tests[j] == 1);
            tests[j] = 0;
        }
        uistream_reset(&csv);
    }

    for(ulib_uint j = 0; j < 60; j++) {
        utest_assert(train_count[j] == 4);
        utest_assert(test_count[j] == 1);
    }

    uistream_deinit(&csv);
    eai_csv_reader_deinit(&reader);
    eai_k_fold_deinit(&fold);
    return true;
}

bool eai_k_fold_test_2(void)
{
    UIStream csv;
    uistream_from_string(&csv, kfold_test_empty);
    EaiCsvReader reader = eai_csv_reader(',', false);
    EaiKFold fold       = eai_k_fold(5);

    for(ulib_uint i = 0; i < 5; i++) {
        eai_k_fold_csv_foreach_train(&fold, &reader, &csv, i, record) {}

        uistream_reset(&csv);

        eai_k_fold_csv_foreach_test(&fold, &reader, &csv, i, record) {}

        uistream_reset(&csv);
    }

    uistream_deinit(&csv);
    eai_csv_reader_deinit(&reader);
    eai_k_fold_deinit(&fold);
    return true;
}
