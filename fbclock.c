/* fbclock: framebuffer clock for Linux on the Parrot DF3120 digital
 * photo frame.
 *
 * Requires: libpng
 */

#include <stdio.h>

#include <unistd.h>
#include <time.h>
#include <png.h>

#include "fbclock.h"


int main(int argc, char *argv[]) {
    time_t t;
    struct tm *tp;

    /* Display offset from top left of screen, in pixels */
    /* TODO: turn into command-line option */
/*    int x_offset = 0;
    int y_offset = 0; */

    /*int *buffer = get_framebuffer(320, 200);*/

    while (1) {
        /* Get current time */
        t = time(NULL);
        tp = localtime(&t);

        /* Display it */
        display_time(tp);

        /* Wait for next update */
        sleep(SLEEP);
    }
    return 0;
}


/* display_time: Display time on framebuffer.
 * tp: pointer to tm struct such as that returned by localtime().
 */
void display_time(struct tm *tp) {
    int x_pos = 0;
    int y_pos = 0;

    int hours = tp->tm_hour;
    int minutes = tp->tm_min;
    int mday = tp->tm_mday;
    int month_num = tp->tm_mon;
    int year = tp->tm_year + 1900;

    display_png(digit_filenames[mday/10], x_pos, y_pos);
    display_png(digit_filenames[mday%10], x_pos, y_pos);

    display_png(short_month_filenames[month_num], x_pos, y_pos);

    display_png(digit_filenames[year/1000], x_pos, y_pos);
    display_png(digit_filenames[(year%1000)/100], x_pos, y_pos);
    display_png(digit_filenames[(year%100)/10], x_pos, y_pos);
    display_png(digit_filenames[year%10], x_pos, y_pos);

    display_png(digit_filenames[hours/10], x_pos, y_pos);
    display_png(digit_filenames[hours%10], x_pos, y_pos);
    display_png(digit_filenames[minutes/10], x_pos, y_pos);
    display_png(digit_filenames[minutes%10], x_pos, y_pos);

    printf("Time: %s\n", asctime(tp));
    fflush(NULL);

    return;
}


/* display_png: display a PNG file at the given location on the framebuffer.
 * filename: name of the file to display.
 * x_pos, y_pos: x and y coordinates at which to display it on the framebuffer.
 */
void display_png(char *filename, int x_pos, int y_pos) {
    printf("Displaying: %s\n", filename);
    fflush(NULL);
    return;
}
