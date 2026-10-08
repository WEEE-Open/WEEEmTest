#include "tarallo.h"
#include <stdint.h>
#include <unistd.h>
#include "display.h"
#include "keyboard.h"
#include "screen.h"
#include "spd.h"
#include "jedec_id.h"
#include "smbios.h"

// the definitions are copied from the `display.c` module, since they weren't global

const char* form_factor(uint8_t form_code);
void print_spdi_tarallo(spd_info spdi, uint8_t row);

//const uint16_t popup_banner_save_buffer[POP_BANNER_W * POP_BANNER_H];

unsigned int seconds = 1;

//it's the main function to communicate with the lan port and subsequently with the tarallo

int communication() {
    sleep(seconds);
    while (get_key() == '\0') {};
    usleep(1000);

    info_display();
    return 0;
}

// display in a window all the characteristics of the ram sticks before loading them to the tarallo

void info_display() {
    bool big_status_displayed = false;

    if (big_status_displayed) {
        return;
    }

    if (!big_status_displayed) {

        save_screen_region(POP_BANNER_REGION, popup_banner_save_buffer);

        set_background_colour(GREEN);

        clear_screen_region(POP_BANNER_REGION);

        int line = POP_BANNER_R + 1;


        printf(line, POP_BANNER_C + 2, "Slot  - serial number - size - type-freq - ff - ECC - manufacturer");
        line++;

        for (int i = 0; i < 16; i++) {
            spd_info curspd;

            parse_spd(&curspd, i);

            if (curspd.module_size > 0) {
                print_spdi_tarallo(curspd, line);
                line++;
                if (line >= POP_BANNER_R + 8) {
                    break;
                }
            }
        }

        prints(POP_BANNER_R+8, POP_BANNER_C+5, "                                    ");
        prints(POP_BANNER_R+9, POP_BANNER_C+5, "Press any key to remove this banner ");

        set_foreground_colour(palette.foreground);
        set_background_colour(palette.background);
    }
        big_status_displayed = true;
}

// service function to print the ram sticks characteristics on the screen, it prints in order:
// Slot X | serial number| size | type | freq | form_factor | if_ecc | manufacturer

void print_spdi_tarallo(spd_info spdi, uint8_t row)
{
    uint8_t curcol;
    uint16_t i;

     char* form_factor_str = "ASDnknown";

    if (dmi_memory_device != NULL) {
            form_factor_str = form_factor(dmi_memory_device->form);
        }

    //serial_id = get_serial(&dmi_memory_device->header, dmi_memory_device->serialnum);

    // Print Slot Index, Module Size, type & Max frequency (Jedec or XMP)
    curcol = printf(row, POP_BANNER_C + 2, "Slot %i: %s - %iGB - %s-%i - %s",
                        spdi.slot_num,
                        spdi.sku,
                        spdi.module_size / 1024,
                        spdi.type,
                        spdi.freq,
                        form_factor_str);

    // Print ECC status
    if (spdi.hasECC) {
        curcol = prints(row, ++curcol, "| YES");
    } else {
        curcol = prints(row, ++curcol, "| NO");
    }
    // Print Manufacturer from JEDEC106
    for (i = 0; i < JEP106_CNT; i++) {
        if (spdi.jedec_code == jep106[i].jedec_code) {
            curcol = printf(row, ++curcol, " | %s", jep106[i].name);
            break;
        }
    }
}

/*
 * function to get the formfactor of all the RAM sticks, reading only the first one
 */

const char* form_factor(uint8_t form_code) {
    switch(form_code) {
        case 0x09 : return "DIMM";
        case 0x0D : return "SODIMM";
        case 0x0F : return "FB-DIMM";
        default   : return "ASD"; // TODO: when sending to tarallo remember to check if "ASD", and if so DO NOT SEND, it's not in the database
    }
}
