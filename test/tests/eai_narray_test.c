#include <eai_narray.h>
#include <eai_narray_builtin.h>

void eai_narray_base_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 3, 3, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);

    utest_assert_uint(eai_narray_size(ulib_float, &array), ==, (3*3*3));
    utest_assert_uint(eai_narray_size(ulib_uint, &array2), ==, (5*5*5*5*5*5*5*5*5*5));

    utest_assert_uint(eai_narray_axes(ulib_float, &array), ==, 3);
    utest_assert_uint(eai_narray_axes(ulib_uint, &array2), ==, 10);

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);
}

void eai_narray_get_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 3, 3, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);

    utest_assert_float(eai_narray_value_at(ulib_float, &array, 2, 2, 2), ==, 0.0);
    utest_assert_uint(eai_narray_value_at(ulib_uint, &array2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4), ==, 0.0);

    utest_assert_float(eai_narray_value_at(ulib_float, &array, 0, 0, 0), ==, 0.0);
    utest_assert_uint(eai_narray_value_at(ulib_uint, &array2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), ==, 0.0);

    utest_assert_float(eai_narray_value_at(ulib_float, &array, 0, 1, 2), ==, 0.0);
    utest_assert_uint(eai_narray_value_at(ulib_uint, &array2, 1, 2, 3, 4, 3, 2, 1, 2, 3, 4), ==, 0.0);

    ulib_uint count = 0;
    UVec(ulib_uint) *shape = eai_narray_shape(ulib_float, &array);

    uvec_foreach(ulib_uint, shape, iter) {
        count++;
        utest_assert_uint(*iter.item, ==, 3);
    }
    utest_assert_uint(count, ==, 3);

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);
}

void eai_narray_shape_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 1, 2, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 5, 1, 2, 3, 4, 5);

    utest_assert_uint(eai_narray_axes(ulib_float, &array), ==, 3);
    utest_assert_uint(eai_narray_axes(ulib_uint, &array2), ==, 5);

    utest_assert_uint(eai_narray_count(ulib_float, &array, 0), ==, 1);
    utest_assert_uint(eai_narray_count(ulib_float, &array, 1), ==, 2);
    utest_assert_uint(eai_narray_count(ulib_float, &array, 2), ==, 3);

    utest_assert_uint(eai_narray_count(ulib_uint, &array2, 0), ==, 1);
    utest_assert_uint(eai_narray_count(ulib_uint, &array2, 1), ==, 2);
    utest_assert_uint(eai_narray_count(ulib_uint, &array2, 2), ==, 3);
    utest_assert_uint(eai_narray_count(ulib_uint, &array2, 3), ==, 4);
    utest_assert_uint(eai_narray_count(ulib_uint, &array2, 4), ==, 5);

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);

}

void eai_narray_set_test(void)
{
    UVec(ulib_uint) shape = uvec(ulib_uint);
    uvec_push(ulib_uint, &shape, 3);
    uvec_push(ulib_uint, &shape, 3);
    uvec_push(ulib_uint, &shape, 3);

    EaiNArray(ulib_float) array = eai_narray_from_shape(ulib_float, &shape);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);

    uvec_deinit(ulib_uint, &shape);

    for(ulib_uint i = 0; i < 1000; i++) {
        ulib_uint idx_1[3] = { rand() % 3, rand() % 3, rand() % 3 };
        ulib_uint idx_2[10] = { rand() % 5, rand() % 5, rand() % 5, rand() % 5, rand() % 5,
                                rand() % 5, rand() % 5, rand() % 5, rand() % 5, rand() % 5 };

        UVec(ulib_uint) coordinates = uvec(ulib_uint);
        uvec_push(ulib_uint, &coordinates, idx_1[0]);
        uvec_push(ulib_uint, &coordinates, idx_1[1]);
        uvec_push(ulib_uint, &coordinates, idx_1[2]);

        ulib_float old_value_1 = eai_narray_value_at_from_coordinates(ulib_float, &array, &coordinates);
        ulib_uint old_value_2 = eai_narray_value_at(ulib_uint,
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

        ulib_float replaced_value_1 = eai_narray_set_value_at_from_coordinates(ulib_float, &array, new_value_1, &coordinates);

        ulib_uint replaced_value_2 = eai_narray_set_value_at(ulib_uint,
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

        ulib_float retrieved_value_1 = eai_narray_value_at_from_coordinates(ulib_float, &array, &coordinates);
        ulib_uint retrieved_value_2 = eai_narray_value_at(ulib_uint,
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

        uvec_deinit(ulib_uint, &coordinates);
    }

    eai_narray_deinit(ulib_float, &array);
    eai_narray_deinit(ulib_uint, &array2);

}

void eai_narray_get_ref_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 3, 3, 3);
    EaiNArray(ulib_uint) array2 = eai_narray(ulib_uint, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5);


    ulib_float *start_1 = eai_narray_value_at_ref(ulib_float, &array, 0, 0, 0);
    ulib_uint *start_2 = eai_narray_value_at_ref(ulib_uint, &array2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    utest_assert_float(*start_1, ==, 0.0);
    utest_assert_float(*start_2, ==, 0.0);

    ulib_float *end_1 = eai_narray_value_at_ref(ulib_float, &array, 2, 2, 2);
    ulib_uint *end_2 = eai_narray_value_at_ref(ulib_uint, &array2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4);

    utest_assert_float(*end_1, ==, 0.0);
    utest_assert_float(*end_2, ==, 0.0);

    for(ulib_uint i = 0; i < 100; i++) {
        ulib_float *x_1 = eai_narray_value_at_ref(ulib_float,
                                             &array,
                                             rand() % 3,
                                             rand() % 3,
                                             rand() % 3);
        ulib_uint *x_2 = eai_narray_value_at_ref(ulib_uint,
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
}

void eai_narray_iter_test(void)
{
    EaiNArray(ulib_float) array = eai_narray(ulib_float, 3, 2, 3, 4);

    ulib_uint count = 0;
    for(ulib_uint i = 0;i < 2; i++) {
        for (ulib_uint j = 0; j < 3; j++) {
            for (ulib_uint k = 0; k < 4; k++) {
                eai_narray_set_value_at(ulib_float, &array, count++, i, j, k);
            }
        }
    }

    eai_narray_foreach(ulib_float, &array, iter) {
        eai_narray_foreach(ulib_float, &iter.item, iter2) {
            eai_narray_foreach(ulib_float, &iter2.item, iter3) {
                eai_narray_foreach(ulib_float, &iter3.item, iter4) {
                    volatile ulib_float val = iter4.item.storage[0];
                    val += 1;
                }
            }
        }
    }

    eai_narray_deinit(ulib_float, &array);
}
