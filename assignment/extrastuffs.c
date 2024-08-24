#include "extrastuffs.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static const size_t last_count = 5, middle_count = 5, first_count = 5;

static const char last_names[last_count][10] = {"Nguyen", "Tran", "Le", "Hoang", "Pham"};

static const char middle_names[middle_count][10] = {"Gia", "Tra", "Thanh", "Van", "Thi"};

static const char first_names[first_count][10] = {"Anh", "Ha", "Linh", "Ly", "Duong"};

static void init_random() {
    static int initialized = 0;
    if(!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
}

char *random_name() {
    init_random();

    static char buffer[30];
    sprintf(buffer,
        "%s %s %s",
        last_names[rand() % last_count],
        middle_names[rand() % middle_count],
        first_names[rand() % first_count]
    );
    
    return buffer;
}