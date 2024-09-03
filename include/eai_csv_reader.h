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

#ifndef eai_csv__CSV_H
#define eai_csv__CSV_H

#include <ulib.h>
#include "eai_compat.h"

EAI_BEGIN_DECLS

typedef enum EaiCsvReaderErrors_e {
    /// No error encountered
    EAI_CSV_READER_OK = 0x00,

    /// an error with the csv format was encountered
    EAI_CSV_READER_PARSING_ERROR = ubit_bit(8, 7),

    /// reader unable to allocate the required memory
    EAI_CSV_READER_MEM_ERROR = ubit_bit(8, 6),

    /// error encountered during while trying to read the input stream
    EAI_CSV_READER_STREAM_ERROR = ubit_bit(8, 5),

    /// error while trying to push elements into record vector
    EAI_CSV_READER_VEC_ERROR = ubit_bit(8, 4)
} EaiCsvReaderError;

/**
 * The reader object
 * @param _rec temporary record vector
 * @param _buf the reader buffer
 * @param _bufpos the position of the reader in the buffer
 * @param _stream the input stream from which the reader is reading
 * @param _bufsize the size of the buffer. When end of stream is reached, then the
 *                 the buffer size will point to the last character
 * @param _flags reader flags (enable crlf, end reached and dquote found) and state
 * @param _sep the separator character
 *
 */
typedef struct EaiCsvReader_s {
    UVec(UString) _rec;
    char *_buf;
    UIStream *_stream;
    ulib_uint _bufpos;
    ulib_uint _lastsep;
    ulib_uint _bufsize;
    ulib_uint i;
    char _sep;

    UBit(8) _flags;
} EaiCsvReader;

/**
 * Instantiate a new csv reader
 * @param sep the field separator
 * @param crlf if true \r\n will be the line separator, else \n
 * @return A EaiCsvReader instance
 */
EAI_PUBLIC
EaiCsvReader eai_csv_reader(char sep, bool crlf);

/**
 * @param reader the reader
 * @return the state of the reader
 */
EAI_PUBLIC
EaiCsvReaderError eai_csv_reader_state(EaiCsvReader *reader);

/**
 * Destroy the specified reader
 * @param reader a reference to the reader
 */
EAI_PUBLIC
void eai_csv_reader_deinit(EaiCsvReader *reader);

/**
 * A for-like loop that iterates through the record of the the given input stream
 *
 * @example eai_csv_reader_foreach(reader, uistream, record) {
 *      // Do things with UVec(UString) *record
 * }
 *
 * @param reader the reader
 * @param uistream the input stream
 * @param record_identifier the name will be assigned to the UVec(UString) * instance in order to be
 *                          accessible inside the loop
 */
#define eai_csv_reader_foreach_record(reader, uistream, record_identifier)                         \
    for(UVec(UString) *record_identifier = eai_csv_reader_start(reader, uistream);                 \
        record_identifier != NULL;                                                                 \
        record_identifier = eai_csv_reader_next(reader))

/**
 * A for-like loop that iterates through the record of the the given input stream
 * Unlike eai_csv_reader_foreach, it exposes a second vector containing the header
 * of the csv
 *
 *
 * @example eai_csv_reader_foreach(reader, uistream, record) {
 *      // Do things with UVec(UString) *record
 * }
 *
 * @param reader the reader
 * @param uistream the input stream
 * @param record_identifier the name will be assigned to the UVec(UString) * instance in order to be
 *                          accessible inside the loop
 */
#define eai_csv_reader_foreach(reader, uistream, header_identifier, record_identifier)             \
    for(UVec(UString) *header_identifier = eai_csv_reader_start_record(reader, uistream),          \
                      *record_identifier = eai_csv_reader_next_record(reader, header_identifier);  \
        record_identifier != NULL;                                                                 \
        record_identifier = eai_csv_reader_next_record(reader, header_identifier))

/**
 * Start reading a csv from the specified input stream
 * User should not use those function. Use eai_csv_reader_foreach instead
 * @param r the reader
 * @param istream the input stream
 * @return a reference to the first record
 */
EAI_PUBLIC
UVec(UString) * eai_csv_reader_start(EaiCsvReader *r, UIStream *istream);

/**
 * Slide the buffer and get the next record
 * When calling this function, the previous record pointer becomes invalid
 * User should not use this function. Use eai_csv_reader_foreach instead
 * @param r
 * @return a reference to the next record, NULL if none exists
 */
EAI_PUBLIC
UVec(UString) * eai_csv_reader_next(EaiCsvReader *r);

/**
 * Start reading a csv from the specified input stream, copy first record
 * in header vector
 * User should not use those function. Use eai_csv_reader_foreach instead
 * @param r the reader
 * @param istream the input stream
 * @return a reference to the first record
 */
EAI_PUBLIC
UVec(UString) * eai_csv_reader_start_record(EaiCsvReader *r, UIStream *istream);

/**
 * Slide the buffer and get the next record
 * When calling this function, the previous record pointer becomes invalid
 * If no record is available, the function deallocates the header vector
 * User should not use this function. Use eai_csv_reader_foreach instead
 * @param r
 * @param header the header pointer, required to release memory at the end of the loop
 * @return a reference to the next record, NULL if none exists
 */
EAI_PUBLIC
UVec(UString) * eai_csv_reader_next_record(EaiCsvReader *r, UVec(UString) * header);

EAI_END_DECLS

#endif // eai_csv__CSV_H
