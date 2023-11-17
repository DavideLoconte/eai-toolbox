#include <eai_narray.h>
#include <eai_narray_builtin.h>

bool eai_narray_base_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 3, 3, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);
    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);
    return true;
}

bool eai_narray_get_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 3, 3, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);

    utest_assert_float(eai_narray_get(ulib_float, &array, 2, 2, 2), ==, 0.0);
    utest_assert_uint(eai_narray_get(ulib_uint, &array2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4), ==, 0.0);

    utest_assert_float(eai_narray_get(ulib_float, &array, 0, 0, 0), ==, 0.0);
    utest_assert_uint(eai_narray_get(ulib_uint, &array2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ==, 0.0);

    utest_assert_float(eai_narray_get(ulib_float, &array, 0, 1, 2), ==, 0.0);
    utest_assert_uint(eai_narray_get(ulib_uint, &array2, 1, 2, 3, 4, 3, 2, 1, 2, 3, 4), ==, 0.0);

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);
    return true;
}

bool eai_narray_shape_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 1, 2, 3);
    EaiNArray(ulib_byte) array2 = eai_narray(ulib_byte, 5, 1, 2, 3, 4, 5);

    utest_assert_uint(eai_narray_axes(ulib_float, &array), ==, 3);
    utest_assert_uint(eai_narray_axes(ulib_byte, &array2), ==, 5);

    utest_assert_uint(eai_narray_count(ulib_float, &array, 0), ==, 1);
    utest_assert_uint(eai_narray_count(ulib_float, &array, 1), ==, 2);
    utest_assert_uint(eai_narray_count(ulib_float, &array, 2), ==, 3);

    utest_assert_uint(eai_narray_count(ulib_byte, &array2, 0), ==, 1);
    utest_assert_uint(eai_narray_count(ulib_byte, &array2, 1), ==, 2);
    utest_assert_uint(eai_narray_count(ulib_byte, &array2, 2), ==, 3);
    utest_assert_uint(eai_narray_count(ulib_byte, &array2, 3), ==, 4);
    utest_assert_uint(eai_narray_count(ulib_byte, &array2, 4), ==, 5);

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_byte, &array2);
    return true;
}

bool eai_narray_set_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 3, 3, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);

    for(ulib_uint i = 0; i < 1000; i++) {
        ulib_uint idx_1[3] = { rand() % 3, rand() % 3, rand() % 3 };
        ulib_uint idx_2[10] = { rand() % 5, rand() % 5, rand() % 5, rand() % 5, rand() % 5,
                                rand() % 5, rand() % 5, rand() % 5, rand() % 5, rand() % 5 };

        ulib_float old_value_1 = eai_narray_get(ulib_float, &array, idx_1[0], idx_1[1], idx_1[2]);
        ulib_uint old_value_2 = eai_narray_get(ulib_uint,
                                               &array2,
                                               idx_2[0],
                                               idx_2[1],
                                               idx_2[2],
                                               idx_2[3],
                                               idx_2[4],
                                               idx_2[5],
                                               idx_2[6],
                                               idx_2[7],
                                               idx_2[8],
                                               idx_2[9]);

        ulib_uint new_value_2 = rand();
        ulib_float new_value_1 = (ulib_float) new_value_2 / 100.0;

        ulib_float replaced_value_1 = eai_narray_set(ulib_float,
                                                     &array,
                                                     new_value_1,
                                                     idx_1[0],
                                                     idx_1[1],
                                                     idx_1[2]);

        ulib_uint replaced_value_2 = eai_narray_set(ulib_uint,
                                                    &array2,
                                                    new_value_2,
                                                    idx_2[0],
                                                    idx_2[1],
                                                    idx_2[2],
                                                    idx_2[3],
                                                    idx_2[4],
                                                    idx_2[5],
                                                    idx_2[6],
                                                    idx_2[7],
                                                    idx_2[8],
                                                    idx_2[9]);

        utest_assert_float(replaced_value_1, ==, old_value_1);
        utest_assert_uint(replaced_value_2, ==, old_value_2);

        ulib_float retrieved_value_1 = eai_narray_get(ulib_float,
                                                      &array,
                                                      idx_1[0],
                                                      idx_1[1],
                                                      idx_1[2]);
        ulib_uint retrieved_value_2 = eai_narray_get(ulib_uint,
                                                     &array2,
                                                     idx_2[0],
                                                     idx_2[1],
                                                     idx_2[2],
                                                     idx_2[3],
                                                     idx_2[4],
                                                     idx_2[5],
                                                     idx_2[6],
                                                     idx_2[7],
                                                     idx_2[8],
                                                     idx_2[9]);

        utest_assert_float(retrieved_value_1, ==, new_value_1);
        utest_assert_uint(retrieved_value_2, ==, new_value_2);

        old_value_1 = retrieved_value_1;
        old_value_2 = retrieved_value_2;
    }

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);
    return true;
}

bool eai_narray_get_ref_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 3, 3, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);

    ulib_float *start_1 = eai_narray_get_ref(ulib_float, &array, 0, 0, 0);
    ulib_uint *start_2 = eai_narray_get_ref(ulib_uint, &array2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    utest_assert_float(*start_1, ==, 0.0);
    utest_assert_float(*start_2, ==, 0.0);

    ulib_float *end_1 = eai_narray_get_ref(ulib_float, &array, 2, 2, 2);
    ulib_uint *end_2 = eai_narray_get_ref(ulib_uint, &array2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4);

    utest_assert_float(*end_1, ==, 0.0);
    utest_assert_float(*end_2, ==, 0.0);

    for(ulib_uint i = 0; i < 100; i++) {
        ulib_float *x_1 = eai_narray_get_ref(ulib_float,
                                             &array,
                                             rand() % 3,
                                             rand() % 3,
                                             rand() % 3);
        ulib_uint *x_2 = eai_narray_get_ref(ulib_uint,
                                            &array2,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5,
                                            rand() % 5);

        utest_assert_float(*x_1, ==, 0.0);
        utest_assert_float(*x_2, ==, 0.0);
        utest_assert_uint(start_1, <=, x_1);
        utest_assert_uint(start_2, <=, x_2);
        utest_assert_uint(x_1, <=, end_1);
        utest_assert_uint(x_2, <=, end_2);
    }

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);
    return true;
}

