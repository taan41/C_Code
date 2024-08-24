#include "randname.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static const char last_names[5][7] = {
    "Nguyen", 
    "Tran", 
    "Le",
    "Hoang", 
    "Pham"
};

static const char middle_names[5][6] = {
    "Gia",
    "Tra",
    "Thanh",
    "Van",
    "Thi"
};

static const char first_names[5][6] = {
    "Anh",
    "Ha",
    "Linh",
    "Ly",
    "Duong"
};

void init_random() {
    static int initialized = 0;
    if(!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
}

char *random_name() {
    init_random();

    static char buffer[22];
    sprintf(buffer, "%s %s %s", last_names[rand() % 5], middle_names[rand() % 5], first_names[rand() % 5]);
    return buffer;
}