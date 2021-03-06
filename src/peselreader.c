#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "peselreader.h"

/*
 * peselreader v0.1.0
 * Copyright © 2014-2018, Chris Warrick.
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


char* FEMALE = "FEMALE";
char* MALE = "MALE";
int FMSIZE = 6;
char* VALID = "OK";
char* INVALID = "FAIL";
int VSIZE = 4;
char* LENGTHERR = "INVALID LENGTH";
char* ENOTFOUND = "peselreader: %s: No such file or directory\n";
char PSTRING[32];

void set_lang() {
    char* l;
    l = getenv("LANG");
    if (strlen(l) < 2) {
        // c locale
    } else if (l[0] == 'p' && l[1] == 'l') {
        FEMALE = "KOBIETA";
        MALE = "MĘŻCZYZNA";
        FMSIZE = 9;
        VALID = "POPRAWNY";
        INVALID = "NIEPOPRAWNY";
        VSIZE = 11;
        LENGTHERR = "NIEPRAWIDŁOWA DŁUGOŚĆ";
        ENOTFOUND = "peselreader: %s: Nie ma takiego pliku ani katalogu\n";
    }
    sprintf(PSTRING, "%%11s: %%04d-%%02d-%%02d, %%%ds, %%%ds\n", FMSIZE, VSIZE);
}

void print_pesel(int p[11]) {
    PESEL pesel = parse_pesel(p);
    char fs[16];
    char vs[32];
    if (pesel.female) {
        strcpy(fs, FEMALE);
    } else {
        strcpy(fs, MALE);
    }

    if (pesel.valid) {
        strcpy(vs, VALID);
    } else {
        strcpy(vs, INVALID);
    }

    printf(PSTRING, pesel.pesels, pesel.year, pesel.month, pesel.day, fs, vs);
    free(pesel.pesels);
    free(pesel.peseld);
}

void parse_and_print_pesel(char* pesels) {
    if (strlen(pesels) != 11) {
        printf("%11s: %s\n", pesels, LENGTHERR);
    } else {
        print_pesel(split_pesel(pesels));
    }
}

void read_file(FILE* fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        line[strlen(line) - 1] = '\0';
        parse_and_print_pesel(line);
    }
}

int main(int argc, char* argv[]) {
    FILE *fp;
    set_lang();
    if (isatty(fileno(stdin))) {
        if (argc == 1) {
            read_file(stdin);
        }
        for (int a = 1; a < argc; a++) {
            if (strcmp(argv[a], "-") == 0) {
                read_file(stdin);
            } else {
                fp = fopen(argv[a], "r");
                if (fp != NULL) {
                    read_file(fp);
                    fclose(fp);
                } else {
                    printf(ENOTFOUND, argv[a]);
                }
            }
        }
    } else {
        read_file(stdin);
    }
    return 0;
}
