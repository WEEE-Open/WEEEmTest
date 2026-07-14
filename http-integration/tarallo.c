#include "tarallo.h"
#include <stdint.h>
#include <unistd.h>
#include "display.h"
#include "keyboard.h"
#include "spd.h"
#include "jedec_id.h"
#include "smbios.h"

// the definitions are copied from the `display.c` module, since they weren't global

#define POP_STAT_R       12
#define POP_STAT_C       18

#define POP_STAT_W       44
#define POP_STAT_H       11

#define POP_STAT_LAST_R  (POP_STAT_R + POP_STAT_H - 1)
#define POP_STAT_LAST_C  (POP_STAT_C + POP_STAT_W - 1)

#define POP_STATUS_REGION  POP_STAT_R, POP_STAT_C, POP_STAT_LAST_R, POP_STAT_LAST_C

static uint16_t popup_status_save_buffer[POP_STAT_W * POP_STAT_H];

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

        save_screen_region(POP_STATUS_REGION, popup_status_save_buffer);

            //set_background_colour(palette.popup_background);

            clear_screen_region(POP_STATUS_REGION);



            {
                prints(POP_STAT_R+1, POP_STAT_C+5, /*print_spdi_tarallo(spdi, 0)*/"");
                prints(POP_STAT_R+2, POP_STAT_C+5, "");
                prints(POP_STAT_R+3, POP_STAT_C+5, "");
                prints(POP_STAT_R+4, POP_STAT_C+5, "");
                prints(POP_STAT_R+5, POP_STAT_C+5, "");
                prints(POP_STAT_R+6, POP_STAT_C+5, "");
                prints(POP_STAT_R+7, POP_STAT_C+5, "");
            }

        prints(POP_STAT_R+8, POP_STAT_C+5, "                                    ");
        prints(POP_STAT_R+9, POP_STAT_C+5, "Press any key to remove this banner ");

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

    // Print Slot Index, Module Size, type & Max frequency (Jedec or XMP)
    curcol = printf(row, 0, " - Slot %i: %kB %s-%i",
                    spdi.slot_num,
                    spdi.module_size * 1024,
                    spdi.type,
                    spdi.freq,
                    form_factor(dmi_memory_device->form));

    // Print ECC status
    if (spdi.hasECC) {
        curcol = prints(row, ++curcol, "ECC");
    }

    // Print Manufacturer from JEDEC106
    for (i = 0; i < JEP106_CNT; i++) {
        if (spdi.jedec_code == jep106[i].jedec_code) {
            curcol = printf(row, ++curcol, "- %s", jep106[i].name);
            break;
        }
    }
}

const char* form_factor(uint8_t form_code) {
    switch(form_code) {
        case 0x09 : return "DIMM";
        case 0x0D : return "SODIMM";
        case 0x0F : return "FB-DIMM";
        default : return "ASD"; // TODO: when sending to tarallo remember to check if "ASD", and if so DO NOT SEND, it's not in the database
    }
}
