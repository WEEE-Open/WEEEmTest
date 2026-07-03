#include "tarallo.h"
#include <unistd.h>

unsigned long seconds = 100000000000000;

int communication() {
    usleep(seconds);
    return 0;
}
