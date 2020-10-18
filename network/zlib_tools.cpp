//
// Created by protobit on 18/10/2020.
//

#include "zlib_tools.h"

int deflateSerialData(char* data, char *dest, int lvl, int len) {
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, lvl);
    if (ret != Z_OK)
        return ret;

    for (int i = 0; i < (len/CHUNK)+1; i ++) {
        if (i == len/CHUNK) {
            memcpy(in, &data[len-(len%CHUNK)], len % CHUNK);
            strm.avail_in = len % CHUNK;
        }
        else {
            memcpy(in, &data[i * CHUNK], CHUNK);
            strm.avail_in = CHUNK;
        }

        strm.next_in = in;
        flush = i == (len/CHUNK) ? Z_FINISH : Z_NO_FLUSH;
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            memcpy(&dest[i*CHUNK],out,have);
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);
    }
    (void)deflateEnd(&strm);

    return Z_OK;
}

int inflateSerialData(char *data, char *dest, int lvl, int len) {
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    for (int i = 0; i < (len/CHUNK)+1; i ++) {
        if (i == len/CHUNK) {
            memcpy(in, &data[len-(len%CHUNK)], len % CHUNK);
            strm.avail_in = len % CHUNK;
        }
        else {
            memcpy(in, &data[i * CHUNK], CHUNK);
            strm.avail_in = CHUNK;
        }

        strm.next_in = in;
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
            }
            have = CHUNK - strm.avail_out;
            memcpy(&dest[i*CHUNK],out,have);
            if (ret == Z_STREAM_END)
                break;
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);
    }
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int deflateSerialData(char *data, int lvl, int len) {
    return deflateSerialData(data,data,lvl,len);
}

int inflateSerialData(char *data, int lvl, int len) {
    return inflateSerialData(data,data,lvl,len);
}
