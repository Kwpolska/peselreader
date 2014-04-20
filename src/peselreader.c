#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "peselreader.h"

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
