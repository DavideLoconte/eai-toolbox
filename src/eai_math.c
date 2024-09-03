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

#include <ulib.h>
#include "eai_math.h"

// By default, assume that the platform supports math.h
#ifndef EAI_TOOLBOX_MATH
#define EAI_TOOLBOX_MATH 1
#endif

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

#if EAI_TOOLBOX_MATH==1

// Functions that requires math.h

#include <math.h>

ulib_float eai_math_sqrt(ulib_float x)
{
    return sqrt(x);
}

#else

ulib_float eai_math_sqrt(ulib_float x)
{
    // Heron's method
    ulib_float x_n = x/2;
    for (ulib_uint i = 0; i < 10; i++) {
        x_n = (x_n + (x / x_n)) / 2;
    }
    return x_n;
}

#endif