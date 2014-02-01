/* fbclock: framebuffer clock for Linux on the Parrot DF3120 digital
 * photo frame.
 *
 * Requires: libpng
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/fb.h>

#include "framebuffer.h"
#include "fbclock.h"
#include "pngimage.h"


int main(int argc, char *argv[]) {
    time_t t;
    struct tm *tp;
    struct framebuffer fb;

    /* Display offset from top left of screen, in pixels */
    /* TODO: turn into command-line option */
    int x_offset = 0;
    int y_offset = 0;

    /* Set up framebuffer */
    new_framebuffer(&fb, "/dev/fb0");

    while (1) {
        /* Get current time */
        t = time(NULL);
        tp = localtime(&t);

        /* Display it */
        display_time(tp, &fb, x_offset, y_offset);

        /* Wait for next update */
        sleep(SLEEP);
    }

    /* Clean up */
    close_framebuffer(&fb);

    return EXIT_SUCCESS;
}


/* display_time: Display time on framebuffer.
 * tp: pointer to tm struct such as that returned by localtime().
 * fb: pointer to framebuffer memory.
 * x_offset, y_offset: x and y position in pixels
 */
void display_time(struct tm *tp, struct framebuffer *fb, int x_offset, int y_offset) {
    int x_pos = x_offset;
    int y_pos = y_offset;

    int hours = tp->tm_hour;
    int minutes = tp->tm_min;
    int mday = tp->tm_mday;
    int month_num = tp->tm_mon;
    int year = tp->tm_year + 1900;

    struct image_size png_size;

    /* TODO: allow user-specified display formats */
    png_size = display_png(fb, digit_filenames[mday/10], x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(fb, digit_filenames[mday%10], x_pos, y_pos);
    x_pos += png_size.x;

    png_size = display_png(fb, short_month_filenames[month_num], x_pos, y_pos);
    x_pos += png_size.x;

    png_size = display_png(fb, digit_filenames[year/1000], x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(fb, digit_filenames[(year%1000)/100], x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(fb, digit_filenames[(year%100)/10], x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(fb, digit_filenames[year%10], x_pos, y_pos);
    x_pos += png_size.x;

    png_size = display_png(fb, digit_filenames[hours/10], x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(fb, digit_filenames[hours%10], x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(fb, digit_filenames[minutes/10], x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(fb, digit_filenames[minutes%10], x_pos, y_pos);

    printf("Time: %s\n", asctime(tp));
    fflush(NULL);

    return;
}

