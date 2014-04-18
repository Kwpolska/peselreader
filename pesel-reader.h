#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

char VERSION[16] = "20140418";

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
