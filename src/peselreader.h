#include <string.h>

/*
 * peselreader v0.1.0
 * Copyright © 2014-2017, Chris Warrick.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions, and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author of this software nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    consent.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

char VERSION[16] = "0.1.0";

// The PESEL struct is defined here.  We use it for storing results.
typedef struct PESEL {
    int* peseld;
    char* pesels;
    int year;
    int month;
    int day;
    int female;
    int valid;
} PESEL;

int* split_pesel(char* pesels) {
    int *peseld = NULL;
    peseld = (int*) calloc(11, sizeof(*peseld));
    for (int i = 0; i < 11; i++) {
        peseld[i] = pesels[i] - 0x30;
    }
    return peseld;
}

char* join_pesel(int* peseld) {
    char *pesels = NULL;
    pesels = (char*) calloc(12, sizeof(*pesels));
    for (int i = 0; i < 11; i++) {
        pesels[i] = peseld[i] + 0x30;
    }
    return pesels;
}

int is_valid(int pesel[11]) {
    int sum;
    int m;
    sum = (1 * pesel[0] +
           3 * pesel[1] +
           7 * pesel[2] +
           9 * pesel[3] +
           1 * pesel[4] +
           3 * pesel[5] +
           7 * pesel[6] +
           9 * pesel[7] +
           1 * pesel[8] +
           3 * pesel[9]);
    m = 10 - (sum % 10);
    if (m == 10) m = 0;
    return (m == pesel[10]);
}

int is_female(int pesel[11]) {
    return (pesel[9] % 2 == 0);
}

int get_year(int pesel[11]) {
    int d3 = pesel[0];
    int d4 = pesel[1];
    int dd = pesel[2];
    int d1;
    if (dd % 2 == 1) dd--;

    if (dd == 8) d1 = 1800;
    if (dd == 0) d1 = 1900;
    if (dd == 2) d1 = 2000;
    if (dd == 4) d1 = 2100;
    if (dd == 6) d1 = 2200;

    return d1 + d3 * 10 + d4;
}

int get_month(int pesel[11]) {
    int d1 = pesel[2];
    int d2 = pesel[3];
    if (d1 == 1 || d1 == 3 || d1 == 5 || d1 == 7 || d1 == 9) {
        d1 = 1;
    } else {
        d1 = 0;
    }
    return d1 * 10 + d2;
}

int get_day(int pesel[11]) {
    return pesel[4] * 10 + pesel[5];
}

PESEL parse_pesel(int pesel[11]) {
    PESEL p;
    p.peseld = pesel;
    p.pesels = join_pesel(pesel);
    p.year = get_year(pesel);
    p.month = get_month(pesel);
    p.day = get_day(pesel);
    p.female = is_female(pesel);
    p.valid = is_valid(pesel);
    return p;
}
