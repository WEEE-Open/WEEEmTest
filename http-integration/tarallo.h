#ifndef TARALLO_H
#define TARALLO_H

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "spd.h"

int communication();

void info_display();

/* needs to display
 * - capacity
 * - speed
 * - ecc (y/n)
 * - form factor (if possible)
 * - type
 * - manufacturer
 */
void print_spdi_tarallo(spd_info spdi, uint8_t row);

const char* form_factor(uint8_t form_code);

#endif
