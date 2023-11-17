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

#include "eai_profile.h"
#include <ulib.h>

#if defined(__APPLE__) || defined(__unix__)
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

ulib_float eai_profile_get_wall_clock(void)
{
    struct timeval time;
    ulib_int ret = gettimeofday(&time, NULL);
    return ret == 0 ? (ulib_float) time.tv_sec + (((ulib_float) time.tv_usec) / 1000000.0) : -1;
}

ulib_uint eai_profile_get_resident_memory(void)
{
    struct rusage info;
    getrusage(RUSAGE_SELF, &info);
#if __APPLE__
    return (ulib_uint) (info.ru_maxrss) / 1024;
#else
    return (ulib_uint) (info.ru_maxrss);
#endif
}

void eai_profile_sleep(ulib_uint seconds) { sleep(seconds); }
#elif defined _WIN32
#include <Psapi.h>
#include <Windows.h>

ulib_float eai_profile_get_wall_clock(void)
{
    LARGE_INTEGER time, freq;
    if(!QueryPerformanceFrequency(&freq)) {
        //  Handle error
        return -1;
    }
    if(!QueryPerformanceCounter(&time)) {
        //  Handle error
        return -1;
    }
    return (double) time.QuadPart / (double) freq.QuadPart;
}

ulib_uint eai_profile_get_resident_memory(void)
{
    DWORD pid = GetCurrentProcessId();
    HANDLE h_process;
    PROCESS_MEMORY_COUNTERS pmc;

    h_process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if(h_process == NULL) {
        return 0;
    }

    if(GetProcessMemoryInfo(h_process, &pmc, sizeof(pmc))) {
        ulib_uint result = (ulib_uint) pmc.WorkingSetSize / 1024;
        CloseHandle(h_process);
        return result;
    }

    CloseHandle(h_process);
    return -1;
}

void eai_profile_sleep(ulib_uint seconds) { Sleep(seconds * 1000); }
#else

#warning "Platform not supported by eai_profile"

ulib_float eai_profile_get_wall_clock(void) { return (ulib_float) 0.0; }

ulib_uint eai_profile_get_resident_memory(void) { return (ulib_uint) 0; }

void eai_profile_sleep(ulib_uint seconds) {}

#endif
