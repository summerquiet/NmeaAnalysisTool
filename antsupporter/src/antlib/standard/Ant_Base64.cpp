/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_BASE64_H
#   include "Ant_Base64.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

static const char encode[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz0123456789+/";
static const char decode[] = "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW"
                             "$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*---------------------------------------------------------------------------*/
// Functions
int Ant_Base64::encodeString(const char* in, int in_len, char* out, int out_size)
{
    if (NULL == in || NULL == out) {
        return -1;
    }

    unsigned char triple[3];
    // int line = 0;
    int done = 0;

    while (in_len) {
        int len = 0;
        for (int i = 0; i < 3; i++) {
            if (in_len) {
                triple[i] = *in++;
                len++;
                in_len--;
            } else
                triple[i] = 0;
        }
        if (!len)
            continue;

        if (done + 4 >= out_size)
            return -1;

        *out++ = encode[triple[0] >> 2];
        *out++ = encode[((triple[0] & 0x03) << 4) |
                        ((triple[1] & 0xf0) >> 4)];
        *out++ = (len > 1 ? encode[((triple[1] & 0x0f) << 2) |
                        ((triple[2] & 0xc0) >> 6)] : '=');
        *out++ = (len > 2 ? encode[triple[2] & 0x3f] : '=');

        done += 4;
        // line += 4;
    }

    if (done + 1 >= out_size)
        return -1;

    *out++ = '\0';

    return done;
}

/*
 * returns length of decoded string in out, or -1 if out was too small
 * according to out_size
 */
int Ant_Base64::dcodeString(const char* in, char* out, int out_size)
{
    if (NULL == in || NULL == out) {
        return -1;
    }
    int done = 0;
    unsigned char v;
    unsigned char quad[4];

    while (*in) {

        int len = 0;
        for (int i = 0; i < 4 && *in; i++) {

            v = 0;
            while (*in && !v) {

                v = *in++;
                v = (v < 43 || v > 122) ? 0 : decode[v - 43];

                if (v) {
                    v = (v == '$') ? 0 : v - 61;
                }
                if (*in) {
                    len++;
                    if (v) {
                        quad[i] = v - 1;
                    }
                }
                else {
                    quad[i] = 0;
                }
            }
        }
        if (!len) {
            continue;
        }

        if (out_size < (done + len - 1)) {
            /* out buffer is too small */
            return -1;
        }

        if (len >= 2) {
            *out++ = quad[0] << 2 | quad[1] >> 4;
        }
        if (len >= 3) {
            *out++ = quad[1] << 4 | quad[2] >> 2;
        }
        if (len >= 4) {
            *out++ = ((quad[2] << 6) & 0xc0) | quad[3];
        }

        done += len - 1;
    }

    if (done + 1 >= out_size) {
        return -1;
    }

    *out++ = '\0';

    return done;
}

int Ant_Base64::selfTest(void)
{
    char buf[64];
    static const char * const plaintext[] = {
        "sanity check base 64"
    };
    static const char * const coded[] = {
        "c2FuaXR5IGNoZWNrIGJhc2UgNjQ="
    };

    for (size_t test = 0; test < sizeof(plaintext) / sizeof(plaintext[0]); test++) {

        buf[sizeof(buf) - 1] = '\0';
        int n = encodeString(plaintext[test], strlen(plaintext[test]), buf, sizeof buf);

        if (-1 == n) {
            return -1;
        }
        if (static_cast<size_t>(n) != strlen(coded[test]) || strcmp(buf, coded[test])) {
            // printf("Failed encode selftest %u result '%s' %d\n", test, buf, n);
            return -1;
        }
        // printf("@ori   : %s\n", buf);
        // printf("@ecode : %s\n", coded[test]);

        buf[sizeof(buf) - 1] = '\0';
        n = dcodeString(coded[test], buf);
        if (-1 == n) {
            return -1;
        }
        if (static_cast<size_t>(n) != strlen(plaintext[test]) ||
                strcmp(buf, plaintext[test])) {
            // printf("Failed decode selftest %u result '%s' %d\n", test, buf, n);
            return -1;
        }
        // printf("&deori : %s\n", buf);
        // printf("&ecode : %s\n", coded[test]);
    }

    return 0;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
