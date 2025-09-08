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

#include "eai_csv_reader.h"

#define EAI_CSV_DEBUG
#define EAI_CSV_NULL_TERMINATOR '\0'

/**
 * Reader flags
 */

/**
 * If set, the line ending will be the crlf
 */
#define ENABLE_CRLF ubit_bit(8, 0)

/**
 * If set, the reader red the last character
 */
#define END_REACHED ubit_bit(8, 1)

/**
 * If set, the reader found a double quote during parsing
 */
#define DQUOTE_FOUND ubit_bit(8, 2)

// Private prototypes

/**
 * @param r
 * @return the current character
 */
#define eai_csv_reader_next_char(r) (r->_buf[r->_bufpos])

/**
 * @param r
 * @return the previous character
 */
#define eai_csv_reader_previous_char(r) (r->_buf[r->_bufpos - 1])

/**
 * @param r the reader
 * @return true if CRLF is set as line end
 */
#define eai_csv_reader_crlf(r) (ubit_is_set(8, r->_flags, ENABLE_CRLF))

/**
 * @param r the reader
 * @return true when end of file has been reached
 */
#define eai_csv_reader_end(r)                                                                      \
    ((r->_bufpos >= r->_bufsize - 1) && (ubit_is_set(8, r->_flags, END_REACHED)))

/**
 * @param r the reader
 * @return true if an error occurred
 */
#define eai_csv_reader_error(r)                                                                    \
    ubit_is_any_set(8,                                                                             \
                    r->_flags,                                                                     \
                    EAI_CSV_READER_VEC_ERROR | EAI_CSV_READER_STREAM_ERROR |                       \
                        EAI_CSV_READER_MEM_ERROR | EAI_CSV_READER_PARSING_ERROR)

/**
 * Rotate _buf the current position to the start of the _buf and discarding
 * the overwritten characters
 * @param r the reader
 */
static void eai_csv_reader_rotate(EaiCsvReader *r);

/**
 * Move reader to next character, increase _buf if needed
 * @param r the reader
 * @return the character after the next (if exists)
 */
static void eai_csv_reader_advance(EaiCsvReader *r);

/**
 * Read the current field and appends it to the _rec vector
 * @param r the reader
 */
static void eai_csv_emit_field(EaiCsvReader *r);

/**
 * Emit a field between the specified start and ending positions
 * @param r the reader
 * @param start the starting position in the buffer
 * @param end the ending position in the buffer
 * @return ulib_ret the result of uvec_push into the record
 */
static ulib_ret eai_csv_emit_field_between(EaiCsvReader *r, ulib_uint start, ulib_uint end);

/**
 * Read the current field between double quotes
 * @param r the reader
 */
static void eai_csv_escape_sequence(EaiCsvReader *r);

/**
 * Fills _buf with new data from the starting position
 * @param r the reader
 * @param from the starting position
 */
static void eai_csv_reader_fill_buffer(EaiCsvReader *r, size_t from);

/**
 * @param r the reader
 * @return true if current character is a separator
 */
static bool eai_csv_reader_is_sep(EaiCsvReader *r);

/**
 * @param r the reader
 * @return true if current character is a line ending
 */
static bool eai_csv_reader_is_line_ending(EaiCsvReader *r);

/**
 * Read the record stored in reader buffer
 * @param r the reader
 * @return the record
 */
static UVec(UString) * eai_csv_reader_emit_record(EaiCsvReader *r);

/**
 * Reset the reader to start parsing
 * @param r the reader
 */
static void eai_csv_reader_reset(EaiCsvReader *r);

/**
 * Remove current character and shifts buffer to the left
 * @param r the reader
 */
static void eai_csv_collapse(EaiCsvReader *r);

/**
 * Go back one character in the current buffer
 * @param r the reader
 */
static void eai_csv_reader_recedes(EaiCsvReader *r);

// Public impl =================================================================

EaiCsvReader eai_csv_reader(const char sep, bool crlf)
{
    EaiCsvReader r;
    r._flags = ubit(8, (crlf ? ENABLE_CRLF : 0x00));
    r._sep = sep;
    r._rec = uvec(UString);
    r._stream = NULL;
    r._buf = NULL;
    return r;
}

void eai_csv_reader_deinit(EaiCsvReader *reader)
{
    uvec_deinit(UString, &reader->_rec);
    free(reader->_buf);
}

EaiCsvReaderError eai_csv_reader_state(EaiCsvReader *reader) { return reader->_flags & 0xF0; }

// Protected impl ==============================================================

UVec(UString) * eai_csv_reader_start(EaiCsvReader *r, UIStream *istream)
{
    eai_csv_reader_reset(r);
    uvec_clear(UString, &r->_rec);
    r->_stream = istream;
    eai_csv_reader_fill_buffer(r, 0);
    return eai_csv_reader_emit_record(r);
}

UVec(UString) * eai_csv_reader_next(EaiCsvReader *r)
{
    if(eai_csv_reader_end(r))
        return NULL;
    uvec_clear(UString, &r->_rec);
    eai_csv_reader_rotate(r);
    return eai_csv_reader_emit_record(r);
}

UVec(UString) * eai_csv_reader_start_record(EaiCsvReader *r, UIStream *istream)
{
    UVec(UString) *record = eai_csv_reader_start(r, istream);
    UVec(UString) *header = ulib_malloc(sizeof(UVec(UString)));

    *header = uvec(UString);
    uvec_foreach(UString, record, str) {
        uvec_push(UString, header, ustring_dup(*str.item));
    }

    return header;
}

UVec(UString) * eai_csv_reader_next_record(EaiCsvReader *r, UVec(UString) * header)
{
    UVec(UString) *record = eai_csv_reader_next(r);
    if(record == NULL) {
        uvec_foreach(UString, header, str) {
            ustring_deinit(str.item);
        }
        uvec_deinit(UString, header);
        free(header);
    }
    return record;
}

// Private impl ================================================================

static void eai_csv_reader_reset(EaiCsvReader *r)
{
    r->_bufsize = 32;
    r->_flags = ubit_unset(8, r->_flags, END_REACHED);
    r->_flags = ubit_unset(8, r->_flags, DQUOTE_FOUND);
    if(r->_buf != NULL)
        free(r->_buf);
    r->_buf = (char *) malloc(sizeof(char) * r->_bufsize);
    if(r->_buf == NULL)
        r->_flags = ubit_set(8, r->_flags, EAI_CSV_READER_MEM_ERROR);
    r->_bufpos = 0;
    r->_lastsep = (ulib_uint) -1;
    r->i = (ulib_uint) -1;
}

static UVec(UString) * eai_csv_reader_emit_record(EaiCsvReader *r)
{
    r->i++;
    while(!eai_csv_reader_end(r) && !eai_csv_reader_error(r)) {
        if(eai_csv_reader_is_sep(r)) {
            eai_csv_emit_field(r);
        } else if(eai_csv_reader_is_line_ending(r) && eai_csv_reader_crlf(r)) {
            eai_csv_reader_recedes(r);
            eai_csv_emit_field(r);
            eai_csv_reader_advance(r);
            return eai_csv_reader_error(r) ? NULL : &r->_rec;
        } else if(eai_csv_reader_is_line_ending(r) && !eai_csv_reader_crlf(r)) {
            eai_csv_emit_field(r);
            return eai_csv_reader_error(r) ? NULL : &r->_rec;
        } else if(eai_csv_reader_next_char(r) == '"') {
            eai_csv_escape_sequence(r);
        } else {
            eai_csv_reader_advance(r);
        }
    }

    if(eai_csv_reader_error(r))
        return NULL;
    eai_csv_emit_field(r);
    return &r->_rec;
}

static void eai_csv_escape_sequence(EaiCsvReader *r)
{
    eai_csv_collapse(r);

    while(!eai_csv_reader_end(r) && !eai_csv_reader_error(r)) {
        if(eai_csv_reader_next_char(r) == '"') {
            if(ubit_is_set(8, r->_flags, DQUOTE_FOUND)) {
                r->_flags = ubit_unset(8, r->_flags, DQUOTE_FOUND);
                eai_csv_collapse(r);
            } else {
                r->_flags = ubit_set(8, r->_flags, DQUOTE_FOUND);
                eai_csv_reader_advance(r);
            }
        } else if(eai_csv_reader_next_char(r) != '"' && ubit_is_set(8, r->_flags, DQUOTE_FOUND)) {
            r->_flags = ubit_unset(8, r->_flags, DQUOTE_FOUND);
            r->_bufpos -= 1;
            eai_csv_collapse(r);
            return;
        } else {
            eai_csv_reader_advance(r);
        }
    }

    if(ubit_is_set(8, r->_flags, DQUOTE_FOUND) && (!eai_csv_reader_error(r))) {
        r->_flags = ubit_unset(8, r->_flags, DQUOTE_FOUND);
        r->_bufpos -= 1;
        eai_csv_collapse(r);
    } else {
        r->_flags = ubit_set(8, r->_flags, EAI_CSV_READER_PARSING_ERROR);
    }
}

static void eai_csv_reader_fill_buffer(EaiCsvReader *r, size_t from)
{
    if(ubit_is_set(8, r->_flags, END_REACHED)) {
        r->_buf[from] = EAI_CSV_NULL_TERMINATOR;
        r->_bufsize = (ulib_uint) from + 1;
        return;
    }

    size_t read;
    size_t count = r->_bufsize - (from + 1);
    ulib_ret ret = uistream_read(r->_stream, r->_buf + from, count, &read);

    if(ret != ULIB_OK) {
        r->_flags = ubit_set(8, r->_flags, EAI_CSV_READER_STREAM_ERROR);
    }

    if(read < count) {
        r->_flags = ubit_set(8, r->_flags, END_REACHED);
        r->_bufsize = (ulib_uint) from + read + 1;
    }

    r->_buf[r->_bufsize - 1] = EAI_CSV_NULL_TERMINATOR;
}

static void eai_csv_reader_recedes(EaiCsvReader *r) { r->_bufpos--; }

static void eai_csv_reader_advance(EaiCsvReader *r)
{
    r->_bufpos++;

    if(r->_bufpos + 1 >= r->_bufsize) {
        char *oldbuf = r->_buf;
        r->_bufsize *= 2;
        r->_buf = (char *) realloc(r->_buf, r->_bufsize);

        if(r->_buf == NULL) {
            r->_buf = oldbuf;
            r->_flags = ubit_set(8, r->_flags, EAI_CSV_READER_MEM_ERROR);
            return;
        }

        // Move references on realloc
        if(oldbuf != r->_buf) {
            ulib_uint i, lastsep = (ulib_uint) -1;
            uvec_clear(UString, &r->_rec);
            for(i = 0; i < r->_bufpos; i++) {
                if(r->_buf[i] == EAI_CSV_NULL_TERMINATOR) {
                    eai_csv_emit_field_between(r, lastsep + 1, i);
                    lastsep = i;
                }
            }
        }

        eai_csv_reader_fill_buffer(r, r->_bufpos);
    }
}

static void eai_csv_reader_rotate(EaiCsvReader *r)
{
    size_t count = r->_bufsize - (r->_bufpos + 1);
    memmove(r->_buf, r->_buf + r->_bufpos, count);
    eai_csv_reader_fill_buffer(r, (r->_bufsize - (r->_bufpos + 1)));
    r->_bufpos = 0;
    r->_lastsep = (ulib_uint) -1;
}

static void eai_csv_emit_field(EaiCsvReader *r)
{
    r->_buf[r->_bufpos] = EAI_CSV_NULL_TERMINATOR;
    ulib_ret ret = eai_csv_emit_field_between(r, r->_lastsep + 1, r->_bufpos);
    if(ret != ULIB_OK) {
        r->_flags = ubit_set(8, r->_flags, EAI_CSV_READER_VEC_ERROR);
    }
    r->_lastsep = r->_bufpos;
    eai_csv_reader_advance(r);
}

static ulib_ret eai_csv_emit_field_between(EaiCsvReader *r, ulib_uint start, ulib_uint end)
{
    UString field;
    field = ustring_wrap(r->_buf + start, end - start);
    return uvec_push(UString, &r->_rec, field);
}

static void eai_csv_collapse(EaiCsvReader *r)
{
    memmove(r->_buf + r->_bufpos, r->_buf + (r->_bufpos + 1), r->_bufsize - (r->_bufpos + 1));
    eai_csv_reader_fill_buffer(r, r->_bufsize - 2);
}

static bool eai_csv_reader_is_sep(EaiCsvReader *r)
{
    return eai_csv_reader_next_char(r) == r->_sep;
}

static bool eai_csv_reader_is_line_ending(EaiCsvReader *r)
{
    return (eai_csv_reader_next_char(r) == '\n' && !eai_csv_reader_crlf(r)) ||
           (eai_csv_reader_next_char(r) == '\n' && eai_csv_reader_crlf(r) &&
            eai_csv_reader_previous_char(r) == '\r');
}
