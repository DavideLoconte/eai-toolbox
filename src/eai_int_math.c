/*
 * MIT License
 *
 * Copyright (c) 2022-2023 Davide Loconte <davide.loconte@hotmail.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ulib.h>

#include "eai_int_math.h"

ulib_uint eai_int_log2(ulib_uint x)
{
    ulib_uint r = 0;
    while(x >>= 1)
        r++;
    return r;
}

ulib_uint eai_int_log10(ulib_uint x)
{
    return (x >= 1000000000) ? 9 :
           (x >= 100000000)  ? 8 :
           (x >= 10000000)   ? 7 :
           (x >= 1000000)    ? 6 :
           (x >= 100000)     ? 5 :
           (x >= 10000)      ? 4 :
           (x >= 1000)       ? 3 :
           (x >= 100)        ? 2 :
           (x >= 10)         ? 1 :
                               0;
}

ulib_uint eai_int_pow2(ulib_uint x) { return 1 << x; }
ulib_uint eai_int_next_pow2(ulib_uint v) { return ulib_uint_ceil2(v); }
