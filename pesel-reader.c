#include <stdio.h>
#include "pesel-reader.h"

char* FEMALE = "FEMALE";
char* MALE = "MALE";
int FMSIZE = 6;
char* VALID = "OK";
char* INVALID = "FAIL";
int VSIZE = 4;
char* LENGTHERR = "INVALID LENGTH";
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

int main(int argc, char* argv[]) {
    set_lang();
    for (int a = 1; a < argc; a++) {
        if (strlen(argv[a]) != 11) {
            printf("%11s: %s\n", argv[a], LENGTHERR);
        } else {
            print_pesel(split_pesel(argv[a]));
        }
    }
    return 0;
}
