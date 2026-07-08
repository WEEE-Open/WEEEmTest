#include "tarallo.h"
#include <unistd.h>
#include "display.h"
#include "keyboard.h"

#define POP_STAT_R       12
#define POP_STAT_C       18

#define POP_STAT_W       44
#define POP_STAT_H       11

#define POP_STAT_LAST_R  (POP_STAT_R + POP_STAT_H - 1)
#define POP_STAT_LAST_C  (POP_STAT_C + POP_STAT_W - 1)

#define POP_STATUS_REGION  POP_STAT_R, POP_STAT_C, POP_STAT_LAST_R, POP_STAT_LAST_C

static uint16_t popup_status_save_buffer[POP_STAT_W * POP_STAT_H];

unsigned int seconds = 1;

int communication() {
    sleep(seconds);
    while (get_key() == '\0') {};
    usleep(1000);

    info_display();
    return 0;
}


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
                prints(POP_STAT_R+1, POP_STAT_C+5, "");
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
